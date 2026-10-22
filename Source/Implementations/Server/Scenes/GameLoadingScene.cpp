#include "Server.pch.h"
#include "GameLoadingScene.h"

#include "../ServerApp.h"
#include "../Server.h"

#include <Core/imgui/imgui.h>

#include <Core/Engine/Engine.h>

namespace server
{
	GameLoadingScene::GameLoadingScene(ServerApp* aApp) :
		myApp(aApp),
		myGameScene(std::make_unique<GameScene>(aApp))
	{
	}
	GameLoadingScene::~GameLoadingScene()
	{
	}
	void GameLoadingScene::AddPlayer(std::string_view aName, shared::ClientID aClient)
	{
		myPlayers.push_back(
			_PlayerInfo{
				.Name = aName.data(),
				.Client = aClient,
				.FinishedLoading = false
			}
		);
	}
	void GameLoadingScene::OnInit()
	{
		GetServer().SetAllowingConnections(false);
		GetServer().SetMessageCallback<shared::NM_GameReady>(&OnGameReady, this);

		if (not myGameScene->InitializeGame(myPlayers.size()))
		{
			//TODO: Failed to initialize game.
			return;
		}

		shared::NM_StartGame startGame = {
			.NumPlayers = myPlayers.size()
		};

		shared::NetworkManager::NetworkMessage message;
		for (_PlayerInfo& player : myPlayers)
		{
			GetServer().CreateMessage(&message, startGame, true);

			GetServer().SendTo(message, GetServer().GetClientRemote(player.Client));
		}
	}
	void GameLoadingScene::OnShutdown()
	{
		GetServer().SetMessageCallback<shared::NM_GameReady>(nullptr, nullptr);
	}
	void GameLoadingScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		bool allReady = true;
		for (const _PlayerInfo& player : myPlayers)
		{
			allReady &= player.FinishedLoading;
		}

		const ImGuiWindowFlags flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		if (ImGui::Begin("Lobby", nullptr, flags))
		{
			if (allReady)
			{
				if (ImGui::Button("Start Game", { -1.f, 0.f }))
				{
					StartGame();
				}
			}
			else
			{
				if (ImGui::Button("Cancel Game", { -1.f, 0.f }))
				{
					//TODO: Add cancel game
				}
			}

			int id = 0;

			const float itemSpacing = 2.f * ImGui::GetStyle().ItemSpacing.x;

			for (const _PlayerInfo& player : myPlayers)
			{
				ImGui::PushID(id++);

				if (ImGui::BeginChild("##Player", { -1, 0.f }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders))
				{
					const float availWidth = ImGui::GetContentRegionAvail().x;

					if (ImGui::BeginChild("##Name", { availWidth * 0.3f - itemSpacing, 0.f }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle))
					{
						ImGui::Text(player.Name.c_str());
					}
					ImGui::EndChild();
					if (ImGui::BeginChild("##Status", { availWidth * 0.3f - itemSpacing, 0.f }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle))
					{
						ImGui::Text(player.FinishedLoading ? "Ready!" : "Loading...");
					}
					ImGui::EndChild();

					if (ImGui::Button("Kick", { availWidth * 0.3f - itemSpacing, 0.f }))
					{
						//TODO: Add kick 
					}
				}
				ImGui::EndChild();

				ImGui::PopID();
			}
		}
		ImGui::End();
	}
	void GameLoadingScene::OnGameReady(const Server::MessageCallbackData<shared::NM_GameReady>& aData)
	{
		GameLoadingScene& scene = *reinterpret_cast<GameLoadingScene*>(aData.UserData);
		Server& server = scene.GetServer();
		const shared::ClientID client = server.GetClientID(aData.Address);

		auto it = std::find_if(
			scene.myPlayers.begin(), 
			scene.myPlayers.end(), 
			[client](const _PlayerInfo& aPlayer) 
			{ 
				return aPlayer.Client == client; 
			}
		);

		if (it != scene.myPlayers.end())
		{
			_PlayerInfo& player = *it;
			if (not player.FinishedLoading)
			{
				player.FinishedLoading = true;

				const cardgame::Player* assignedPlayer = scene.myGameScene->AssignPlayer(player.Client);
				assignedPlayer;

				//shared::NM_AssignPlayer assignPlayer = {
				//	.PlayerID = assignedPlayer->GetID()
				//};
				//shared::NetworkManager::NetworkMessage message;
				//server.CreateMessage(&message, assignPlayer, true);
				//server.SendTo(message, aData.Address);
			}
		}
	}
	void GameLoadingScene::StartGame()
	{
		GameScene* gameScene = GetEngine().ChangeScene(std::move(myGameScene));

		// Assign names 
		for (const _PlayerInfo& info : myPlayers)
		{
			cardgame::SetNameCommand setName = {
				.Player = gameScene->GetPlayer(info.Client)->GetID(),
				.Name = {}
			};
			constexpr size_t maxNameLen = sizeof(setName.Name) - 1;
			const size_t end = info.Name.copy(setName.Name, maxNameLen, 0);
			setName.Name[end] = '\0';	// Might be reduntant if '.Name = {}' sets all 0.

			cardgame::Command command = {
				.Type = cardgame::COMMAND_SET_NAME,
				.Command = {
					.SetName = setName
				}
			};
			gameScene->PushCommand(command);
		}
	}
	Server& GameLoadingScene::GetServer() const noexcept
	{
		return myApp->GetServer();
	}
}
