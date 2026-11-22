#include "Server.pch.h"
#include "GameLoadingScene.h"

#include "ServerApp.h"
#include "Server.h"
#include "Scenes/MainScene.h"

#include <Core/imgui/imgui.h>

#include <Core/Engine/Engine.h>

namespace server
{
	GameLoadingScene::GameLoadingScene(ServerApp* aApp) :
		myApp(aApp),
		myGameScene(std::make_unique<GameScene>(aApp)),
		myStartTimer(0.f)
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
	void GameLoadingScene::OnEnter()
	{
		GetServer().SetAllowingConnections(false);
		GetServer().SetMessageCallback<shared::NM_GameReady>(&OnGameReady, this);

		shared::NM_StartGame startGame = {
		};

		shared::NetworkManager::NetworkMessage message;
		for (_PlayerInfo& player : myPlayers)
		{
			GetServer().CreateMessage(&message, startGame, true);

			GetServer().SendTo(message, GetServer().GetClientRemote(player.Client));
		}
	}
	void GameLoadingScene::OnExit()
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

		if (allReady)
		{
			myStartTimer += aDeltaTime;
			if (myStartTimer > 2.f)
			{
				StartGame();
			}
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
			ImGui::SeparatorText("Waiting for Players to be Ready");
			if (ImGui::Button("Cancel Game", { -1.f, 0.f }))
			{
				GetServer().DisconnectClients(shared::NETWORK_DISCONNECT_END_OF_SESSION);
				GetEngine().ChangeScene<MainScene>(myApp);
			}

			int id = 0;

			const float itemSpacing = 2.f * ImGui::GetStyle().ItemSpacing.x;

			if (ImGui::BeginChild("##Players", { -1, -1 }, ImGuiChildFlags_Borders))
			{
				ImGui::SeparatorText("Players");

				auto it = myPlayers.begin();
				while (it != myPlayers.end())
				{
					_PlayerInfo& player = *it;

					ImGui::PushID(id++);

					bool erase = false;

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
							erase = true;
							GetServer().DisconnectClient(player.Client, shared::NETWORK_DISCONNECT_KICKED_BY_OPERATOR);
						}
					}
					ImGui::EndChild();

					ImGui::PopID();

					if (erase)
					{
						it = myPlayers.erase(it);
					}
					else
					{
						it = std::next(it);
					}
				}
			}
			ImGui::EndChild();
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
			const cardgame::Player* assignedPlayer = gameScene->AssignPlayer(info.Client, info.Name);
			assignedPlayer;
		}
	}
	Server& GameLoadingScene::GetServer() const noexcept
	{
		return myApp->GetServer();
	}
}
