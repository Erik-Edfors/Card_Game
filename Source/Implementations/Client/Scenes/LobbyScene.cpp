#include "Client.pch.h"
#include "LobbyScene.h"
#include "GameScene.h"
#include "ClientApp.h"
#include "Scenes/ServerSelectScene.h"


#include <Core/ComponentSystem/ComponentSystem.h>

#include <Core/Engine/Engine.h>
#include <Core/Engine/Components/TransformComponent.h>
#include <Core/Engine/Components/CameraComponent.h>
#include <Core/Engine/Components/CameraControllerComponent.h>
#include <Core/Engine/Components/SpriteComponent.h>
#include <Core/Engine/Components/TextComponent.h>

//TODO: Remove
#include <Implementations/Client/Components/CardComponent.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/Assets/TextureRegistry.h>
#include <Core/Assets/FontRegistry.h>

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

namespace client
{
	LobbyScene::LobbyScene(ClientApp* aApp) :
		myApp(aApp),
		myGameScene(std::make_unique<GameScene>(aApp))
	{
	}

	Client& LobbyScene::GetClient() const noexcept
	{
		return myApp->GetClient();
	}

	void LobbyScene::OnInit()
	{
		Client& client = myApp->GetClient();

		client.SetMessageCallback<shared::NM_StartGame>(&OnStartGame, this);

		cs::ComponentSystem& cs = GetComponentSystem();
		cs.RegisterComponentType<TransformComponent>();
		cs.RegisterComponentType<CameraComponent>();
		cs.RegisterComponentType<CameraControllerComponent>();
		cs.RegisterComponentType<SpriteComponent>();
		cs.RegisterComponentType<TextComponent>();
		//TODO: Remove
		cs.RegisterComponentType<CardComponent>();

		//assets::TextureRegistry& textureRegistry = myClient->GetTextureRegistry();
		//assets::FontRegistry& fontRegistry = myClient->GetFontRegistry();

		//SpriteRenderLayer* spriteRenderLayer = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		//TextRenderLayer* textRenderLayer = GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		{
			cs::EntityHandle cameraEntity = cs.CreateEntity();

			TransformComponent* transform = cs.AddComponent<TransformComponent>(cameraEntity);
			CameraComponent* camera = cs.AddComponent<CameraComponent>(cameraEntity);
			CameraControllerComponent* controller = cs.AddComponent<CameraControllerComponent>(cameraEntity, &GetEngine().GetInputManager());

			transform->SetTranslation({ 0.f, 0.f, -2.f });

			myCamera = camera;

			controller;
		}

		//{
		//	cs::EntityHandle spriteHandle = cs.CreateEntity();

		//	TransformComponent* transform = cs.AddComponent<TransformComponent>(spriteHandle);
		//	transform->SetTranslation({ 0.f, 0.f, 0.5f });
		//	transform->SetScale({ 0.625f, 1.f, 1.f });

		//	SpriteComponent* sprite = cs.AddComponent<SpriteComponent>(spriteHandle, spriteRenderLayer);
		//	sprite->SetAtlas(textureRegistry.Get("core:card_base")->ShaderResource.Get());
		//	sprite->SetOrigin({ 0.f, 1.f });

		//	//TODO: Remove
		//	CardComponent* card = cs.AddComponent<CardComponent>(spriteHandle, nullptr, 0);
		//	card;
		//}

		//{
		//	cs::EntityHandle textHandle = cs.CreateEntity();
		//	TransformComponent* transform = cs.AddComponent<TransformComponent>(textHandle);
		//	transform->SetTranslation({ 0.05f, -0.1f, -0.01f });

		//	TextComponent* text = cs.AddComponent<TextComponent>(textHandle, textRenderLayer);
		//	text->SetFont(fontRegistry.Get("core:arial"));
		//	text->SetFontSize(0.1f);
		//	text->SetColor({ 0.f, 0.f, 0.f, 1.f });
		//	text->SetText("Hello World!");
		//}
	}

	void LobbyScene::OnShutdown()
	{
		Client& client = myApp->GetClient();

		client.SetMessageCallback<shared::NM_StartGame>(nullptr, nullptr);
	}

	void LobbyScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		if (GetClient().GetConnectionStatus() != CLIENT_CONNECTION_CONNECTED)
		{
			GetEngine().ChangeScene<ServerSelectScene>(myApp);
			return;
		}

		const CommonUtilities::Matrix4x4f view = myCamera->GetView();
		const CommonUtilities::Matrix4x4f projection = myCamera->GetProjection();

		SpriteRenderLayer* spriteRL = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRL = GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		spriteRL->SetView(view);
		spriteRL->SetProjection(projection);

		textRL->SetView(view);
		textRL->SetProjection(projection);

		const ImGuiWindowFlags flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		if (ImGui::Begin("Lobby", nullptr, flags))
		{
			ImGui::SeparatorText("Lobby");

			if (ImGui::InputText("Name", &myNameInput))
			{
				if (myNameInput.length() >= cardgame::MAX_PLAYER_NAME_SIZE - 1)
				{
					myNameInput.resize(cardgame::MAX_PLAYER_NAME_SIZE - 1);
				}

				shared::NM_NameRequest nameRequest{
					.Name = {}
				};
				myNameInput.copy(nameRequest.Name, myNameInput.size());
				nameRequest.Name[myNameInput.size()] = '\0';

				shared::NetworkManager::NetworkMessage message;
				GetClient().CreateMessage(&message, nameRequest, true);
				GetClient().Send(message);
			}
		}
		ImGui::End();
	}

	//void LobbyScene::OnAssignPlayer(const Client::MessageCallbackData<shared::NM_AssignPlayer>& aData)
	//{
	//	LobbyScene& scene = *reinterpret_cast<LobbyScene*>(aData.UserData);

	//	if (scene.GetClient().GetID() == aData.Message.Client)
	//	{
	//		scene.myGameScene->SetPlayerID(aData.Message.PlayerID);
	//	}
	//}

	void LobbyScene::OnStartGame(const Client::MessageCallbackData<shared::NM_StartGame>& aData)
	{
		LobbyScene& scene = *reinterpret_cast<LobbyScene*>(aData.UserData);

		GameScene* gameScene = scene.GetEngine().PushScene<GameScene>(std::move(scene.myGameScene));
		gameScene;
	}
}
