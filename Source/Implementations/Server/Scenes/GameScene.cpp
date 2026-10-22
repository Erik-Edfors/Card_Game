#include "Server.pch.h"
#include "GameScene.h"
#include "../ServerApp.h"
#include "../Server.h"

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

#include <Core/Engine/Engine.h>
#include <Core/Engine/CommandParser.h>
#include <Core/Engine/Console.h>

#include <Core/CommonUtilities/ArgsParser.hpp>

#include <Implementations/Shared/Cards/Cards.h>

namespace server
{
	GameScene::GameScene(ServerApp* aServerApp) :
		myServer(aServerApp)
	{
	}
	GameScene::~GameScene()
	{
	}

	bool GameScene::InitializeGame(size_t aNumPlayers)
	{
		if (aNumPlayers == 0)
		{
			//TODO: Log, cannot start with 0 players.
			return false;
		}

		cardgame::CardFactory factory;
		shared::RegisterCards(factory);

		if (not myGame.Initialize(aNumPlayers, factory))
		{
			//TODO: handle failure
			return false;
		}

		return true;
	}

	const cardgame::Player* GameScene::AssignPlayer(shared::ClientID aClient)
	{
		const size_t numPlayers = myGame.GetNumPlayers();
		const cardgame::Player** players = new const cardgame::Player * [numPlayers];
		myGame.GetPlayers(players, numPlayers);

		for (size_t i = 0; i < numPlayers; i++)
		{
			const cardgame::Player* player = players[i];
			const cardgame::PlayerID id = player->GetID();
			if (not myPlayerToClient.contains(id))
			{
				myPlayerToClient[id] = aClient;
				myClientToPlayer[aClient] = id;

				delete[] players;
				return player;
			}
		}
		delete[] players;
		return nullptr;
	}

	const cardgame::Player* GameScene::GetPlayer(shared::ClientID aClient)
	{
		auto it = myClientToPlayer.find(aClient);
		return it != myClientToPlayer.end() ? myGame.GetPlayer(it->second) : nullptr;
	}

	bool GameScene::PushCommand(const cardgame::Command& aCommand)
	{
		const size_t index = myGame.GetNumCommands();
		if (myGame.PushCommand(aCommand))
		{
			SendCommand(aCommand, index);
			return true;
		}
		return false;
	}

	//void GameScene::AssignPlayer(networking::SocketAddress aRemote)
	//{
	//	myPlayerToRemote[aPlayer] = aRemote;
	//	myRemoteToPlayer[aRemote] = aPlayer;
	//}

	const cardgame::Game& GameScene::GetGame() const noexcept
	{
		return myGame;
	}

	Server& GameScene::GetServer() const noexcept
	{
		return myServer->GetServer();
	}

	void GameScene::OnInit()
	{
		Server& server = myServer->GetServer();
		server.SetAllowingConnections(false);

		server.SetMessageCallback<shared::NM_PlayCardRequest>(&OnPlayCardRequest, this);
		server.SetMessageCallback<shared::NM_EndTurnRequest>(&OnEndTurnRequest, this);

		GetEngine().GetCommandParser().AddCommand("game", &OnCmdGame, this);

		const size_t numPlayers = myGame.GetNumPlayers();
		const cardgame::Player** players = new const cardgame::Player * [numPlayers];
		myGame.GetPlayers(players, numPlayers);

		for (size_t i = 0; i < numPlayers; i++)
		{
			const cardgame::Player& player = *players[i];
			shared::ClientID client = myPlayerToClient.at(player.GetID());

			// Assign players to clients.
			{
				shared::NM_AssignPlayer assignPlayer = {
					.PlayerID = player.GetID()
				};
				shared::NetworkManager::NetworkMessage message;
				server.CreateMessage(&message, assignPlayer, true);
				server.SendTo(message, server.GetClientRemote(client));
			}

			for (size_t j = 0; j < 5; j++)
			{
				cardgame::Command command = {
					.Type = cardgame::COMMAND_CREATE_CARD,
					.Command = {
						.CreateCard = {
							.Owner = player.GetID(),
							.Location = player.GetHandLocation(),
							.FactoryID = "CORE_DAMAGE_CARD"_hash
						}
					}
				};

				if (not PushCommand(command))
				{
					//TODO: Log, failed to create card.
				}
			}

			for (size_t j = 0; j < 5; j++)
			{
				cardgame::Command command = {
					.Type = cardgame::COMMAND_CREATE_CARD,
					.Command = {
						.CreateCard = {
							.Owner = player.GetID(),
							.Location = player.GetHandLocation(),
							.FactoryID = "CORE_RESOURCE_CARD"_hash
						}
					}
				};

				if (not PushCommand(command))
				{
					//TODO: Log, failed to create card.
				}
			}
		}

		{
			cardgame::Command command = {
				.Type = cardgame::COMMAND_START,
				.Command = {
					.Start = {
					}
				}
			};
			if (not PushCommand(command))
			{
				//TODO: Log, failed to start game.
			}
		}

		delete[] players;
	}

