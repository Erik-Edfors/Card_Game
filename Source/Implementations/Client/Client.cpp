#include "Client.pch.h"
#include "Client.h"

namespace client
{
	Client::Client() :
		myConnectionStatus(CLIENT_CONNECTION_DISCONNECTED),
		myConnectionAttempts(0),
		myServer({ }),
		myConnectionStart({}),
		myLastHeardFromRemote({})
	{
	}

	Client::~Client()
	{
	}

	bool Client::Initialize(networking::Port aPort)
	{
		if (not myNetworkManager.Initialize(4096, aPort))
		{
			return false;
		}

		myNetworkManager.SetCallback<shared::NM_ServerInfo>(&OnServerInfo, this);
		myNetworkManager.SetCallback<shared::NM_ConnectionResult>(&OnConnectionResult, this);
		myNetworkManager.SetCallback<shared::NM_Disconnect>(&OnDisconnect, this);

		return true;
	}
	bool Client::Shutdown()
	{
		bool result = true;

		Disconnect(shared::NETWORK_DISCONNECT_END_OF_SESSION);

		myCallbacks.clear();

		result &= myNetworkManager.Shutdown();

		return result;
	}
	void Client::Update()
	{
		myNetworkManager.Update(64);

		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		// Cancelling timed out contracts.
		{
			auto it = myServerInfoContracts.begin();
			while (it != myServerInfoContracts.end())
			{
				ServerInfoContract& infoContract = *it;

				if (std::chrono::duration<float>(now - infoContract.TimeOfIssue).count() > infoContract.Timeout)
				{
					infoContract.Task.Cancel();
					it = myServerInfoContracts.erase(it);
				}
				else
				{
					it = std::next(it);
				}
			}
		}

		switch (GetConnectionStatus())
		{
		case CLIENT_CONNECTION_PENDING:
		{
			const float timeSinceConnectionRequest = std::chrono::duration<float>(now - myConnectionStart).count();

			if (timeSinceConnectionRequest > 30.f)
			{
				Disconnect(shared::NETWORK_DISCONNECT_TIMED_OUT);
			}
			else if (static_cast<float>(myConnectionAttempts) * 0.5f < timeSinceConnectionRequest) // TODO: Allow setting time between connection attempts.
			{
				SendConnectionRequest(myServer);
				++myConnectionAttempts;
			}
		}
		break;
		case CLIENT_CONNECTION_CONNECTED:
		{
			const float timeSinceLastHeardFromRemote = std::chrono::duration<float>(now - myLastHeardFromRemote).count();

			if (timeSinceLastHeardFromRemote > 30.f) // TODO: Allow setting timeout.
			{
				Disconnect(shared::NETWORK_DISCONNECT_TIMED_OUT);
			}
		}
		break;
		}
	}

	CommonUtilities::Contract<Client::ServerInfo> Client::RequestServerInfo(const networking::SocketAddress& aServer, float aTimeout)
	{
		CommonUtilities::Contract<Client::ServerInfo> contract;
		CommonUtilities::Task<Client::ServerInfo> task = contract.GetTask();

		shared::NM_ServerInfoRequest request = {

		};

		shared::NetworkManager::NetworkMessage message;
		if (not CreateMessage(&message, request))
		{
			task.Cancel();
			return contract;
		}

		if (not SendTo(message, aServer))
		{
			task.Cancel();
			return contract;
		}

		myServerInfoContracts.push_back(
			ServerInfoContract{
				.Remote = aServer,
				.Task = std::move(task),
				.TimeOfIssue = std::chrono::steady_clock::now(),
				.Timeout = aTimeout
			}
		);

		return contract;
	}

	bool Client::RequestConnection(const networking::SocketAddress& aServer)
	{
		if (GetConnectionStatus() != CLIENT_CONNECTION_DISCONNECTED)
		{
			Disconnect(shared::NETWORK_DISCONNECT_END_OF_SESSION);
		}

		if (not SendConnectionRequest(aServer))
		{
			return false;
		}

		++myConnectionAttempts;
		myConnectionStart = std::chrono::steady_clock::now();
		myConnectionStatus = CLIENT_CONNECTION_PENDING;
		myServer = aServer;

		return true;
	}

