#pragma once
#include <Core/ComponentSystem/Defines.h>
#include <Implementations/CardGame/Defines.h>
#include "Components/ButtonComponent.h"

namespace cs
{
	class ComponentSystem;
}

namespace cardgame
{
	class Game;
}

class Scene;

namespace client
{
	class GameScene;
	class ClientApp;

	bool RegisterComponentTypes(cs::ComponentSystem& aSystem);

	cs::EntityHandle CreateCamera(ClientApp& aApp, Scene& aScene);
	cs::EntityHandle CreateFreeCamera(ClientApp& aApp, Scene& aScene);

	cs::EntityHandle CreateCard(ClientApp& aApp, GameScene& aScene, cardgame::Game& aGame, cardgame::CardID aCardID, cardgame::PlayerID aLocalPlayer);
	cs::EntityHandle CreateDisplayCard(ClientApp& aApp, GameScene& aScene, cardgame::Game& aGame, cardgame::CardID aCardID, cardgame::PlayerID aLocalPlayer);

	cs::EntityHandle CreatePlayerHealthBar(Scene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer);
	cs::EntityHandle CreatePlayerResourcesBar(Scene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer);

	cs::EntityHandle CreatePlayer(ClientApp& aApp, GameScene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer);

	cs::EntityHandle CreateDeck(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation);
	cs::EntityHandle CreateHand(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation);
	cs::EntityHandle CreatePlay(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation);
	cs::EntityHandle CreateDiscard(Scene& aScene, cardgame::Game& aGame, cardgame::LocationID aLocation);
	cs::EntityHandle CreatePlayerInfo(ClientApp& aApp, Scene& aScene, cardgame::Game& aGame, cardgame::PlayerID aPlayer);

	cs::EntityHandle CreateButton(ClientApp& aApp, Scene& aScene, std::wstring_view aText, ButtonComponent::Callback aCallback, void* aUserData);
}