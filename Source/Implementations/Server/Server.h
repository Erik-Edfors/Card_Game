#pragma once
#include <Implementations/Shared/Network/NetworkManager.h>
#include <Implementations/Shared/Network/Messages.h>

namespace server
{
	class Server
	{
	public:
		//using Client = unsigned int;

		struct ClientInfo
		{
			networking::SocketAddress Remote;
			shared::ClientID ID;
			std::chrono::steady_clock::time_point LastHeardFrom;
		};

		struct ConnectionCallbackData
		{
			networking::SocketAddress Remote;
			shared::ClientID Client;
			void* UserData;
		};
		struct DisconnectionCallbackData
		{
			networking::SocketAddress Remote;
			shared::ClientID Client;
			shared::NETWORK_DISCONNECT_REASON Reason;
			void* UserData;
		};
		template<class _MessageTy>
		struct MessageCallbackData
		{
			_MessageTy Message;
			networking::SocketAddress Address;
			void* UserData;
		};

		using ConnectionCallback = void(*)(const ConnectionCallbackData& aData);
		using DisconnectionCallback = void(*)(const DisconnectionCallbackData& aData);
		template<class _MessageTy>
		using MessageCallback = void(*)(const MessageCallbackData<_MessageTy>& aData);

		Server();
		~Server();

		bool Initialize();
		bool Shutdown();
		void Update();

		bool Bind(networking::Port aPort);
		bool IsBound() const;

		void SetName(std::string_view aName) noexcept;
		const std::string& GetName() const noexcept;

		void SetDescription(std::string_view aDesc) noexcept;
		const std::string& GetDescription() const noexcept;

		shared::ClientID GetClientID(const networking::SocketAddress& aRemote) const noexcept;
		networking::SocketAddress GetClientRemote(shared::ClientID aClient) const noexcept;

		void SetAllowingConnections(bool aAllow) noexcept;
		bool IsAllowingConnections() const noexcept;

		bool DisconnectClient(shared::ClientID aClient, shared::NETWORK_DISCONNECT_REASON aReason);
		bool DisconnectClients(shared::NETWORK_DISCONNECT_REASON aReason);
		bool IsClient(shared::ClientID aClient) const noexcept;
		bool IsClient(const networking::SocketAddress& aAddress) const noexcept;

		size_t GetNumClients() const noexcept;
		size_t GetClients(ClientInfo* outClients, size_t aClientsSize) const noexcept;

		bool SendTo(const shared::NetworkManager::NetworkMessage& aMessage, const networking::SocketAddress& aAddress);
		bool SendToClients(const shared::NetworkManager::NetworkMessage& aMessage);

		template<class _MessageTy>
		void SetMessageCallback(MessageCallback<_MessageTy> aCallback, void* aUserData);

		void SetConnectionCallback(ConnectionCallback aCallback, void* aUserData);
		void SetDisconnectionCallback(DisconnectionCallback aCallback, void* aUserData);

		template<class _MessageTy>
		bool CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage, bool aAssured);

	private:
		static void OnKeepAlive(const MessageCallbackData<shared::NM_KeepAlive>& aData);
		static void OnInfoRequest(const shared::NetworkManager::MessageCallbackData<shared::NM_ServerInfoRequest>& aData);
		static void OnConnectionRequest(const shared::NetworkManager::MessageCallbackData<shared::NM_ConnectionRequest>& aData);
		static void OnDisconnect(const shared::NetworkManager::MessageCallbackData<shared::NM_Disconnect>& aData);
		static void OnAssuredTimeout(const shared::NetworkManager::AssuredTimeoutCallbackData& aData);

		shared::ClientID AddClient(const networking::SocketAddress& aAddress);
		bool RemoveClient(shared::ClientID aClient);
		//bool RemoveClient(const networking::SocketAddress& aAddress);
		bool RemoveClients();

		template<class _MessageTy>
		static void MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData);

		shared::ClientID CreateClientID() noexcept;

		shared::NetworkManager myNetworkManager;

		shared::ClientID myNextClientID;

		struct _ClientEntry
		{
			networking::SocketAddress Address;
			std::chrono::steady_clock::time_point LastHeardFrom;
		};

		struct _MessageCallbackEntry
		{
			void* Callback;
			void* UserData;
		};

		std::string myName;
		std::string myDescription;

		bool myAllowingConnections;

		std::chrono::steady_clock::time_point myLastKeepAlive;

		ConnectionCallback myConnectionCallback;
		void* myConnectionCallbackUserData;

		DisconnectionCallback myDisconnectionCallback;
		void* myDisconnectionCallbackUserData;

		std::unordered_map<shared::ClientID, _ClientEntry> myClients;
		std::unordered_map<networking::SocketAddress, shared::ClientID> myRemoteToClientID;
		std::unordered_map<MessageID, _MessageCallbackEntry> myCallbacks;
	};

	/*
	* Server should be updated.
	* Server should manage connecting clients. User can give a callback that determines if a client is allowed to connect.
	* Server should be able to disconnect clients, sending a disconnect message.
	* Should be easy to allow/disallow connection requests.
	* Should be able to set callbacks for messages. These callbacks should be given the client which sent the message.
	*/

	template<class _MessageTy>
	inline bool Server::CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage, bool aAssured)
	{
		return myNetworkManager.CreateMessage<_MessageTy>(outMessage, aMessage, aAssured);
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
			myCallbacks[_MessageTy::ID] = _MessageCallbackEntry{
				.Callback = aCallback,
				.UserData = aUserData
			};
		}
	}

	template<class _MessageTy>
	inline void Server::MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);
		const shared::ClientID clientID = server.GetClientID(aData.Address);

		if (not server.IsClient(clientID))
		{
			return;
		}

		_ClientEntry& client = server.myClients.at(clientID);
		client.LastHeardFrom = std::chrono::steady_clock::now();

		auto it = server.myCallbacks.find(_MessageTy::ID);
		if (it == server.myCallbacks.end())
		{
			return;
		}

		const _MessageCallbackEntry& entry = it->second;

		MessageCallback<_MessageTy> callback = reinterpret_cast<MessageCallback<_MessageTy>>(entry.Callback);

		Server::MessageCallbackData<_MessageTy> data = {
			.Message = aData.Message,
			.Address = aData.Address,
			.UserData = entry.UserData
		};

		callback(data);
	}
}