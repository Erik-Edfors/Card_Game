#pragma once
#include "../Server.h"
#include <Core/Engine/CommandParser.h>
#include <Core/Engine/Scene.h>
#include <Core/Networking/SocketAddress.h>
#include <Implementations/CardGame/Game.h>
#include <Implementations/Shared/Network/Messages.h>

namespace server
{
	class ServerApp;

	class GameScene : public Scene
	{
	public:
		GameScene(ServerApp* aServerApp);
		~GameScene();

		bool InitializeGame(size_t aNumPlayers);

		//cardgame::PlayerID AddPlayer(networking::SocketAddress aRemote);
		const cardgame::Player* AssignPlayer(shared::ClientID aClient);
		const cardgame::Player* GetPlayer(shared::ClientID aClient);

		bool PushCommand(const cardgame::Command& aCommand);

		const cardgame::Game& GetGame() const noexcept;
		Server& GetServer() const noexcept;

	protected:
		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		static void OnCmdGame(const CommandParser::CallbackData& aData);

		//static void OnCommandRequest(const Server::MessageCallbackData<shared::NM_CommandRequest>& aData);
		static void OnPlayCardRequest(const Server::MessageCallbackData<shared::NM_PlayCardRequest>& aData);
		static void OnEndTurnRequest(const Server::MessageCallbackData<shared::NM_EndTurnRequest>& aData);
		//static void OnGameReady(const Server::MessageCallbackData<shared::NM_GameReady>& aData);

		bool SendCommand(const cardgame::Command& aCommand, size_t aIndex);

		ServerApp* myServer;
		cardgame::Game myGame;
		std::unordered_map<cardgame::PlayerID, shared::ClientID> myPlayerToClient;
		std::unordered_map<shared::ClientID, cardgame::PlayerID> myClientToPlayer;

		//bool myGameStarted;
		//size_t myNumReadyClients;
	};
}