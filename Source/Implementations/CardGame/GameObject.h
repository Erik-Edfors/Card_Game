#pragma once
#include "Commands.h"

namespace cardgame
{
	class Game;

	class GameObject
	{
	protected:
		bool Do(Game& aGame, PlayCommand aCommand);
		bool Do(Game& aGame, MoveCardCommand aCommand);
		bool Do(Game& aGame, CreateCardCommand aCommand);
		bool Do(Game& aGame, StartCommand aCommand);
		bool Do(Game& aGame, EndTurnCommand aCommand);
		bool Do(Game& aGame, HealPlayerCommand aCommand);
		bool Do(Game& aGame, DamagePlayerCommand aCommand);
		bool Do(Game& aGame, AddPlayerResourcesCommand aCommand);
		bool Do(Game& aGame, RemovePlayerResourcesCommand aCommand);
		bool Do(Game& aGame, SetNameCommand aCommand);
	};
}