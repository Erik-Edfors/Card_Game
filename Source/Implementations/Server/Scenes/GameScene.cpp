#include "Server.pch.h"
#include "GameScene.h"
#include "../ServerApp.h"
#include "../Server.h"
#include "MainScene.h"

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

#include <Core/Engine/Engine.h>
#include <Core/Engine/CommandParser.h>
#include <Core/Engine/Console.h>

#include <Core/CommonUtilities/ArgsParser.hpp>

#include <Implementations/Shared/Cards/Cards.h>

#include <Implementations/CardGame/Task.h>

#include <Core/CommonUtilities/Random.hpp>

#define FILE_CATEGORY "GameScene.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->GameScene"

namespace server
{
	GameScene::GameScene(ServerApp* aServerApp) :
		myApp(aServerApp)
	{
		cardgame::CardFactory factory;
		shared::RegisterCards(factory);

		if (not myGame.Initialize(factory))
		{
			//TODO: handle failure
		}
	}
	GameScene::~GameScene()
	{
	}

	const cardgame::Player* GameScene::AssignPlayer(shared::ClientID aClient, std::string_view aName)
	{
		cardgame::PlayerID id = cardgame::INVALID_PLAYER_ID;
		{
			cardgame::Command command = {
				.Type = cardgame::COMMAND_CREATE_PLAYER,
				.Command = {
					.CreatePlayer = {
					}
				}
			};
			cardgame::Command::Result result = {};
			if (not PushCommand(command, &result))
			{
				//TODO: Log.
			}
			id = result.CreatePlayer.Player;
		}

		{
			cardgame::SetNameCommand setName = {};
			setName.Player = id;
			aName.substr(0, sizeof(setName.Name) - 1).copy(setName.Name, sizeof(setName.Name), 0);

			cardgame::Command command = {
				.Type = cardgame::COMMAND_SET_NAME,
				.Command = {
					.SetName = setName
				}
			};
			if (not PushCommand(command))
			{
				//TODO: Log.
			}
		}
		myClientToPlayer[aClient] = id;
		myPlayerToClient[id] = aClient;

		const cardgame::Player* player = myGame.GetPlayer(id);
		return player;
	}

	const cardgame::Player* GameScene::GetPlayer(shared::ClientID aClient)
	{
		auto it = myClientToPlayer.find(aClient);
		return it != myClientToPlayer.end() ? myGame.GetPlayer(it->second) : nullptr;
	}

	bool GameScene::PushCommand(const cardgame::Command& aCommand, cardgame::Command::Result* outResult)
	{
		const size_t index = myGame.GetNumCommands();
		if (myGame.PushCommand(aCommand, outResult))
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
		return myApp->GetServer();
	}

	void GameScene::OnInit()
	{
		std::random_device rd = std::random_device();
		CommonUtilities::Seed(myRNG, std::ref(rd));

		myGameListener.SetPusher(&myGame.GetEventPusher());

		myGameListener.BeginListen(cardgame::Game::EVENT_TURN_BEGIN);
	}

