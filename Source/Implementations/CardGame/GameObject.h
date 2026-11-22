#pragma once
#include "Commands.h"

namespace cardgame
{
	class Game;
	class Task;

	class GameObject
	{
	protected:
		bool PushTask(Game& aGame, Task* aTask);
		bool RetractTask(Game& aGame, Task* aTask);

		bool Do(Game& aGame, PlayCommand aCommand, PlayCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, MoveCardCommand aCommand, MoveCardCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, CreateCardCommand aCommand, CreateCardCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, RemoveCardCommand aCommand, RemoveCardCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, CreateLocationCommand aCommand, CreateLocationCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, RemoveLocationCommand aCommand, RemoveLocationCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, CreatePlayerCommand aCommand, CreatePlayerCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, RemovePlayerCommand aCommand, RemovePlayerCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, StartCommand aCommand, StartCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, EndTurnCommand aCommand, EndTurnCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, HealPlayerCommand aCommand, HealPlayerCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, DamagePlayerCommand aCommand, DamagePlayerCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, AddPlayerResourcesCommand aCommand, AddPlayerResourcesCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, RemovePlayerResourcesCommand aCommand, RemovePlayerResourcesCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, SetNameCommand aCommand, SetNameCommand::Result* outResult = nullptr);

		bool Do(Game& aGame, SetSeedCommand aCommand, SetSeedCommand::Result* outResult = nullptr);
		bool Do(Game& aGame, GenerateUniformRandomIntegerCommand aCommand, GenerateUniformRandomIntegerCommand::Result* outResult = nullptr);
	};
}