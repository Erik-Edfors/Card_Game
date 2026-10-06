#include "Server.pch.h"
#include "Server.h"

#include <Implementations/Shared/Network/Messages.h>
#include <ranges>
#include <set>

namespace server
{
	void Server::OnConnectionRequest(const shared::NetworkManager::MessageCallbackData<shared::NM_ConnectionRequest>& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);

		if (server.IsAllowingConnections())
		{
			server.AddClient(aData.Address);

			shared::NM_ConnectionResult connectionResult = {
				.Accepted = true
			};
			shared::NetworkManager::NetworkMessage message;
			server.CreateMessage(&message, connectionResult);
			server.SendTo(message, aData.Address);
		}
		else
		{
			shared::NM_ConnectionResult connectionResult = {
				.Accepted = false
			};
			shared::NetworkManager::NetworkMessage message;
			server.CreateMessage(&message, connectionResult);
			server.SendTo(message, aData.Address);
		}
	}

	void Server::OnDisconnect(const shared::NetworkManager::MessageCallbackData<shared::NM_Disconnect>& aData)
	{
		Server& server = *reinterpret_cast<Server*>(aData.UserData);

		const networking::SocketAddress& client = aData.Address;

		if (server.IsClient(client))
		{
			server.RemoveClient(client);
		}
	}

	Server::Server() :
		myAllowingConnections(false)
	{
	}

	Server::~Server()
	{
	}

	bool Server::Initialize(networking::Port aPort)
	{
		if (not myNetworkManager.Initialize(4096, aPort))
		{
			return false;
		}

		myNetworkManager.SetCallback<shared::NM_ConnectionRequest>(&OnConnectionRequest, this);
		myNetworkManager.SetCallback<shared::NM_Disconnect>(&OnDisconnect, this);

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

		std::vector<networking::SocketAddress> timedOutClients;
		for (const auto& entry : myClients)
		{
			const _ClientEntry& client = entry.second;
			const bool timedOut = std::chrono::duration<float>(now - client.LastHeardFrom).count() > 30.f; // TODO: Allow setting timeout.

			if (timedOut)
			{
				timedOutClients.push_back(client.Address);
			}
		}

		for (const networking::SocketAddress& client : timedOutClients)
		{
			DisconnectClient(client, shared::NETWORK_DISCONNECT_TIMED_OUT);
		}
	}

	void Server::SetAllowingConnections(bool aAllow) noexcept
	{
		myAllowingConnections = aAllow;
	}

	bool Server::IsAllowingConnections() const noexcept
	{
		return myAllowingConnections;
	}

	void Server::AddClient(const networking::SocketAddress& aAddress)
	{
		myClients[aAddress] = _ClientEntry{
			.Address = aAddress,
			.LastHeardFrom = std::chrono::steady_clock::now()
		};
	}

	bool Server::RemoveClient(const networking::SocketAddress& aAddress)
	{
		return myClients.erase(aAddress) != 0;
	}

	bool Server::RemoveClients()
	{
		myClients.clear();
		return true;
	}

	bool Server::DisconnectClient(const networking::SocketAddress& aAddress, shared::NETWORK_DISCONNECT_REASON aReason)
	{
		if (not RemoveClient(aAddress))
		{
			// Is not a client, therefore does not need to be disconnected.
			return false;
		}

		shared::NM_Disconnect disconnect = {
			.Reason = aReason
		};
		shared::NetworkManager::NetworkMessage message;
		if (not myNetworkManager.CreateMessage(&message, disconnect))
		{
			// TODO: Log failed to create message.
			// This causes a dirty disconnect.
			// Meaning the server will no longer treat 
			// the remot as a client, but the remote will not know that.
		}
		else
		{
			if (not myNetworkManager.SendTo(message, aAddress))
			{
				//TODO: Log failed to send disconnect.
				// This causes a dirty disconnect.
				// Meaning the server will no longer treat 
				// the remot as a client, but the remote will not know that.
			}
		}

		return true;
	}

	bool Server::DisconnectClients(shared::NETWORK_DISCONNECT_REASON aReason)
	{
		shared::NM_Disconnect disconnect = {
			.Reason = aReason
		};
		shared::NetworkManager::NetworkMessage message;
		if (not myNetworkManager.CreateMessage(&message, disconnect))
		{
			// TODO: Log failed to create message.
			// This causes a dirty disconnect.
			// Meaning the server will no longer treat 
			// the remot as a client, but the remote will not know that.
		}
		else
		{
			if (not SendToClients(message))
			{
				//TODO: Log failed to send disconnect.
				// This causes a dirty disconnect.
				// Meaning the server will no longer treat 
				// the remot as a client, but the remote will not know that.
			}
		}

		if (not RemoveClients())
		{
			// Failed to remove clients.
			return false;
		}

		return true;
	}

	bool Server::IsClient(const networking::SocketAddress& aAddress) const noexcept
	{
		return myClients.contains(aAddress);
	}

	size_t Server::GetNumClients() const noexcept
	{
		return myClients.size();
	}

	bool Server::SendTo(const shared::NetworkManager::NetworkMessage& aMessage, const networking::SocketAddress& aAddress)
	{
		return myNetworkManager.SendTo(aMessage, aAddress);
	}

	bool Server::SendToClients(const shared::NetworkManager::NetworkMessage& aMessage)
	{
		bool result = true;
		for (auto& entry : myClients)
		{
			result &= SendTo(aMessage, entry.second.Address);
		}
		return result;
	}
}