	void GameScene::OnShutdown()
	{
		myGame.Shutdown();
	}
	void GameScene::OnEnter()
	{
		Server& server = myApp->GetServer();

		server.SetAllowingConnections(false);

		server.SetMessageCallback<shared::NM_PlayCardRequest>(&OnPlayCardRequest, this);
		server.SetMessageCallback<shared::NM_EndTurnRequest>(&OnEndTurnRequest, this);
		server.SetMessageCallback<shared::NM_SubmitPlayers>(&OnSubmitPlayers, this);
		server.SetMessageCallback<shared::NM_SubmitCards>(&OnSubmitCards, this);
		server.SetMessageCallback<shared::NM_SubmitLocations>(&OnSubmitLocations, this);
		server.SetMessageCallback<shared::NM_SubmitAmount>(&OnSubmitAmount, this);
		server.SetMessageCallback<shared::NM_SubmitOption>(&OnSubmitOption, this);

		server.SetDisconnectionCallback(OnDisconnect, this);

		GetEngine().GetCommandParser().AddCommand("game", &OnCmdGame, this);
	}
	void GameScene::OnExit()
	{
		Server& server = myApp->GetServer();

		//server.SetMessageCallback<shared::NM_CommandRequest>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_PlayCardRequest>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_EndTurnRequest>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_SubmitPlayers>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_SubmitCards>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_SubmitLocations>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_SubmitAmount>(nullptr, nullptr);
		server.SetMessageCallback<shared::NM_SubmitOption>(nullptr, nullptr);

		server.SetDisconnectionCallback(nullptr, nullptr);

		GetEngine().GetCommandParser().RemoveCommand("game");
	}
	void GameScene::OnStart()
	{
		Server& server = myApp->GetServer();

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

			AddCardsToDeck(player.GetID(), 10, "CORE_RESOURCE_CARD");
			AddCardsToDeck(player.GetID(), 10, "CORE_DAMAGE_CARD");
			AddCardsToDeck(player.GetID(), 4, "CORE_SHIELD_CARD");
			AddCardsToDeck(player.GetID(), 2, "CORE_DESTROY_CARD");
			AddCardsToDeck(player.GetID(), 2, "CORE_SACRIFICE_CARD");
		}

		{
			const cardgame::Command seedCommand = {
				.Type = cardgame::COMMAND_SET_SEED,
				.Command = {
					.SetSeed = {
						.Seed = std::random_device()()
					}
				}
			};

			const cardgame::Command startCommand = {
				.Type = cardgame::COMMAND_START,
				.Command = {
					.Start = {
					}
				}
			};

			if (not PushCommand(seedCommand))
			{
				//TODO: Log, failed to seed game.
			}
			if (not PushCommand(startCommand))
			{
				//TODO: Log, failed to start game.
			}
		}

