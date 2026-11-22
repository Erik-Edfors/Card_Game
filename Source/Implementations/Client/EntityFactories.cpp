#include "Client.pch.h"
#include "EntityFactories.h"
#include "ClientApp.h"
#include "Scenes/GameScene.h"
#include "Components/CardComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/PlayerInfoComponent.h"
#include "Components/Locations/DeckComponent.h"
#include "Components/Locations/HandComponent.h"
#include "Components/Locations/PlayComponent.h"
#include "Components/Locations/DiscardComponent.h"

#include <Core/ComponentSystem/ComponentSystem.h>

#include <Core/Engine/Engine.h>
#include <Core/Engine/Scene.h>
#include <Core/Engine/Components/TransformComponent.h>
#include <Core/Engine/Components/CameraComponent.h>
#include <Core/Engine/Components/CameraControllerComponent.h>
#include <Core/Engine/Components/SpriteComponent.h>
#include <Core/Engine/Components/TextComponent.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Implementations/CardGame/Game.h>

#include <Core/Assets/FontRegistry.h>
#include <Core/Assets/TextureRegistry.h>

namespace client
{
	bool RegisterComponentTypes(cs::ComponentSystem& aSystem)
	{
		bool result = true;
		result &= aSystem.RegisterComponentType<TransformComponent>();
		result &= aSystem.RegisterComponentType<CameraComponent>();
		result &= aSystem.RegisterComponentType<CameraControllerComponent>();
		result &= aSystem.RegisterComponentType<SpriteComponent>();
		result &= aSystem.RegisterComponentType<TextComponent>();


		result &= aSystem.RegisterComponentType<CardComponent>();
		result &= aSystem.RegisterComponentType<PlayerComponent>();
		result &= aSystem.RegisterComponentType<DeckComponent>();
		result &= aSystem.RegisterComponentType<HandComponent>();
		result &= aSystem.RegisterComponentType<PlayComponent>();
		result &= aSystem.RegisterComponentType<DiscardComponent>();
		result &= aSystem.RegisterComponentType<PlayerInfoComponent>();

		result &= aSystem.RegisterComponentType<ButtonComponent>();
		return result;
	}

	cs::EntityHandle CreateCamera(ClientApp& aApp, Scene& aScene)
	{
		aApp;

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		cs::EntityHandle entityHandle = system.CreateEntity();

		system.AddComponent<TransformComponent>(entityHandle);
		system.AddComponent<CameraComponent>(entityHandle);

		return entityHandle;
	}

	cs::EntityHandle CreateFreeCamera(ClientApp& aApp, Scene& aScene)
	{
		cs::ComponentSystem& system = aScene.GetComponentSystem();
		Engine& engine = aScene.GetEngine();

		cs::EntityHandle entityHandle = CreateCamera(aApp, aScene);
		system.AddComponent<CameraControllerComponent>(entityHandle, &engine.GetInputManager());

		return entityHandle;
	}

