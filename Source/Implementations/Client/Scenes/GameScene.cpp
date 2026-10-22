#include "Client.pch.h"
#include "GameScene.h"
#include "../ClientApp.h"
#include "../Client.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/Console.h>

#include <Core/CommonUtilities/ArgsParser.hpp>

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

#include <Implementations/Shared/Cards/Cards.h>

#include "ServerSelectScene.h"

namespace client
{
	GameScene::GameScene(ClientApp* aApp) :
		myApp(aApp),
		myPlayerID(0)
	{
	}
	GameScene::~GameScene()
	{
	}

	void GameScene::SetPlayerID(cardgame::PlayerID aID)
	{
		myPlayerID = aID;
	}

	cardgame::PlayerID GameScene::GetPlayerID() const noexcept
	{
		return myPlayerID;
	}

	bool GameScene::InitializeGame(size_t aNumPlayers)
	{
		cardgame::CardFactory factory;
		shared::RegisterCards(factory);

		if (not myGame.Initialize(aNumPlayers, factory))
		{
			//TODO Log failure.
			return false;
		}

		return true;
	}

	Client& GameScene::GetClient() const noexcept
	{
		return myApp->GetClient();
	}

	void GameScene::OnInit()
	{
		Client& client = myApp->GetClient();

		client.SetMessageCallback<shared::NM_Command>(&OnCommand, this);
		client.SetMessageCallback<shared::NM_AssignPlayer>(&OnAssignPlayer, this);

		GetEngine().GetCommandParser().AddCommand("game", &OnCmdGame, this);

		shared::NM_GameReady gameReady;
		shared::NetworkManager::NetworkMessage message;
		client.CreateMessage(&message, gameReady, true);
		client.Send(message);
	}
	void GameScene::OnShutdown()
	{
		Client& client = myApp->GetClient();

		client.SetMessageCallback<shared::NM_Command>(nullptr, this);
		client.SetMessageCallback<shared::NM_AssignPlayer>(nullptr, this);
		GetEngine().GetCommandParser().RemoveCommand("game");
	}
	void GameScene::OnEnter()
	{

	}
	void GameScene::OnExit()
	{

	}
	void GameScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		Client& client = myApp->GetClient();
		client;

		if (client.GetConnectionStatus() != client::CLIENT_CONNECTION_CONNECTED)
		{
			GetEngine().ChangeScene<ServerSelectScene>(myApp);
			return;
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
			const cardgame::Player* turnPlayer = myGame.GetTurnPlayer();

			const size_t numPlayers = myGame.GetNumPlayers();
			const cardgame::Player** players = new const cardgame::Player * [numPlayers];
			myGame.GetPlayers(players, numPlayers);

			const float itemWidth = ImGui::GetContentRegionAvail().x / 3 - 2.f * ImGui::GetStyle().ItemSpacing.x;
			const float headerY = ImGui::GetCursorPosY();

			ImGui::SetCursorPos({ itemWidth * 0.f, headerY });
			ImGui::Text("Name");
			ImGui::SetCursorPos({ itemWidth * 1.f, headerY });
			ImGui::Text("Health");
			ImGui::SetCursorPos({ itemWidth * 2.f, headerY });
			ImGui::Text("Resources");

			for (size_t i = 0; i < numPlayers; i++)
			{
				const cardgame::Player& player = *players[i];
				ImGui::PushID(&player);

				const bool highlighted = turnPlayer != nullptr && turnPlayer->GetID() == player.GetID();

				if (highlighted)
				{
					ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(25, 255, 25, 255));
				}
				const bool childOpen = ImGui::BeginChild("##Player", { -1.f, 0.f }, ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY);
				if (highlighted)
				{
					ImGui::PopStyleColor();
				}

				if (childOpen)
				{
					const float cursorPosY = ImGui::GetCursorPosY();

					ImGui::SetCursorPos({ itemWidth * 0.f, cursorPosY });
					ImGui::Text(player.GetName().c_str());
					ImGui::SetCursorPos({ itemWidth * 1.f, cursorPosY });
					ImGui::Text("%i", player.GetHealth());
					ImGui::SetCursorPos({ itemWidth * 2.f, cursorPosY });
					ImGui::Text("%i", player.GetResourceCount());
				}
				ImGui::EndChild();
				ImGui::PopID();

			}

			delete[] players;

			ImGui::Separator();

			if (turnPlayer != nullptr)
			{
				ImGui::Text(std::format("Turn Player: {}", myGame.GetTurnPlayer()->GetID()).c_str());

				if (turnPlayer->GetID() == myPlayerID)
				{
					const cardgame::Location& hand = *myGame.GetLocation(turnPlayer->GetHandLocation());

					if (ImGui::Button("End Turn"))
					{
						shared::NM_EndTurnRequest endTurn = {
						};
						shared::NetworkManager::NetworkMessage message;
						client.CreateMessage(&message, endTurn, true);
						client.Send(message);
					}

					for (cardgame::CardID cardID : hand)
					{
						ImGui::PushID(static_cast<int>(cardID));

						const cardgame::Card& card = *myGame.GetCard(cardID);

						if (ImGui::Button(card.GetName()))
						{
							shared::NM_PlayCardRequest playCard = {
								.Player = GetPlayerID(),
								.Card = cardID
							};
							shared::NetworkManager::NetworkMessage message;
							client.CreateMessage(&message, playCard, true);
							client.Send(message);
						}
						if (ImGui::BeginItemTooltip())
						{
							ImGui::Text(card.GetDescription());
							ImGui::EndTooltip();
						}

						ImGui::PopID();
					}
				}
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

	void GameScene::OnAssignPlayer(const Client::MessageCallbackData<shared::NM_AssignPlayer>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		scene.myPlayerID = aData.Message.PlayerID;
	}
	void GameScene::OnCommand(const Client::MessageCallbackData<shared::NM_Command>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		const cardgame::Command& command = aData.Message.Command;
		const size_t index = aData.Message.Index;
		const size_t expectedIndex = scene.myGame.GetNumCommands();

		scene.GetEngine().GetConsole().PrintLine(std::format("Recieved Command: {}.", command.Type));

		// If a command is recieved out of order.
		// It is saved and later pushed when all previous command have been recieved and pushed.
		if (index != expectedIndex)
		{
			if (index < expectedIndex)
			{
				//TODO: Log error!
			}
			else
			{
				scene.myPendingCommands[index] = command;
			}
		}
		else
		{
			if (not scene.myGame.PushCommand(command))
			{
				//TODO: Handle invalid command.
			}
			scene.PushPendingCommands();
		}

	}
	void GameScene::PushPendingCommands()
	{
		size_t expectedIndex = myGame.GetNumCommands();

		auto it = myPendingCommands.find(expectedIndex);
		while (it != myPendingCommands.end())
		{
			if (not myGame.PushCommand(it->second))
			{
				//TODO: Log failure to push command.
			}

			myPendingCommands.erase(it);
			expectedIndex = myGame.GetNumCommands();
			it = myPendingCommands.find(expectedIndex);
		}
	}
}