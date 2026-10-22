#include "Server.pch.h"
#include "LobbyScene.h"
#include "GameLoadingScene.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/CommandParser.h>

#include <Core/imgui/imgui.h>

#include "../ServerApp.h"
#include "../Server.h"

#include "GameScene.h"

namespace server
{
	LobbyScene::LobbyScene(ServerApp* aApp) :
		myApp(aApp)
	{
	}
	LobbyScene::~LobbyScene()
	{
	}

	void LobbyScene::OnInit()
	{
		Server& server = GetServer();
		server.SetAllowingConnections(true);

		server.SetMessageCallback<shared::NM_NameRequest>(&OnNameRequest, this);
		server.SetConnectionCallback(&OnConnection, this);
		server.SetDisconnectionCallback(&OnDisconnection, this);
	}
	void LobbyScene::OnShutdown()
	{
		Server& server = GetServer();
		server.SetAllowingConnections(false);

		server.SetMessageCallback<shared::NM_NameRequest>(nullptr, nullptr);
		server.SetConnectionCallback(nullptr, nullptr);
		server.SetDisconnectionCallback(nullptr, nullptr);
	}
	void LobbyScene::OnEnter()
	{
	}
	void LobbyScene::OnExit()
	{
	}
	void LobbyScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		const ImGuiWindowFlags flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		if (ImGui::Begin("Lobby", nullptr, flags))
		{
			if (ImGui::Button("Start Game", { -1.f, 0.f }))
			{
				StartGame();
			}

			int id = 0;
			for (const PlayerInfo& player : myPlayers)
			{
				ImGui::PushID(id++);

				if (ImGui::BeginChild("##Player", { -1, 0.f }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders))
				{
					if (ImGui::BeginChild("##Name", { 0.f, 0.f }, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle))
					{
						ImGui::Text(player.Name.c_str());
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();

				ImGui::PopID();
			}
		}
		ImGui::End();
	}
	void LobbyScene::OnNameRequest(const Server::MessageCallbackData<shared::NM_NameRequest>& aData)
	{
		LobbyScene& scene = *reinterpret_cast<LobbyScene*>(aData.UserData);
		Server& server = scene.GetServer();
		const shared::ClientID client = server.GetClientID(aData.Address);

		auto it = std::find_if(scene.myPlayers.begin(), scene.myPlayers.end(), [client](const PlayerInfo& aPlayer) { return aPlayer.Client == client; });
		if (it != scene.myPlayers.end())
		{
			PlayerInfo& player = *it;
			player.Name = aData.Message.Name;
		}
	}
	void LobbyScene::OnConnection(const Server::ConnectionCallbackData& aData)
	{
		LobbyScene& scene = *reinterpret_cast<LobbyScene*>(aData.UserData);

		scene.AddPlayer(aData.Client);
	}
	void LobbyScene::OnDisconnection(const Server::DisconnectionCallbackData& aData)
	{
		LobbyScene& scene = *reinterpret_cast<LobbyScene*>(aData.UserData);

		scene.RemovePlayer(aData.Client);
	}
	Server& LobbyScene::GetServer() const noexcept
	{
		return myApp->GetServer();
	}
	void LobbyScene::AddPlayer(shared::ClientID aClient)
	{
		myPlayers.push_back(
			PlayerInfo{
				.Client = aClient,
				.Name = ""
			}
		);
	}
	void LobbyScene::RemovePlayer(shared::ClientID aClient)
	{
		std::erase_if(myPlayers, [aClient](const PlayerInfo& aPlayer) { return aPlayer.Client == aClient; });
	}
	void LobbyScene::StartGame()
	{
		GameLoadingScene* loadingScene = GetEngine().ChangeScene<GameLoadingScene>(myApp);

		const size_t numPlayers = myPlayers.size();

		for (size_t i = 0; i < numPlayers; i++)
		{
			PlayerInfo& player = myPlayers[i];
			loadingScene->AddPlayer(player.Name, player.Client);
		}
	}
}