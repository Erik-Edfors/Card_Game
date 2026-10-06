#include "Card.h"
#include "Game.h"

namespace cardgame
{
	Card::Card() :
		myGame(nullptr)
	{
	}

	Card::~Card()
	{
	}

	void Card::Init(Game* aGame)
	{
		myGame = aGame;
		myEventListener.SetPusher(&myGame->GetEventPusher());
	}
	void Card::Update()
	{
		CommonUtilities::Event event;
		while (myEventListener.GetEvent(&event))
		{
			OnEvent(event);
		}
	}

	Game* Card::GetGame() const noexcept
	{
		return myGame;
	}
}