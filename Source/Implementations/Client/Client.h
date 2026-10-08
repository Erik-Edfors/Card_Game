#pragma once
#include <Implementations/Shared/Network/NetworkManager.h>
#include <Implementations/Shared/Network/Messages.h>
#include <Core/CommonUtilities/Contract.hpp>

#include <map>
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
		struct ServerInfo
		{
			std::string Name;
			std::string Description;
		};

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

		CommonUtilities::Contract<ServerInfo> RequestServerInfo(const networking::SocketAddress& aServer, float aTimeout = 30.f);

		bool RequestConnection(const networking::SocketAddress& aServer);
		bool Disconnect(shared::NETWORK_DISCONNECT_REASON aReason);
		const networking::SocketAddress& GetRemote() const noexcept;

		CLIENT_CONNECTION_STATUS GetConnectionStatus() const noexcept;

		bool IsConnectedTo(const networking::SocketAddress& aServer) const noexcept;

		bool SendTo(const shared::NetworkManager::NetworkMessage& aMessage, const networking::SocketAddress& aAddress);
		bool Send(const shared::NetworkManager::NetworkMessage& aMessage);

		template<class _MessageTy>
		void SetMessageCallback(MessageCallback<_MessageTy> aCallback, void* aUserData, bool aAllowForeign = false);

		template<class _MessageTy>
		bool CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage);

	private:
		static void OnServerInfo(const shared::NetworkManager::MessageCallbackData<shared::NM_ServerInfo>& aData);
		static void OnConnectionResult(const shared::NetworkManager::MessageCallbackData<shared::NM_ConnectionResult>& aData);
		static void OnDisconnect(const shared::NetworkManager::MessageCallbackData<shared::NM_Disconnect>& aData);

		bool SendConnectionRequest(const networking::SocketAddress& aRemote);

		template<class _MessageTy>
		static void MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData);

		struct ServerInfoContract
		{
			networking::SocketAddress Remote;
			CommonUtilities::Task<ServerInfo> Task;
			std::chrono::steady_clock::time_point TimeOfIssue;
			float Timeout;
		};

		std::vector<ServerInfoContract> myServerInfoContracts;

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
			bool AllowForeign;
		};

		std::unordered_map<MessageID, _CallbackEntry> myCallbacks;
	};

	template<class _MessageTy>
	inline bool Client::CreateMessage(shared::NetworkManager::NetworkMessage* outMessage, const _MessageTy& aMessage)
	{
		return myNetworkManager.CreateMessage<_MessageTy>(outMessage, aMessage);
	}

	template<class _MessageTy>
	inline void Client::SetMessageCallback(MessageCallback<_MessageTy> aCallback, void* aUserData, bool aAllowForeign)
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
				.UserData = aUserData,
				.AllowForeign = aAllowForeign
			};
		}
	}

	template<class _MessageTy>
	inline void Client::MessageCallbackDecorator(const shared::NetworkManager::MessageCallbackData<_MessageTy>& aData)
	{
		Client& client = *reinterpret_cast<Client*>(aData.UserData);

		auto it = client.myCallbacks.find(_MessageTy::ID);
		if (it == client.myCallbacks.end())
		{
			return;
		}

		const _CallbackEntry& entry = it->second;

		bool isForeign = not client.IsConnectedTo(aData.Address);

		if (not isForeign)
		{
			client.myLastHeardFromRemote = std::chrono::steady_clock::now();
		}
		else if (not entry.AllowForeign)
		{
			return;
		}

		MessageCallback<_MessageTy> callback = reinterpret_cast<MessageCallback<_MessageTy>>(entry.Callback);

		Client::MessageCallbackData<_MessageTy> data = {
			.Message = aData.Message,
			.Address = aData.Address,
			.UserData = entry.UserData
		};

		callback(data);
	}
}