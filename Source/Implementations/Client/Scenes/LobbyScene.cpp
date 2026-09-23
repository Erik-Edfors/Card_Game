#include "Client.pch.h"
#include "LobbyScene.h"

#include "../Client.h"

#include <Core/ComponentSystem/ComponentSystem.h>

#include <Core/Engine/Engine.h>
#include <Core/Engine/Components/TransformComponent.h>
#include <Core/Engine/Components/CameraComponent.h>
#include <Core/Engine/Components/CameraControllerComponent.h>
#include <Core/Engine/Components/SpriteComponent.h>
#include <Core/Engine/Components/TextComponent.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/Assets/TextureRegistry.h>

namespace client
{
	LobbyScene::LobbyScene(Client* aClient) : 
		myClient(aClient)
	{
	}

	void LobbyScene::OnInit()
	{
		cs::ComponentSystem& cs = GetComponentSystem();
		cs.RegisterComponentType<TransformComponent>();
		cs.RegisterComponentType<CameraComponent>();
		cs.RegisterComponentType<CameraControllerComponent>();
		cs.RegisterComponentType<SpriteComponent>();
		cs.RegisterComponentType<TextComponent>();

		assets::TextureRegistry& textureRegistry = myClient->GetTextureRegistry();

		SpriteRenderLayer* spriteRenderLayer = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();

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
			transform->SetTranslation({ 0.f, 0.f, 0.01f });
			transform->SetScale({ 0.625f, 1.f, 1.f });

			SpriteComponent* sprite = cs.AddComponent<SpriteComponent>(spriteHandle, spriteRenderLayer);
			sprite->SetAtlas(textureRegistry.Get("core:card_base")->ShaderResource.Get());
			sprite->SetOrigin({ 0.f, 1.f });
		}
	}

	void LobbyScene::OnUpdate(float aDeltaTime)
	{
		const CommonUtilities::Matrix4x4f view = myCamera->GetView();
		const CommonUtilities::Matrix4x4f projection = myCamera->GetProjection();

		SpriteRenderLayer* spriteRL = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRL = GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		spriteRL->SetView(view);
		spriteRL->SetProjection(projection);

		textRL->SetView(view);
		textRL->SetProjection(projection);

		aDeltaTime;
	}
}
