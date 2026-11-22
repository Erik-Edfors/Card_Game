#pragma once
#include "GameScene.h"

#include <Core/Engine/Scene.h>
#include <Core/Networking/SocketAddress.h>

namespace server
{
	class ServerApp;
	class Server;

	class GameLoadingScene : public Scene
	{
	public:
		GameLoadingScene(ServerApp* aApp);
		virtual ~GameLoadingScene();

		void AddPlayer(std::string_view aName, shared::ClientID aClient);

	protected:
		virtual void OnEnter() override;
		virtual void OnExit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		struct _PlayerInfo
		{
			std::string Name;
			shared::ClientID Client;
			bool FinishedLoading;
		};

		static void OnGameReady(const Server::MessageCallbackData<shared::NM_GameReady>& aData);

		void StartGame();

		float myStartTimer;

		std::unique_ptr<GameScene> myGameScene;
		Server& GetServer() const noexcept;

		ServerApp* myApp;

		std::vector<_PlayerInfo> myPlayers;
	};
}