	void GameScene::OnShutdown()
	{
		GetEngine().GetCommandParser().RemoveCommand("game");

		myGame.Shutdown();
	}
	void GameScene::OnEnter()
	{
		//server.SetMessageCallback<shared::NM_CommandRequest>(&OnCommandRequest, this);
	}
	void GameScene::OnExit()
	{
		Server& server = myServer->GetServer();

		//server.SetMessageCallback<shared::NM_CommandRequest>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_PlayCardRequest>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_EndTurnRequest>(nullptr, nullptr);
	}
	void GameScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		const ImGuiWindowFlags flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		if (ImGui::Begin("Game", nullptr, flags))
		{
			const cardgame::Player* turnPlayer = myGame.GetTurnPlayer();

			ImGui::Text(std::format("Number of players: {}", myGame.GetNumPlayers()).c_str());
			ImGui::Text(std::format("Turn: {}", myGame.GetTurnIndex()).c_str());
			if (turnPlayer != nullptr)
			{
				ImGui::Text(std::format("Turn Player: {}", myPlayerToClient.at(turnPlayer->GetID())).c_str());
			}
		}
		ImGui::End();
	}

	void GameScene::OnCmdGame(const CommandParser::CallbackData& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Console& console = scene.GetEngine().GetConsole();

		CommonUtilities::ArgsParser parser;
		parser.Parse(aData.Arguments, aData.NumArguments);

		size_t numArguments, numOptions, numFlags;
		const CommonUtilities::Argument* arguments;
		const CommonUtilities::Option* options;
		const CommonUtilities::Flag* flags;

		parser.GetArguments(&arguments, &numArguments);
		parser.GetOptions(&options, &numOptions);
		parser.GetFlags(&flags, &numFlags);

		if (numArguments >= 2)
		{
			switch (CommonUtilities::StringHash(arguments[1].GetValue()))
			{
			case "hash"_hash:
			{
				console.PrintLine(std::format("Hash: {:#0x}", scene.myGame.GetGamestateHash()));
			}
			break;
			case "stack"_hash:
			{
				size_t numCommands = scene.myGame.GetNumCommands();
				cardgame::Command* commands = new cardgame::Command[numCommands];
				scene.myGame.GetCommands(commands, 0, numCommands);
				for (size_t i = 0; i < numCommands; i++)
				{
					const cardgame::Command& command = commands[i];
					unsigned int hash = CommonUtilities::TStringHash<cardgame::Command>(&command, 1);

					console.PrintLine(std::format("[{}]: {:#0x}", command.Type, hash));
				}
				delete[] commands;
			}
			break;
			default:
				console.PrintLine(std::format("Unknown argument! {}", arguments[1].GetValue()));
				break;
			}
		}
	}

	void GameScene::OnPlayCardRequest(const Server::MessageCallbackData<shared::NM_PlayCardRequest>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		//TODO: Verify that action is allowed by client.
		// Make sure client has authority over the given player id.

		//TODO: Randomize card in draw pile to move to hand.
		cardgame::Command play = {
			.Type = cardgame::COMMAND_PLAY,
			.Command = {
				.Play = {
					.Player = aData.Message.Player,
					.Card = aData.Message.Card,
					.IgnoreCost = false
				}
			}
		};

		if (not scene.PushCommand(play))
		{
			//TODO: Handle failure.
		}
	}

	void GameScene::OnEndTurnRequest(const Server::MessageCallbackData<shared::NM_EndTurnRequest>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		cardgame::Game& game = scene.myGame;
		Server& server = scene.GetServer();

		const shared::ClientID client = server.GetClientID(aData.Address);

		//TODO: Verify that action is allowed by client.
		// Make sure client has authority over the current turn player.

		cardgame::Command endTurn = {
			.Type = cardgame::COMMAND_END_TURN,
			.Command = {
				.EndTurn = {
				}
			}
		};

		const cardgame::PlayerID issuer = scene.myClientToPlayer.at(client);
		const cardgame::Player* turnPlayer = game.GetTurnPlayer();

		// Only the player, whose turn it is, can end the turn.
		if (turnPlayer == nullptr || issuer != turnPlayer->GetID())
		{
			return;
		}

		if (not scene.PushCommand(endTurn))
		{
			//TODO: Handle failure.
		}
	}

	bool GameScene::SendCommand(const cardgame::Command& aCommand, size_t aIndex)
	{
		Server& server = myServer->GetServer();

		shared::NM_Command command = {
			.Command = aCommand,
			.Index = aIndex
		};

		shared::NetworkManager::NetworkMessage message;
		if (not server.CreateMessage(&message, command, true))
		{
			return false;
		}
		if (not server.SendToClients(message))
		{
			return false;
		}

		return true;
	}

	//void GameScene::OnCommandRequest(const Server::MessageCallbackData<shared::NM_CommandRequest>& aData)
	//{
	//	GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

	//	cardgame::Command commandReq = aData.Message.Command;

	//	cardgame::PlayerID remotePlayer;
	//	{
	//		auto it = scene.myRemoteToPlayer.find(aData.Address);
	//		if (it == scene.myRemoteToPlayer.end())
	//		{
	//			return;
	//		}

	//		remotePlayer = it->second;
	//	}

	//	if (remotePlayer != scene.myGame.GetGameState().TurnPlayer.ID)
	//	{
	//		return;
	//	}

	//	if (scene.myGame.PushCommand(commandReq))
	//	{
	//		bool succeeded = false;
	//		Server& server = scene.myServer->GetServer();

	//		shared::NM_Command command = {
	//			.Command = commandReq
	//		};

	//		shared::NetworkManager::NetworkMessage message;
	//		if (server.CreateMessage(&message, command, true))
	//		{
	//			if (server.SendToClients(message))
	//			{
	//				succeeded = true;
	//			}
	//		}

	//		if (not succeeded)
	//		{
	//			scene.myGame.PopCommand();
	//		}
	//	}
	//}
}
