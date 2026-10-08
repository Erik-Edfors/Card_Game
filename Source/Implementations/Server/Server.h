#pragma once
#include <Implementations/Shared/Network/NetworkManager.h>
#include <Implementations/Shared/Network/Messages.h>

namespace server
{
	class Server
	{
	public:
		//using Client = unsigned int;

		template<class _MessageTy>
		struct MessageCallbackData
		{
			_MessageTy Message;
			networking::SocketAddress Address;
			void* UserData;
		};

		template<class _MessageTy>
		using MessageCallback = void(*)(const MessageCallbackData<_MessageTy>& aData);

		Server();
		~Server();

		bool Initialize(networking::Port aPort);
		bool Shutdown();
		void Update();

		void SetAllowingConnections(bool aAllow) noexcept;
		bool IsAllowingConnections() const noexcept;

		bool DisconnectClient(const networking::SocketAddress& aAddress, shared::NETWORK_DISCONNECT_REASON aReason);
		bool DisconnectClients(shared::NETWORK_DISCONNECT_REASON aReason);
		bool IsClient(const networking::SocketAddress& aAddress) const noexcept;

		size_t GetNumClients() const noexcept;

		bool SendTo(const shared::NetworkManager::NetworkMessage& aMessage, const networking::SocketAddress& aAddress);
		bool SendToClients(const shared::NetworkManager::NetworkMessage& aMessage);

		template<class _MessageTy>
		void SetMessageCallback(MessageCallback<_MessageTy> aCallback, void* aUserData);

		template<class _MessageTy>
		bool CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage);

	private:
		static void OnInfoRequest(const shared::NetworkManager::MessageCallbackData<shared::NM_ServerInfoRequest>& aData);
		static void OnConnectionRequest(const shared::NetworkManager::MessageCallbackData<shared::NM_ConnectionRequest>& aData);
		static void OnDisconnect(const shared::NetworkManager::MessageCallbackData<shared::NM_Disconnect>& aData);

		void AddClient(const networking::SocketAddress& aAddress);
		bool RemoveClient(const networking::SocketAddress& aAddress);
		bool RemoveClients();

		template<class _MessageTy>
		static void MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData);

		shared::NetworkManager myNetworkManager;

		struct _ClientEntry
		{
			networking::SocketAddress Address;
			std::chrono::steady_clock::time_point LastHeardFrom;
		};

		struct _CallbackEntry
		{
			void* Callback;
			void* UserData;
		};

		bool myAllowingConnections;

		std::unordered_map<networking::SocketAddress, _ClientEntry> myClients;
		std::unordered_map<MessageID, _CallbackEntry> myCallbacks;
	};

	/*
	* Server should be updated.
	* Server should manage connecting clients. User can give a callback that determines if a client is allowed to connect.
	* Server should be able to disconnect clients, sending a disconnect message.
	* Should be easy to allow/disallow connection requests.
	* Should be able to set callbacks for messages. These callbacks should be given the client which sent the message.
	*/

	template<class _MessageTy>
	inline bool Server::CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage)
	{
		return myNetworkManager.CreateMessage<_MessageTy>(outMessage, aMessage);
	}

	template<class _MessageTy>
	inline void Server::SetMessageCallback(MessageCallback<_MessageTy> aCallback, void* aUserData)
	{
		myNetworkManager.SetCallback<_MessageTy>(&MessageCallbackDecorator<_MessageTy>, this);
		if (aCallback == nullptr)
		{
			myCallbacks.erase(_MessageTy::ID);
		}
		else
		{
			myCallbacks[_MessageTy::ID] = _CallbackEntry{
				.Callback = aCallback,
				.UserData = aUserData
			};
		}
	}

	template<class _MessageTy>
	inline void Server::MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);

		if (not server.IsClient(aData.Address))
		{
			return;
		}

		_ClientEntry& client = server.myClients.at(aData.Address);
		client.LastHeardFrom = std::chrono::steady_clock::now();

		auto it = server.myCallbacks.find(_MessageTy::ID);
		if (it == server.myCallbacks.end())
		{
			return;
		}

		const _CallbackEntry& entry = it->second;

		MessageCallback<_MessageTy> callback = reinterpret_cast<MessageCallback<_MessageTy>>(entry.Callback);

		Server::MessageCallbackData<_MessageTy> data = {
			.Message = aData.Message,
			.Address = aData.Address,
			.UserData = entry.UserData
		};

		callback(data);
	}
}