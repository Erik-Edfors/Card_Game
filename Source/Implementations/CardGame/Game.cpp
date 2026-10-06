#include "Game.h"

namespace cardgame
{
	Game::Game()
	{
	}
	Game::~Game()
	{
	}

	bool Game::PushCommand(const Command& aCommand)
	{
		bool result = false;

		switch (aCommand.Type)
		{
		case COMMAND_PLAY: result = Do(aCommand.Play); break;
		case COMMAND_MOVE_CARD: result = Do(aCommand.MoveCard); break;
		case COMMAND_CREATE_CARD: result = Do(aCommand.CreateCard); break;
		}

		if (result)
		{
			myCommandStack.push_back(aCommand);
		}

		return result;
	}

	CardFactory& Game::GetCardFactory() noexcept
	{
		return myCardFactory;
	}

	CommonUtilities::EventPusher& Game::GetEventPusher() noexcept
	{
		return myEventManager;
	}
}