#pragma once
#include <Implementations/Shared/Network/NetworkManager.h>
#include <Implementations/Shared/Network/Messages.h>
//#include <Core/Networking/Defines.h>

namespace client
{
	enum
	{
		CLIENT_CONNECTION_DISCONNECTED,
		CLIENT_CONNECTION_PENDING,
		CLIENT_CONNECTION_CONNECTED
	};
	using CLIENT_CONNECTION_STATUS = unsigned char;

	class Client
	{
	public:
		template<class _MessageTy>
		struct MessageCallbackData
		{
			_MessageTy Message;
			networking::SocketAddress Address;
			void* UserData;
		};

		template<class _MessageTy>
		using MessageCallback = void(*)(const MessageCallbackData<_MessageTy>& aData);

		Client();
		~Client();

		bool Initialize(networking::Port aPort);
		bool Shutdown();
		void Update();

		bool RequestConnection(const networking::SocketAddress& aServer);
		bool Disconnect(shared::NETWORK_DISCONNECT_REASON aReason);
		const networking::SocketAddress& GetRemote() const noexcept;

		CLIENT_CONNECTION_STATUS GetConnectionStatus() const noexcept;

		bool IsConnectedTo(const networking::SocketAddress& aServer) const noexcept;

		bool SendTo(const shared::NetworkManager::NetworkMessage& aMessage, const networking::SocketAddress& aAddress);
		bool Send(const shared::NetworkManager::NetworkMessage& aMessage);

		template<class _MessageTy>
		void SetMessageCallback(MessageCallback<_MessageTy> aCallback, void* aUserData);

		template<class _MessageTy>
		bool CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage);

	private:
		static void OnConnectionResult(const shared::NetworkManager::MessageCallbackData<shared::NM_ConnectionResult>& aData);
		static void OnDisconnect(const shared::NetworkManager::MessageCallbackData<shared::NM_Disconnect>& aData);

		bool SendConnectionRequest(const networking::SocketAddress& aRemote);

		template<class _MessageTy>
		static void MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData);

		shared::NetworkManager myNetworkManager;
		size_t myConnectionAttempts;
		std::chrono::steady_clock::time_point myConnectionStart;
		std::chrono::steady_clock::time_point myLastHeardFromRemote;
		networking::SocketAddress myServer;

		CLIENT_CONNECTION_STATUS myConnectionStatus;

		struct _CallbackEntry
		{
			void* Callback;
			void* UserData;
		};

		std::unordered_map<MessageID, _CallbackEntry> myCallbacks;
	};

	template<class _MessageTy>
	inline bool Client::CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage)
	{
		return myNetworkManager.CreateMessage<_MessageTy>(outMessage, aMessage);
	}

	template<class _MessageTy>
	inline void Client::SetMessageCallback(MessageCallback<_MessageTy> aCallback, void* aUserData)
	{
		myNetworkManager.SetCallback<_MessageTy>(&MessageCallbackDecorator<_MessageTy>, this);
		myCallbacks[_MessageTy::ID] = _CallbackEntry{
			.Callback = aCallback,
			.UserData = aUserData
		};
	}

	template<class _MessageTy>
	inline void Client::MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData)
	{
		Client& client = *reinterpret_cast<Client*>(aData.UserData);

		if (not client.IsConnectedTo(aData.Address))
		{
			return;
		}

		client.myLastHeardFromRemote = std::chrono::steady_clock::now();

		auto it = client.myCallbacks.find(_MessageTy::ID);
		if (it == client.myCallbacks.end())
		{
			return;
		}

		const _CallbackEntry& entry = it->second;

		MessageCallback<_MessageTy> callback = reinterpret_cast<MessageCallback<_MessageTy>>(entry.Callback);

		Client::MessageCallbackData<_MessageTy> data = {
			.Message = aData.Message,
			.Address = aData.Address,
			.UserData = entry.UserData
		};

		callback(data);
	}
}