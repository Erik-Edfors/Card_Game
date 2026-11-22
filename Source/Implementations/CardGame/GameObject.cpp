#include "GameObject.h"
#include "Game.h"

namespace cardgame
{
	bool GameObject::PushTask(Game& aGame, Task* aTask) { return static_cast<GameObjectInterface&>(aGame).PushTask(aTask); }
	bool GameObject::RetractTask(Game& aGame, Task* aTask) { return static_cast<GameObjectInterface&>(aGame).RetractTask(aTask); }

	bool GameObject::Do(Game& aGame, PlayCommand aCommand, PlayCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, MoveCardCommand aCommand, MoveCardCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, CreateCardCommand aCommand, CreateCardCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, RemoveCardCommand aCommand, RemoveCardCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, CreateLocationCommand aCommand, CreateLocationCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, RemoveLocationCommand aCommand, RemoveLocationCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, CreatePlayerCommand aCommand, CreatePlayerCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, RemovePlayerCommand aCommand, RemovePlayerCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, StartCommand aCommand, StartCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, EndTurnCommand aCommand, EndTurnCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, HealPlayerCommand aCommand, HealPlayerCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, DamagePlayerCommand aCommand, DamagePlayerCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, AddPlayerResourcesCommand aCommand, AddPlayerResourcesCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, RemovePlayerResourcesCommand aCommand, RemovePlayerResourcesCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, SetNameCommand aCommand, SetNameCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }

	bool GameObject::Do(Game& aGame, SetSeedCommand aCommand, SetSeedCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
	bool GameObject::Do(Game& aGame, GenerateUniformRandomIntegerCommand aCommand, GenerateUniformRandomIntegerCommand::Result* outResult) { return static_cast<GameObjectInterface&>(aGame).Do(aCommand, outResult); }
}