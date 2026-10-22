#include "GameObject.h"
#include "Game.h"

namespace cardgame
{
	bool GameObject::Do(Game& aGame, PlayCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, MoveCardCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, CreateCardCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, StartCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, EndTurnCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, HealPlayerCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, DamagePlayerCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, AddPlayerResourcesCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, RemovePlayerResourcesCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
	bool GameObject::Do(Game& aGame, SetNameCommand aCommand) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand); }
}