	cs::EntityHandle CreateCard(ClientApp& aApp, GameScene& aScene, cardgame::Game& aGame, cardgame::CardID aCardID, cardgame::PlayerID aLocalPlayer)
	{
		cs::ComponentSystem& system = aScene.GetComponentSystem();
		Engine& engine = aScene.GetEngine();

		SpriteRenderLayer* spriteRenderLayer = engine.GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRenderLayer = engine.GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		assets::FontRegistry& fontRegistry = aApp.GetFontRegistry();
		assets::TextureRegistry& textureRegistry = aApp.GetTextureRegistry();

		const cardgame::Card* card = aGame.GetCard(aCardID);
		if (card == nullptr)
		{
			return cs::INVALID_ENTITY_HANDLE;
		}

		cs::EntityHandle entityHandle = system.CreateEntity();
		cs::EntityHandle spriteHandle = system.CreateEntity();
		cs::EntityHandle titleHandle = system.CreateEntity();
		cs::EntityHandle textHandle = system.CreateEntity();

		cs::ComponentRef<TransformComponent> entityTransform = system.AddComponent<TransformComponent>(entityHandle);
		cs::ComponentRef<TransformComponent> spriteTransform = system.AddComponent<TransformComponent>(spriteHandle);
		cs::ComponentRef<TransformComponent> titleTransform = system.AddComponent<TransformComponent>(titleHandle);
		cs::ComponentRef<TransformComponent> textTransform = system.AddComponent<TransformComponent>(textHandle);

		spriteTransform->SetParent(entityTransform);
		titleTransform->SetParent(entityTransform);
		textTransform->SetParent(entityTransform);

		const CommonUtilities::Vector3f spriteScale = { 0.625f, 1.f, 1.f };
		const CommonUtilities::Vector2f spriteOrigin = { 0.5f, 0.5f };
		spriteTransform->SetScale(spriteScale);
		titleTransform->SetTranslation({ 0.f/*0.05f - spriteOrigin.x * spriteScale.x*/, -0.1f + spriteOrigin.y * spriteScale.y, -0.01f });
		textTransform->SetTranslation({ 0.02f - spriteOrigin.x * spriteScale.x, -0.59f + spriteOrigin.y * spriteScale.y, -0.01f });

		entityTransform->SetScale({ 0.2f, 0.2f, 0.2f });

		cs::ComponentRef<SpriteComponent> spriteFront = system.AddComponent<SpriteComponent>(spriteHandle, spriteRenderLayer);
		cs::ComponentRef<SpriteComponent> spriteBack = system.AddComponent<SpriteComponent>(spriteHandle, spriteRenderLayer);
		cs::ComponentRef<TextComponent> title = system.AddComponent<TextComponent>(titleHandle, textRenderLayer);
		cs::ComponentRef<TextComponent> text = system.AddComponent<TextComponent>(textHandle, textRenderLayer);
		cs::ComponentRef<CardComponent> cardComp = system.AddComponent<CardComponent>(entityHandle, &aScene, aCardID, spriteFront.Get(), spriteBack.Get(), title.Get(), text.Get());

		spriteFront->SetEnabled(false);
		spriteFront->SetAtlas(textureRegistry.Get("core:card_base")->ShaderResource.Get());
		spriteFront->SetOrigin(spriteOrigin);

		spriteBack->SetAtlas(textureRegistry.Get("core:card_back")->ShaderResource.Get());
		spriteBack->SetOrigin(spriteOrigin);

		title->SetEnabled(false);
		title->SetFont(fontRegistry.Get("core:arial"));
		title->SetAlignment(0.5f);
		title->SetFontSize(0.1f);
		title->SetColor({ 0.f, 0.f, 0.f, 1.f });
		title->SetText("Title");

		text->SetEnabled(false);
		text->SetFont(fontRegistry.Get("core:arial"));
		text->SetFontSize(0.05f);
		text->SetColor({ 0.f, 0.f, 0.f, 1.f });
		text->SetText("Text");
		text->SetMaxWidth(0.6f);

		cardComp->SetLocalPlayer(aLocalPlayer);

		return entityHandle;
	}
	cs::EntityHandle CreateDisplayCard(ClientApp& aApp, GameScene& aScene, cardgame::Game& aGame, cardgame::CardID aCardID, cardgame::PlayerID aLocalPlayer)
	{
		cs::ComponentSystem& system = aScene.GetComponentSystem();

		cs::EntityHandle entity = CreateCard(aApp, aScene, aGame, aCardID, aLocalPlayer);

		cs::ComponentRef<TransformComponent> transform = system.GetComponent<TransformComponent>(entity);
		transform->SetScale({ transform->GetScale().x * 4.f, transform->GetScale().y * 4.f, 1.f });

		cs::ComponentRef<CardComponent> card = system.GetComponent<CardComponent>(entity);
		card->SetDisplayOnly(true);

		return entity;
	}
	cs::EntityHandle CreatePlayerHealthBar(Scene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer)
	{
		aScene;
		aGame;
		aPlayer;
		return cs::INVALID_ENTITY_HANDLE;
	}
	cs::EntityHandle CreatePlayerResourcesBar(Scene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer)
	{
		aScene;
		aGame;
		aPlayer;
		return cs::INVALID_ENTITY_HANDLE;
	}
	cs::EntityHandle CreatePlayer(ClientApp& aApp, GameScene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer)
	{
		aGame;

		Engine& engine = aScene.GetEngine();

		SpriteRenderLayer* spriteRenderLayer = engine.GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();

		assets::TextureRegistry& textureRegistry = aApp.GetTextureRegistry();

		const cardgame::Player* gamePlayer = aGame.GetPlayer(aPlayer);

		const cardgame::LocationID deck = gamePlayer->GetDeckLocation();
		const cardgame::LocationID hand = gamePlayer->GetHandLocation();
		const cardgame::LocationID play = gamePlayer->GetPlayLocation();
		const cardgame::LocationID discard = gamePlayer->GetDiscardLocation();

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		const cs::EntityHandle entity = system.CreateEntity();

		cs::ComponentRef<TransformComponent> transform = system.AddComponent<TransformComponent>(entity);
		cs::ComponentRef<SpriteComponent> sprite = system.AddComponent<SpriteComponent>(entity, spriteRenderLayer);

		cs::ComponentRef<DeckComponent> deckComp = system.GetComponent<DeckComponent>(CreateDeck(aScene, aGame, deck));
		cs::ComponentRef<HandComponent> handComp = system.GetComponent<HandComponent>(CreateHand(aScene, aGame, hand));
		cs::ComponentRef<PlayComponent> playComp = system.GetComponent<PlayComponent>(CreatePlay(aScene, aGame, play));
		cs::ComponentRef<DiscardComponent> discardComp = system.GetComponent<DiscardComponent>(CreateDiscard(aScene, aGame, discard));
		cs::ComponentRef<PlayerInfoComponent> playerInfoComp = system.GetComponent<PlayerInfoComponent>(CreatePlayerInfo(aApp, aScene, aGame, aPlayer));

		PlayerComponent* player = system.AddComponent<PlayerComponent>(
			entity,
			&aScene,
			aPlayer,
			deckComp.Get(),
			handComp.Get(),
			playComp.Get(),
			discardComp.Get(),
			playerInfoComp.Get(),
			sprite.Get()
		);
		player;

		transform->SetScale({ 0.2f, 0.2f, 1.f });

		sprite->SetAtlas(textureRegistry.Get("core:player_icon")->ShaderResource.Get());
		sprite->SetOrigin({ 0.5f, 0.5f });

		return entity;
	}
	cs::EntityHandle CreateDeck(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation)
	{
		aGame;

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		const cs::EntityHandle entity = system.CreateEntity();

		system.AddComponent<TransformComponent>(entity);
		system.AddComponent<DeckComponent>(entity, aLocation);

		return entity;
	}
	cs::EntityHandle CreateHand(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation)
	{
		aGame;

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		const cs::EntityHandle entity = system.CreateEntity();

		system.AddComponent<TransformComponent>(entity);
		system.AddComponent<HandComponent>(entity, aLocation);

		return entity;
	}
	cs::EntityHandle CreatePlay(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation)
	{
		aGame;

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		const cs::EntityHandle entity = system.CreateEntity();

		system.AddComponent<TransformComponent>(entity);
		system.AddComponent<PlayComponent>(entity, aLocation);

		return entity;
	}
	cs::EntityHandle CreateDiscard(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation)
	{
		aGame;

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		const cs::EntityHandle entity = system.CreateEntity();

		system.AddComponent<TransformComponent>(entity);
		system.AddComponent<DiscardComponent>(entity, aLocation);

		return entity;
	}
	cs::EntityHandle CreatePlayerInfo(ClientApp& aApp, Scene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer)
	{
		constexpr float fontSize = 0.025f;

		assets::FontRegistry& fontRegistry = aApp.GetFontRegistry();
		assets::TextureRegistry& textureRegistry = aApp.GetTextureRegistry();

		SpriteRenderLayer* spriteRenderLayer = aApp.GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRenderLayer = aApp.GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		const cs::EntityHandle entity = system.CreateEntity();
		const cs::EntityHandle spriteHandle = system.CreateEntity();
		const cs::EntityHandle textHandle = system.CreateEntity();

		cs::ComponentRef<TransformComponent> entityTransform = system.AddComponent<TransformComponent>(entity);
		cs::ComponentRef<TransformComponent> spriteTransform = system.AddComponent<TransformComponent>(spriteHandle);
		cs::ComponentRef<TransformComponent> textTransform = system.AddComponent<TransformComponent>(textHandle);

		spriteTransform->SetParent(entityTransform);
		spriteTransform->SetScale({ 1.5f * 8.f * fontSize, 1.5f * fontSize, 1.f });

		textTransform->SetParent(entityTransform);
		textTransform->SetTranslation({ 0.2f * fontSize, 0.25f * fontSize, -0.01f });

		cs::ComponentRef<SpriteComponent> sprite = system.AddComponent<SpriteComponent>(spriteHandle, spriteRenderLayer);
		sprite->SetAtlas(textureRegistry.Get("core:player_info_back")->ShaderResource.Get());

		cs::ComponentRef<TextComponent> text = system.AddComponent<TextComponent>(textHandle, textRenderLayer);
		text->SetFont(fontRegistry.Get("core:arial"));
		text->SetFontSize(fontSize);
		text->SetColor({ 0.f, 0.f, 0.f, 1.f });

		system.AddComponent<PlayerInfoComponent>(entity, &aGame, aPlayer, sprite.Get(), text.Get());

		return entity;
	}
	cs::EntityHandle CreateButton(ClientApp& aApp, Scene& aScene, std::wstring_view aText, ButtonComponent::Callback aCallback, void* aUserData)
	{
		constexpr float fontSize = 0.025f;

		const CommonUtilities::InputManager& input = aApp.GetEngine().GetInputManager();

		assets::FontRegistry& fontRegistry = aApp.GetFontRegistry();
		assets::TextureRegistry& textureRegistry = aApp.GetTextureRegistry();

		SpriteRenderLayer* spriteRenderLayer = aApp.GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRenderLayer = aApp.GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		cs::ComponentSystem& system = aScene.GetComponentSystem();

		const cs::EntityHandle entity = system.CreateEntity();
		const cs::EntityHandle spriteHandle = system.CreateEntity();
		const cs::EntityHandle textHandle = system.CreateEntity();

		cs::ComponentRef<TransformComponent> entityTransform = system.AddComponent<TransformComponent>(entity);
		cs::ComponentRef<TransformComponent> spriteTransform = system.AddComponent<TransformComponent>(spriteHandle);
		cs::ComponentRef<TransformComponent> textTransform = system.AddComponent<TransformComponent>(textHandle);

		spriteTransform->SetParent(entityTransform);
		spriteTransform->SetScale({ 4.f * 1.5f * fontSize, 1.5f * fontSize, 1.f });

		textTransform->SetParent(entityTransform);
		textTransform->SetTranslation({ 0.f * fontSize, -0.4f * fontSize, -0.01f });

		cs::ComponentRef<SpriteComponent> sprite = system.AddComponent<SpriteComponent>(spriteHandle, spriteRenderLayer);
		sprite->SetAtlas(textureRegistry.Get("core:button_frame")->ShaderResource.Get());
		sprite->SetOrigin({ 0.5f, 0.5f });

		cs::ComponentRef<ButtonComponent> button = system.AddComponent<ButtonComponent>(entity, &input, sprite.Get());
		button->SetCallback(aCallback, aUserData);

		cs::ComponentRef<TextComponent> text = system.AddComponent<TextComponent>(textHandle, textRenderLayer);
		text->SetFont(fontRegistry.Get("core:arial"));
		text->SetFontSize(fontSize);
		text->SetColor({ 0.f, 0.f, 0.f, 1.f });
		text->SetAlignment(0.5f);
		text->SetText(aText.data());

		return entity;
	}
}