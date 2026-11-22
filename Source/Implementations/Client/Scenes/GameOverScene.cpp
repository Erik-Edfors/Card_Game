#include "Client.pch.h"
#include "GameOverScene.h"
#include "ClientApp.h"
#include "EntityFactories.h"
#include "Scenes/ServerSelectScene.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/Components/TransformComponent.h>
#include <Core/Engine/Components/TextComponent.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>
#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>
#include <Core/GraphicsEngine/RenderLayers/TextRenderLayer.h>
#include <Core/GraphicsEngine/Projection.h>

#include <Core/RenderHardwareInterface/Window.h>

#include <Core/Assets/TextureRegistry.h>
#include <Core/Assets/FontRegistry.h>

namespace client
{
	GameOverScene::GameOverScene(ClientApp* aApp) :
		myApp(aApp),
		myAspectRatio(0.f)
	{
	}
	GameOverScene::~GameOverScene()
	{
	}
	void GameOverScene::SetWinnerName(std::string_view aName)
	{
		myText->SetText(std::format("Winner!\n{}", aName).c_str());
	}
	void GameOverScene::OnInit()
	{
		cs::ComponentSystem& system = GetComponentSystem();

		if (not RegisterComponentTypes(system))
		{
			//TODO: Log failed to register components.
		}

		assets::FontRegistry& fontRegistry = myApp->GetFontRegistry();

		SpriteRenderLayer* spriteRL = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRL = GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		const CommonUtilities::Matrix4x4f view(
			{
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f
			}
		);
		const CommonUtilities::Matrix4x4f projection = CreateOrthographicProjection(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);


		spriteRL->SetView(view);
		spriteRL->SetProjection(projection);

		textRL->SetView(view);
		textRL->SetProjection(projection);



		system.RegisterComponentType<TransformComponent>();
		system.RegisterComponentType<TextComponent>();

		cs::EntityHandle entity = system.CreateEntity();

		cs::ComponentRef<TransformComponent> transform = system.AddComponent<TransformComponent>(entity);
		cs::ComponentRef<TextComponent> text = system.AddComponent<TextComponent>(entity, textRL);

		myText = text;

		text->SetColor({ 1.f, 1.f, 1.f, 1.f });
		text->SetFont(fontRegistry.Get("core:arialbd"));
		text->SetFontSize(0.2f);
		text->SetAlignment(0.5f);

		transform->SetTranslation({ 0.5f, 0.5f, 0.5f });

		cs::ComponentRef<TransformComponent> buttonTransform = system.GetComponent<TransformComponent>(CreateButton(*myApp, *this, L"Exit", &OnExitButton, this));
		buttonTransform->SetTranslation({ 0.5f, 0.15f, 0.5f });
		buttonTransform->SetScale({ 2.f, 2.f, 2.f });
	}
	void GameOverScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		graphics::Window* window = GetEngine().GetGraphicsEngine().GetWindow();

		const float aspectRatio = window->GetClientWidth() / window->GetClientHeight();

		if (aspectRatio != myAspectRatio)
		{
			SpriteRenderLayer* spriteRL = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
			TextRenderLayer* textRL = GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

			if (aspectRatio < 1.f)
			{
				const float padding = ((1.f / aspectRatio) - 1.f) / 2.f;
				const CommonUtilities::Matrix4x4f projection = CreateOrthographicProjection(0.f, 1.f, 0.f - padding, 1.f + padding, 0.f, 1.f);
				spriteRL->SetProjection(projection);
				textRL->SetProjection(projection);
			}
			else
			{
				const float padding = (aspectRatio - 1.f) / 2.f;
				const CommonUtilities::Matrix4x4f projection = CreateOrthographicProjection(0.f - padding, 1.f + padding, 0.f, 1.f, 0.f, 1.f);
				spriteRL->SetProjection(projection);
				textRL->SetProjection(projection);
			}
			myAspectRatio = aspectRatio;
		}
	}
	void GameOverScene::OnExitButton(const ButtonComponent::CallbackData& aData)
	{
		GameOverScene& scene = *reinterpret_cast<GameOverScene*>(aData.UserData);

		scene.GetEngine().ChangeScene<ServerSelectScene>(scene.myApp);
	}
}
