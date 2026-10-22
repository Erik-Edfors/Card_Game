#include "Client.pch.h"
#include "LobbyScene.h"
#include "GameScene.h"

#include "../ClientApp.h"

#include <Core/ComponentSystem/ComponentSystem.h>

#include <Core/Engine/Engine.h>
#include <Core/Engine/Components/TransformComponent.h>
#include <Core/Engine/Components/CameraComponent.h>
#include <Core/Engine/Components/CameraControllerComponent.h>
#include <Core/Engine/Components/SpriteComponent.h>
#include <Core/Engine/Components/TextComponent.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/Assets/TextureRegistry.h>
#include <Core/Assets/FontRegistry.h>

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

namespace client
{
	LobbyScene::LobbyScene(ClientApp* aApp) :
		myClient(aApp),
		myGameScene(std::make_unique<GameScene>(aApp))
	{
	}

	Client& LobbyScene::GetClient() const noexcept
	{
		return myClient->GetClient();
	}

	void LobbyScene::OnInit()
	{
		Client& client = myClient->GetClient();

		client.SetMessageCallback<shared::NM_StartGame>(&OnStartGame, this);

		cs::ComponentSystem& cs = GetComponentSystem();
		cs.RegisterComponentType<TransformComponent>();
		cs.RegisterComponentType<CameraComponent>();
		cs.RegisterComponentType<CameraControllerComponent>();
		cs.RegisterComponentType<SpriteComponent>();
		cs.RegisterComponentType<TextComponent>();

		assets::TextureRegistry& textureRegistry = myClient->GetTextureRegistry();
		assets::FontRegistry& fontRegistry = myClient->GetFontRegistry();

		SpriteRenderLayer* spriteRenderLayer = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRenderLayer = GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		{
			cs::EntityHandle cameraEntity = cs.CreateEntity();

			TransformComponent* transform = cs.AddComponent<TransformComponent>(cameraEntity);
			CameraComponent* camera = cs.AddComponent<CameraComponent>(cameraEntity);
			CameraControllerComponent* controller = cs.AddComponent<CameraControllerComponent>(cameraEntity, &GetEngine().GetInputManager());

			transform->SetTranslation({ 0.f, 0.f, -2.f });

			myCamera = camera;

			controller;
		}

		{
			cs::EntityHandle spriteHandle = cs.CreateEntity();

			TransformComponent* transform = cs.AddComponent<TransformComponent>(spriteHandle);
			transform->SetTranslation({ 0.f, 0.f, 0.f });
			transform->SetScale({ 0.625f, 1.f, 1.f });

			SpriteComponent* sprite = cs.AddComponent<SpriteComponent>(spriteHandle, spriteRenderLayer);
			sprite->SetAtlas(textureRegistry.Get("core:card_base")->ShaderResource.Get());
			sprite->SetOrigin({ 0.f, 1.f });
		}

		{
			cs::EntityHandle textHandle = cs.CreateEntity();
			TransformComponent* transform = cs.AddComponent<TransformComponent>(textHandle);
			transform->SetTranslation({ 0.05f, -0.1f, -0.01f });

			TextComponent* text = cs.AddComponent<TextComponent>(textHandle, textRenderLayer);
			text->SetFont(fontRegistry.Get("core:arial"));
			text->SetFontSize(0.1f);
			text->SetColor({ 0.f, 0.f, 0.f, 1.f });
			text->SetText("Hello World!");
		}
	}

	void LobbyScene::OnShutdown()
	{
		Client& client = myClient->GetClient();

		client.SetMessageCallback<shared::NM_StartGame>(nullptr, nullptr);
	}

	void LobbyScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

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
			if (ImGui::InputText("Name", &myNameInput))
			{
				if (myNameInput.length() >= 24)
				{
					myNameInput.resize(24);
				}

				shared::NM_NameRequest nameRequest{
					.Name = myNameInput
				};
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
		gameScene->InitializeGame(aData.Message.NumPlayers);
	}
}
