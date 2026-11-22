#pragma once
#include "../Server.h"
#include <Core/Engine/CommandParser.h>
#include <Core/Engine/Scene.h>
#include <Core/Networking/SocketAddress.h>
#include <Implementations/CardGame/Game.h>
#include <Implementations/Shared/Network/Messages.h>
#include <random>

namespace server
{
	class ServerApp;

	class GameScene : public Scene
	{
	public:
		GameScene(ServerApp* aServerApp);
		~GameScene();

		//cardgame::PlayerID AddPlayer(networking::SocketAddress aRemote);
		const cardgame::Player* AssignPlayer(shared::ClientID aClient, std::string_view aName);
		const cardgame::Player* GetPlayer(shared::ClientID aClient);

		bool PushCommand(const cardgame::Command& aCommand, cardgame::Command::Result* outResult = nullptr);

		const cardgame::Game& GetGame() const noexcept;
		Server& GetServer() const noexcept;

	protected:
		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;
		virtual void OnStart() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		static void OnCmdGame(const CommandParser::CallbackData& aData);

		//static void OnCommandRequest(const Server::MessageCallbackData<shared::NM_CommandRequest>& aData);
		static void OnSubmitPlayers(const Server::MessageCallbackData<shared::NM_SubmitPlayers>& aData);
		static void OnSubmitCards(const Server::MessageCallbackData<shared::NM_SubmitCards>& aData);
		static void OnSubmitLocations(const Server::MessageCallbackData<shared::NM_SubmitLocations>& aData);
		static void OnSubmitAmount(const Server::MessageCallbackData<shared::NM_SubmitAmount>& aData);
		static void OnSubmitOption(const Server::MessageCallbackData<shared::NM_SubmitOption>& aData);

		static void OnPlayCardRequest(const Server::MessageCallbackData<shared::NM_PlayCardRequest>& aData);
		static void OnEndTurnRequest(const Server::MessageCallbackData<shared::NM_EndTurnRequest>& aData);
		//static void OnGameReady(const Server::MessageCallbackData<shared::NM_GameReady>& aData);

		static void OnDisconnect(const Server::DisconnectionCallbackData& aData);

		bool SendCommand(const cardgame::Command& aCommand, size_t aIndex);

		void OnTurnBegin(const cardgame::Game::EventTurnBegin& aData);

		bool AddCardsToDeck(cardgame::PlayerID aPlayer, size_t aNumCards, CommonUtilities::StringHash aCardFactoryID);

		CommonUtilities::EventListener myGameListener;

		ServerApp* myApp;
		cardgame::Game myGame;
		std::unordered_map<cardgame::PlayerID, shared::ClientID> myPlayerToClient;
		std::unordered_map<shared::ClientID, cardgame::PlayerID> myClientToPlayer;

		std::mt19937 myRNG;

		//bool myGameStarted;
		//size_t myNumReadyClients;
	};
}