		delete[] players;
	}
	void GameScene::OnUpdate(float aDeltaTime)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::OnUpdate"
		aDeltaTime;

		CommonUtilities::Event event;
		while (myGameListener.GetEvent(&event))
		{
			switch (event.ID)
			{
			case cardgame::Game::EVENT_TURN_BEGIN: OnTurnBegin(std::any_cast<cardgame::Game::EventTurnBegin>(event.Data)); break;
			default:
				LOG_WARNING(LOG_CATEGORY, "Recieved unprocessed event. Event '{}'.", event.ID);
				break;
			}
		}

		const ImGuiWindowFlags flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		if (ImGui::Begin("Game", nullptr, flags))
		{
			ImGui::SeparatorText("Game Controls");

			if (ImGui::Button("End Game", { -1, 0 }))
			{
				GetEngine().ChangeScene<MainScene>(myApp);
			}

			if (ImGui::BeginChild("##Players", { -1, -1 }, ImGuiChildFlags_Borders))
			{
				ImGui::SeparatorText("Players");

				size_t numPlayers = myGame.GetNumPlayers();
				const cardgame::Player** players = new const cardgame::Player * [numPlayers];
				numPlayers = myGame.GetPlayers(players, numPlayers);

				for (size_t i = 0; i < numPlayers; i++)
				{
					const cardgame::Player* player = players[i];

					shared::ClientID client = shared::INVALID_CLIENT_ID;

					auto it = myPlayerToClient.find(player->GetID());
					if (it != myPlayerToClient.end())
					{
						client = it->second;
					}

					ImGui::PushID(static_cast<int>(i));

					if (ImGui::BeginChild("##Player", { -1, 0 }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle))
					{
						// Cannot kick a non-client player
						ImGui::BeginDisabled(client == shared::INVALID_CLIENT_ID);
						if (ImGui::Button("Kick"))
						{
							GetServer().DisconnectClient(client, shared::NETWORK_DISCONNECT_KICKED_BY_OPERATOR);
						}
						ImGui::EndDisabled();

						ImGui::SameLine();
						const float nameWidth = ImGui::CalcTextSize("WWWWWWWWWWWWWWWW").x;
						if (ImGui::BeginChild("##Name", { nameWidth, 0.f }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle))
						{
							ImGui::Text(player->GetName().c_str());
						}
						ImGui::EndChild();

						ImGui::SameLine();
						if (ImGui::BeginChild("##Health", { 0.f, 0.f }, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle))
						{
							ImGui::Text(std::format("{} HP", player->GetHealth()).c_str());
						}
						ImGui::EndChild();

						ImGui::SameLine();
						if (ImGui::BeginChild("##Resources", { 0.f, 0.f }, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle))
						{
							ImGui::Text(std::format("{} RP", player->GetResourceCount()).c_str());
						}
						ImGui::EndChild();
					}
					ImGui::EndChild();

					ImGui::PopID();
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();

#undef LOG_CATEGORY
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
			case "damage"_hash:
			{
				if (numArguments >= 4)
				{
					cardgame::Health damage = 0;
					try
					{
						damage = std::stoi(arguments[3].GetValue());
					}
					catch (std::exception)
					{
						break;
					}

					size_t numPlayers = scene.myGame.GetNumPlayers();
					const cardgame::Player** players = new const cardgame::Player * [numPlayers];
					numPlayers = scene.myGame.GetPlayers(players, numPlayers);

					for (size_t i = 0; i < numPlayers; i++)
					{
						const cardgame::Player* player = players[i];
						if (player != nullptr && player->GetName() == arguments[2].GetValue())
						{
							scene.PushCommand(
								cardgame::Command{
									.Type = cardgame::COMMAND_DAMAGE_PLAYER,
										.Command = {
										.DamagePlayer = {
											.Source = cardgame::INVALID_CARD_ID,
											.Target = player->GetID(),
											.Amount = damage
										}
									}
								}
							);
						}
					}

					delete[] players;
				}
			}
			break;
			case "kill"_hash:
			{
				if (numArguments >= 3)
				{
					size_t numPlayers = scene.myGame.GetNumPlayers();
					const cardgame::Player** players = new const cardgame::Player * [numPlayers];
					numPlayers = scene.myGame.GetPlayers(players, numPlayers);

					for (size_t i = 0; i < numPlayers; i++)
					{
						const cardgame::Player* player = players[i];
						if (player != nullptr && player->GetName() == arguments[2].GetValue())
						{
							scene.PushCommand(
								cardgame::Command{
									.Type = cardgame::COMMAND_REMOVE_PLAYER,
										.Command = {
										.RemovePlayer = {
											.Player = player->GetID()
										}
									}
								}
							);
						}
					}

					delete[] players;
				}
			}
			break;
			default:
				console.PrintLine(std::format("Unknown argument! {}", arguments[1].GetValue()));
				break;
			}
		}
	}

	void GameScene::OnSubmitPlayers(const Server::MessageCallbackData<shared::NM_SubmitPlayers>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Server& server = scene.GetServer();

		// Sent invalid count.
		if (aData.Message.Count > cardgame::MAX_SELECTED_PLAYERS)
		{
			return;
		}

		auto it = scene.myClientToPlayer.find(server.GetClientID(aData.Address));
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}
		const cardgame::PlayerID player = it->second;

		cardgame::SubmitPlayersCommand submitPlayers = {};
		submitPlayers.Authority = player;
		submitPlayers.Count = aData.Message.Count;
		memcpy(submitPlayers.Players, aData.Message.Players, sizeof(submitPlayers.Players));

		const cardgame::Command command = {
			.Type = cardgame::COMMAND_SUBMIT_PLAYERS,
			.Command = {
				.SubmitPlayers = submitPlayers
			}
		};

		const bool succeeded = scene.PushCommand(command);
		const shared::NM_SubmissionResult result = {
			.Succeeded = succeeded
		};

		shared::NetworkManager::NetworkMessage message;
		if (server.CreateMessage(&message, result, true))
		{
			server.SendTo(message, aData.Address);
		}
	}

	void GameScene::OnSubmitCards(const Server::MessageCallbackData<shared::NM_SubmitCards>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Server& server = scene.GetServer();

		// Sent invalid count.
		if (aData.Message.Count > cardgame::MAX_SELECTED_CARDS)
		{
			return;
		}

		auto it = scene.myClientToPlayer.find(server.GetClientID(aData.Address));
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}
		const cardgame::PlayerID player = it->second;

		cardgame::SubmitCardsCommand submitCards = {};
		submitCards.Authority = player;
		submitCards.Count = aData.Message.Count;
		memcpy(submitCards.Cards, aData.Message.Cards, sizeof(submitCards.Cards));

		const cardgame::Command command = {
			.Type = cardgame::COMMAND_SUBMIT_CARDS,
			.Command = {
				.SubmitCards = submitCards
			}
		};

		const bool succeeded = scene.PushCommand(command);
		const shared::NM_SubmissionResult result = {
			.Succeeded = succeeded
		};

		shared::NetworkManager::NetworkMessage message;
		if (server.CreateMessage(&message, result, true))
		{
			server.SendTo(message, aData.Address);
		}
	}

	void GameScene::OnSubmitLocations(const Server::MessageCallbackData<shared::NM_SubmitLocations>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Server& server = scene.GetServer();

		// Sent invalid count.
		if (aData.Message.Count > cardgame::MAX_SELECTED_LOCATIONS)
		{
			return;
		}

		auto it = scene.myClientToPlayer.find(server.GetClientID(aData.Address));
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}
		const cardgame::PlayerID player = it->second;

		cardgame::SubmitLocationsCommand submitLocations = {};
		submitLocations.Authority = player;
		submitLocations.Count = aData.Message.Count;
		memcpy(submitLocations.Locations, aData.Message.Locations, sizeof(submitLocations.Locations));

		const cardgame::Command command = {
			.Type = cardgame::COMMAND_SUBMIT_LOCATIONS,
			.Command = {
				.SubmitLocations = submitLocations
			}
		};

		const bool succeeded = scene.PushCommand(command);
		const shared::NM_SubmissionResult result = {
			.Succeeded = succeeded
		};

		shared::NetworkManager::NetworkMessage message;
		if (server.CreateMessage(&message, result, true))
		{
			server.SendTo(message, aData.Address);
		}
	}

	void GameScene::OnSubmitAmount(const Server::MessageCallbackData<shared::NM_SubmitAmount>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Server& server = scene.GetServer();

		auto it = scene.myClientToPlayer.find(server.GetClientID(aData.Address));
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}
		const cardgame::PlayerID player = it->second;

		const cardgame::Command command = {
			.Type = cardgame::COMMAND_SUBMIT_AMOUNT,
			.Command = {
				.SubmitAmount = {
					.Authority = player,
					.Amount = aData.Message.Amount
				}
			}
		};

		const bool succeeded = scene.PushCommand(command);
		const shared::NM_SubmissionResult result = {
			.Succeeded = succeeded
		};

		shared::NetworkManager::NetworkMessage message;
		if (server.CreateMessage(&message, result, true))
		{
			server.SendTo(message, aData.Address);
		}
	}

	void GameScene::OnSubmitOption(const Server::MessageCallbackData<shared::NM_SubmitOption>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Server& server = scene.GetServer();

		auto it = scene.myClientToPlayer.find(server.GetClientID(aData.Address));
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}
		const cardgame::PlayerID player = it->second;

		const cardgame::Command command = {
			.Type = cardgame::COMMAND_SUBMIT_OPTION,
			.Command = {
				.SubmitOption = {
					.Authority = player,
					.Index = aData.Message.Index
				}
			}
		};

		const bool succeeded = scene.PushCommand(command);
		const shared::NM_SubmissionResult result = {
			.Succeeded = succeeded
		};

		shared::NetworkManager::NetworkMessage message;
		if (server.CreateMessage(&message, result, true))
		{
			server.SendTo(message, aData.Address);
		}
	}

	void GameScene::OnPlayCardRequest(const Server::MessageCallbackData<shared::NM_PlayCardRequest>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Server& server = scene.GetServer();
		cardgame::Game& game = scene.myGame;

		// All tasks must be finished before any cards can be played.
		if (game.HasTasks())
		{
			return;
		}

		const cardgame::Card* card = game.GetCard(aData.Message.Card);
		if (card == nullptr)
		{
			return;
		}

		auto it = scene.myClientToPlayer.find(server.GetClientID(aData.Address));
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}
		const cardgame::PlayerID player = it->second;

		// Ignore if player tries to play a card belonging to another player.
		// TODO: Allow if a card effect gives control over another player.
		if (player != card->GetOwner() || player != aData.Message.Player)
		{
			return;
		}

		const cardgame::Command play = {
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

		auto it = scene.myClientToPlayer.find(server.GetClientID(aData.Address));
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}
		const cardgame::PlayerID player = it->second;

		//TODO: Verify that action is allowed by client.
		// Make sure client has authority over the current turn player.

		// All tasks must be finished before any cards can be played.
		if (game.HasTasks())
		{
			return;
		}

		cardgame::Command endTurn = {
			.Type = cardgame::COMMAND_END_TURN,
			.Command = {
				.EndTurn = {
				}
			}
		};

		const cardgame::Player* turnPlayer = game.GetTurnPlayer();

		// Only the player, whose turn it is, can end the turn.
		if (turnPlayer == nullptr || player != turnPlayer->GetID())
		{
			return;
		}

		if (not scene.PushCommand(endTurn))
		{
			//TODO: Handle failure.
		}
	}

	void GameScene::OnDisconnect(const Server::DisconnectionCallbackData& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		auto it = scene.myClientToPlayer.find(aData.Client);
		if (it == scene.myClientToPlayer.end())
		{
			return;
		}

		const cardgame::PlayerID playerID = it->second;

		const cardgame::Command command = {
			.Type = cardgame::COMMAND_REMOVE_PLAYER,
			.Command = {
				.RemovePlayer = {
					.Player = playerID
				}
			}
		};

		scene.PushCommand(command);

		scene.myClientToPlayer.erase(aData.Client);
		scene.myPlayerToClient.erase(playerID);
	}

	bool GameScene::SendCommand(const cardgame::Command& aCommand, size_t aIndex)
	{
		Server& server = myApp->GetServer();

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

	void GameScene::OnTurnBegin(const cardgame::Game::EventTurnBegin& aData)
	{
		const cardgame::Player* player = myGame.GetPlayer(aData.Owner);
		if (player == nullptr)
		{
			//TODO: Log.
			return;
		}
		const cardgame::Location* deck = myGame.GetLocation(player->GetDeckLocation());

		for (size_t i = 0; i < 3; i++)
		{
			const int deckSize = static_cast<int>(deck->GetNumCards());

			if (deckSize == 0)
			{
				break;
			}

			const size_t index = std::uniform_int_distribution<int>(0, deckSize - 1)(myRNG);

			cardgame::CardID card;
			deck->GetCards(&card, index, 1);

			const cardgame::Command command = {
				.Type = cardgame::COMMAND_MOVE_CARD,
				.Command = {
					.MoveCard = {
						.Card = card,
						.Location = player->GetHandLocation()
					}
				}
			};

			PushCommand(command);
		}
	}

	bool GameScene::AddCardsToDeck(cardgame::PlayerID aPlayer, size_t aNumCards, CommonUtilities::StringHash aCardFactoryID)
	{
		const cardgame::Player* player = GetGame().GetPlayer(aPlayer);
		if (player == nullptr)
		{
			return false;
		}

		const cardgame::Command command = {
			.Type = cardgame::COMMAND_CREATE_CARD,
			.Command = {
				.CreateCard = {
					.Owner = player->GetID(),
					.Location = player->GetDeckLocation(),
					.FactoryID = aCardFactoryID
				}
			}
		};

		for (size_t i = 0; i < aNumCards; i++)
		{
			if (not PushCommand(command))
			{
				return false;
			}
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

#undef FILE_CATEGORY
#undef CLASS_CATEGORY