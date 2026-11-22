#include "Server.pch.h"
#include "Server.h"

#include <Implementations/Shared/Network/Messages.h>
#include <ranges>
#include <set>

namespace server
{
	void Server::OnKeepAlive(const MessageCallbackData<shared::NM_KeepAlive>& aData)
	{
		aData;
	}
	void Server::OnInfoRequest(const shared::NetworkManager::MessageCallbackData<shared::NM_ServerInfoRequest>& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);
		if (server.IsAllowingConnections())
		{
			shared::NM_ServerInfo info = {
				.Name = server.GetName(),
				.Description = server.GetDescription()
			};
			shared::NetworkManager::NetworkMessage message;
			server.CreateMessage(&message, info, false);
			server.SendTo(message, aData.Address);
		}
	}
	void Server::OnConnectionRequest(const shared::NetworkManager::MessageCallbackData<shared::NM_ConnectionRequest>& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);

		if (server.IsAllowingConnections())
		{
			shared::ClientID client = server.AddClient(aData.Address);

			shared::NM_ConnectionResult connectionResult = {
				.Accepted = true
			};
			shared::NetworkManager::NetworkMessage message;
			server.CreateMessage(&message, connectionResult, true);
			server.SendTo(message, aData.Address);

			if (server.myConnectionCallback != nullptr)
			{
				ConnectionCallbackData data = {
					.Remote = aData.Address,
					.Client = client,
					.UserData = server.myConnectionCallbackUserData
				};
				server.myConnectionCallback(data);
			}
		}
		else
		{
			shared::NM_ConnectionResult connectionResult = {
				.Accepted = false
			};
			shared::NetworkManager::NetworkMessage message;
			server.CreateMessage(&message, connectionResult, true);
			server.SendTo(message, aData.Address);
		}
	}

	void Server::OnDisconnect(const shared::NetworkManager::MessageCallbackData<shared::NM_Disconnect>& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);

		const shared::ClientID id = server.GetClientID(aData.Address);

		if (server.IsClient(id))
		{
			server.RemoveClient(id);

			//shared::NM_Disconnect disconnect = {
			//	.Client = id,
			//	.Reason = aData.Message.Reason
			//};
			//shared::NetworkManager::NetworkMessage message;
			//if (not server.myNetworkManager.CreateMessage(&message, disconnect, false))
			//{
			//	// TODO: Log failed to create message.
			//	// This causes a dirty disconnect.
			//	// Meaning the server will no longer treat 
			//	// the remot as a client, but the remote will not know that.
			//}
			//else
			//{
			//	// Send to other clients. Not disconnecting client.
			//	if (not server.SendToClients(message))
			//	{
			//		//TODO: Log failed to send disconnect.
			//		// This causes a dirty disconnect.
			//		// Meaning the server will no longer treat 
			//		// the remot as a client, but the remote will not know that.
			//	}
			//}

			if (server.myDisconnectionCallback != nullptr)
			{
				DisconnectionCallbackData data = {
					.Remote = aData.Address,
					.Client = id,
					.Reason = aData.Message.Reason,
					.UserData = server.myDisconnectionCallbackUserData,
				};
				server.myDisconnectionCallback(data);
			}
		}

	}

	void Server::OnAssuredTimeout(const shared::NetworkManager::AssuredTimeoutCallbackData& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);

		shared::ClientID client = server.GetClientID(aData.Remote);
		if (client != shared::INVALID_CLIENT_ID)
		{
			server.DisconnectClient(client, shared::NETWORK_DISCONNECT_TIMED_OUT);
		}
	}

	Server::Server() :
		myNextClientID(shared::INVALID_CLIENT_ID + 1),
		myAllowingConnections(false),
		myConnectionCallback(nullptr),
		myConnectionCallbackUserData(nullptr),
		myDisconnectionCallback(nullptr),
		myDisconnectionCallbackUserData(nullptr),
		myLastKeepAlive({})
	{
	}

	Server::~Server()
	{
	}

	bool Server::Initialize()
	{
		if (not myNetworkManager.Initialize(4096))
		{
			//TODO: Log failed to initialize.
			return false;
		}

		SetMessageCallback<shared::NM_KeepAlive>(&OnKeepAlive, nullptr);
		myNetworkManager.SetCallback<shared::NM_ServerInfoRequest>(&OnInfoRequest, this);
		myNetworkManager.SetCallback<shared::NM_ConnectionRequest>(&OnConnectionRequest, this);
		myNetworkManager.SetCallback<shared::NM_Disconnect>(&OnDisconnect, this);
		myNetworkManager.SetAssuredTimeoutCallback(&OnAssuredTimeout, this);

		return true;
	}
	bool Server::Shutdown()
	{
		bool result = true;

		DisconnectClients(shared::NETWORK_DISCONNECT_END_OF_SESSION);

		myCallbacks.clear();

		SetAllowingConnections(false);

		result &= myNetworkManager.Shutdown();

		return result;
	}
	void Server::Update()
	{
		myNetworkManager.Update(64);

		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		std::vector<shared::ClientID> timedOutClients;
		for (const auto& entry : myClients)
		{
			const shared::ClientID id = entry.first;
			const _ClientEntry& client = entry.second;
			const bool timedOut = std::chrono::duration<float>(now - client.LastHeardFrom).count() > 30.f; // TODO: Allow setting timeout.

			if (timedOut)
			{
				timedOutClients.push_back(id);
			}
		}

		for (const shared::ClientID client : timedOutClients)
		{
			DisconnectClient(client, shared::NETWORK_DISCONNECT_TIMED_OUT);
		}

		// Sending ping to server to keep connection alive
		if (GetNumClients() != 0)
		{
			//TODO: Allow user to set interval.
			constexpr float keepAliveInterval = 5.f;
			if (std::chrono::duration<float>(now - myLastKeepAlive).count() > keepAliveInterval)
			{
				shared::NM_KeepAlive keepAlive = {
				};

				shared::NetworkManager::NetworkMessage message;
				if (CreateMessage(&message, keepAlive, false))
				{
					if (SendToClients(message))
					{
						myLastKeepAlive = now;
					}
				}
			}
		}
	}

	bool Server::Bind(networking::Port aPort)
	{
		return myNetworkManager.Bind(aPort);
	}
	bool Server::IsBound() const
	{
		return myNetworkManager.IsBound();
	}

	void Server::SetName(std::string_view aName) noexcept
	{
		myName = aName;
	}
	const std::string& Server::GetName() const noexcept
	{
		return myName;
	}

	void Server::SetDescription(std::string_view aDesc) noexcept
	{
		myDescription = aDesc;
	}
	const std::string& Server::GetDescription() const noexcept
	{
		return myDescription;
	}

	shared::ClientID Server::GetClientID(const networking::SocketAddress& aRemote) const noexcept
	{
		auto it = myRemoteToClientID.find(aRemote);
		if (it != myRemoteToClientID.end())
		{
			return it->second;
		}
		return shared::INVALID_CLIENT_ID;
	}

	networking::SocketAddress Server::GetClientRemote(shared::ClientID aClient) const noexcept
	{
		auto it = myClients.find(aClient);
		if (it != myClients.end())
		{
			return it->second.Address;
		}
		return {};
	}

	void Server::SetAllowingConnections(bool aAllow) noexcept
	{
		myAllowingConnections = aAllow;
	}

	bool Server::IsAllowingConnections() const noexcept
	{
		return myAllowingConnections;
	}

	shared::ClientID Server::AddClient(const networking::SocketAddress& aAddress)
	{
		const shared::ClientID id = CreateClientID();
		myRemoteToClientID[aAddress] = id;
		myClients[id] = _ClientEntry{
			.Address = aAddress,
			.LastHeardFrom = std::chrono::steady_clock::now()
		};
		return id;
	}

	bool Server::RemoveClient(shared::ClientID aClient)
	{
		auto it = myClients.find(aClient);
		if (it == myClients.end())
		{
			return false;
		}
		myRemoteToClientID.erase(it->second.Address);
		myClients.erase(it);
		return true;
	}

	bool Server::RemoveClients()
	{
		myClients.clear();
		myRemoteToClientID.clear();
		return true;
	}

	shared::ClientID Server::CreateClientID() noexcept
	{
		return myNextClientID++;
	}

	bool Server::DisconnectClient(shared::ClientID aClient, shared::NETWORK_DISCONNECT_REASON aReason)
	{
		if (not IsClient(aClient))
		{
			return false;
		}

		shared::NM_Disconnect disconnect = {
			.Reason = aReason
		};
		shared::NetworkManager::NetworkMessage message;
		if (not myNetworkManager.CreateMessage(&message, disconnect, false))
		{
			// TODO: Log failed to create message.
			// This causes a dirty disconnect.
			// Meaning the server will no longer treat 
			// the remot as a client, but the remote will not know that.
		}
		else
		{
			// Send to both other clients, and removed client.
			if (not SendTo(message, GetClientRemote(aClient)))
			{
				//TODO: Log failed to send disconnect.
				// This causes a dirty disconnect.
				// Meaning the server will no longer treat 
				// the remot as a client, but the remote will not know that.
			}
		}

		if (myDisconnectionCallback != nullptr)
		{
			DisconnectionCallbackData data = {
				.Remote = GetClientRemote(aClient),
				.Client = aClient,
				.Reason = aReason,
				.UserData = myDisconnectionCallbackUserData,
			};
			myDisconnectionCallback(data);
		}


		if (not RemoveClient(aClient))
		{
			// Is not a client, therefore does not need to be disconnected.
			return false;
		}

		return true;
	}

	bool Server::DisconnectClients(shared::NETWORK_DISCONNECT_REASON aReason)
	{
		for (const auto& entry : myClients)
		{
			shared::NM_Disconnect disconnect = {
				.Reason = aReason
			};
			shared::NetworkManager::NetworkMessage message;
			if (not myNetworkManager.CreateMessage(&message, disconnect, false))
			{
				// TODO: Log failed to create message.
				// This causes a dirty disconnect.
				// Meaning the server will no longer treat 
				// the remot as a client, but the remote will not know that.
			}
			else
			{
				if (not SendTo(message, entry.second.Address))
				{
					//TODO: Log failed to send disconnect.
					// This causes a dirty disconnect.
					// Meaning the server will no longer treat 
					// the remot as a client, but the remote will not know that.
				}
			}
		}

		if (myDisconnectionCallback != nullptr)
		{
			for (const auto& entry : myClients)
			{
				const shared::ClientID clientID = entry.first;
				const _ClientEntry& client = entry.second;

				DisconnectionCallbackData data = {
					.Remote = client.Address,
					.Client = clientID,
					.Reason = aReason,
					.UserData = myDisconnectionCallbackUserData,
				};
				myDisconnectionCallback(data);
			}
		}

		if (not RemoveClients())
		{
			// Failed to remove clients.
			return false;
		}

		return true;
	}

	bool Server::IsClient(shared::ClientID aClient) const noexcept
	{
		return myClients.contains(aClient);
	}

	bool Server::IsClient(const networking::SocketAddress& aAddress) const noexcept
	{
		return myClients.contains(GetClientID(aAddress));
	}

	size_t Server::GetNumClients() const noexcept
	{
		return myClients.size();
	}

	size_t Server::GetClients(ClientInfo* outClients, size_t aClientsSize) const noexcept
	{
		size_t count = 0;

		for (const auto& entry : myClients)
		{
			if (count >= aClientsSize)
			{
				break;
			}

			const _ClientEntry& client = entry.second;
			ClientInfo& out = outClients[count++];

			out = ClientInfo{
				.Remote = client.Address,
				.LastHeardFrom = client.LastHeardFrom
			};
		}

		return count;
	}

	bool Server::SendTo(const shared::NetworkManager::NetworkMessage& aMessage, const networking::SocketAddress& aAddress)
	{
		if (not IsBound())
		{
			return false;
		}
		return myNetworkManager.SendTo(aMessage, aAddress);
	}

	bool Server::SendToClients(const shared::NetworkManager::NetworkMessage& aMessage)
	{
		if (not IsBound())
		{
			return false;
		}
		bool result = true;
		for (auto& entry : myClients)
		{
			result &= SendTo(aMessage, entry.second.Address);
		}
		return result;
	}
	void Server::SetConnectionCallback(ConnectionCallback aCallback, void* aUserData)
	{
		myConnectionCallback = aCallback;
		myConnectionCallbackUserData = aUserData;
	}
	void Server::SetDisconnectionCallback(DisconnectionCallback aCallback, void* aUserData)
	{
		myDisconnectionCallback = aCallback;
		myDisconnectionCallbackUserData = aUserData;
	}
}