	bool Client::Disconnect(shared::NETWORK_DISCONNECT_REASON aReason)
	{
		if (GetConnectionStatus() == CLIENT_CONNECTION_DISCONNECTED)
		{
			// Not connected to a remote.
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
			if (not myNetworkManager.SendTo(message, myServer))
			{
				//TODO: Log failed to send disconnect.
				// This causes a dirty disconnect.
				// Meaning the server will no longer treat 
				// the remot as a client, but the remote will not know that.
			}
		}

		myConnectionAttempts = 0;
		myConnectionStart = {};
		myServer = {};
		myConnectionStatus = CLIENT_CONNECTION_DISCONNECTED;

		return true;
	}

	const networking::SocketAddress& Client::GetRemote() const noexcept
	{
		return myServer;
	}

	CLIENT_CONNECTION_STATUS Client::GetConnectionStatus() const noexcept
	{
		return myConnectionStatus;
	}

	bool Client::IsConnectedTo(const networking::SocketAddress& aServer) const noexcept
	{
		return GetConnectionStatus() == CLIENT_CONNECTION_CONNECTED && myServer == aServer;
	}

	bool Client::SendTo(const shared::NetworkManager::NetworkMessage& aMessage, const networking::SocketAddress& aAddress)
	{
		return myNetworkManager.SendTo(aMessage, aAddress);
	}

	bool Client::Send(const shared::NetworkManager::NetworkMessage& aMessage)
	{
		if (GetConnectionStatus() != CLIENT_CONNECTION_CONNECTED)
		{
			return false;
		}
		return myNetworkManager.SendTo(aMessage, myServer);
	}

	void Client::OnServerInfo(const shared::NetworkManager::MessageCallbackData<shared::NM_ServerInfo>& aData)
	{
		Client& client = *reinterpret_cast<Client*>(aData.UserData);

		auto it = client.myServerInfoContracts.begin();
		while (it != client.myServerInfoContracts.end())
		{
			ServerInfoContract& infoContract = *it;

			if (infoContract.Remote == aData.Address)
			{
				infoContract.Task.Finish(
					ServerInfo{
						.Name = aData.Message.Name,
						.Description = aData.Message.Description
					}
				);

				it = client.myServerInfoContracts.erase(it);
			}
			else
			{
				it = std::next(it);
			}
		}
	}

	void Client::OnConnectionResult(const shared::NetworkManager::MessageCallbackData<shared::NM_ConnectionResult>& aData)
	{
		Client& client = *reinterpret_cast<Client*>(aData.UserData);

		if (client.myServer != aData.Address)
		{
			// Wrong remote sent a connection result.
			return;
		}

		if (client.GetConnectionStatus() != CLIENT_CONNECTION_PENDING)
		{
			// The client is not trying to connect to a remote at this time.
			return;
		}

		if (not aData.Message.Accepted)
		{
			// Connection declined.
			return;
		}

		// Connection accepted.
		client.myConnectionStatus = CLIENT_CONNECTION_CONNECTED;
		client.myLastHeardFromRemote = std::chrono::steady_clock::now();
	}

	void Client::OnDisconnect(const shared::NetworkManager::MessageCallbackData<shared::NM_Disconnect>& aData)
	{
		Client& client = *reinterpret_cast<Client*>(aData.UserData);

		if (client.myServer != aData.Address)
		{
			// Wrong remote sent disconnect.
			return;
		}

		if (client.GetConnectionStatus() != CLIENT_CONNECTION_CONNECTED)
		{
			// The client is not connected to a remote that can be disconnected.
			return;
		}

		// Connection accepted.
		client.myConnectionStatus = CLIENT_CONNECTION_DISCONNECTED;
		client.myServer = {};
	}
	bool Client::SendConnectionRequest(const networking::SocketAddress& aRemote)
	{
		shared::NM_ConnectionRequest connectionRequest = {

		};
		shared::NetworkManager::NetworkMessage message;
		if (not myNetworkManager.CreateMessage(&message, connectionRequest))
		{
			// TODO: Log failed to create message.
			return false;
		}

		if (not myNetworkManager.SendTo(message, aRemote))
		{
			//TODO: Log failed to send disconnect.
			return false;
		}

		return true;
	}
}