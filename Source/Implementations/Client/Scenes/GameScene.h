#pragma once
#include <Core/Engine/Scene.h>
#include <Core/Engine/CommandParser.h>
#include <Implementations/CardGame/Game.h>
#include <Implementations/CardGame/Commands.h>
#include "../Client.h"

namespace client
{
	class ClientApp;

	class GameScene : public Scene
	{
	public:
		GameScene(ClientApp* aApp);
		~GameScene();

		void SetPlayerID(cardgame::PlayerID aID);
		cardgame::PlayerID GetPlayerID() const noexcept;

		bool InitializeGame(size_t aNumPlayers);

		Client& GetClient() const noexcept;

	protected:
		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		static void OnCmdGame(const CommandParser::CallbackData& aData);

		static void OnAssignPlayer(const Client::MessageCallbackData<shared::NM_AssignPlayer>& aData);
		static void OnCommand(const Client::MessageCallbackData<shared::NM_Command>& aData);
		void PushPendingCommands();

		std::unordered_map<size_t, cardgame::Command> myPendingCommands;

		cardgame::Game myGame;
		cardgame::PlayerID myPlayerID;
		ClientApp* myApp;
	};
}