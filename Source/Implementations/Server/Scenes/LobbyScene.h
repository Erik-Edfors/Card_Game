#pragma once
#include "../Server.h"
#include <Core/Networking/SocketAddress.h>
#include <Core/Engine/Scene.h>
#include <string>
#include <map>

namespace server
{
	class ServerApp;

	class LobbyScene : public Scene
	{
	public:
		LobbyScene(ServerApp* aApp);
		~LobbyScene();

		void SetMaxPlayers(size_t aMax) noexcept;
		size_t GetMaxPlayers() const noexcept;

		size_t GetNumPlayers() const noexcept;

	protected:
		virtual void OnEnter() override;
		virtual void OnExit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		static void OnNameRequest(const Server::MessageCallbackData<shared::NM_NameRequest>& aData);
		static void OnConnection(const Server::ConnectionCallbackData& aData);
		static void OnDisconnection(const Server::DisconnectionCallbackData& aData);

		Server& GetServer() const noexcept;

		void AddPlayer(shared::ClientID aClient);
		void RemovePlayer(shared::ClientID aClient);

		void StartGame();

		struct PlayerInfo
		{
			shared::ClientID Client;
			std::string Name;
		};

		ServerApp* myApp;

		std::vector<PlayerInfo> myPlayers;
		size_t myMaxPlayers;
	};
}