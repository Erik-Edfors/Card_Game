#include "Shared.pch.h"
#include "ResourceCard.h"
#include <Implementations/CardGame/Game.h>

namespace shared
{
	ResourceCard::ResourceCard()
	{
	}

	ResourceCard::~ResourceCard()
	{
	}

	cardgame::ResourceCount ResourceCard::GetPlayCost() const
	{
		return 0;
	}

	bool ResourceCard::CanBePlayed() const
	{
		return IsInHand();
	}

	const char* ResourceCard::GetName() const
	{
		return "Resource";
	}

	const char* ResourceCard::GetDescription() const
	{
		return "Add 1 Resource at the beginning of your Turn.";
	}

	void ResourceCard::OnInit()
	{
		BeginListen(cardgame::Game::EVENT_TURN_BEGIN);
	}

	bool ResourceCard::OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData)
	{
		aData;

		switch (aEvent)
		{
		case cardgame::Game::EVENT_TURN_BEGIN:
		{
			cardgame::Game::EventTurnBegin data = std::any_cast<cardgame::Game::EventTurnBegin>(aData);

			if (data.Owner == GetOwner() && IsInPlay() && RepeatLock("TURN_BEGIN"))
			{
				cardgame::AddPlayerResourcesCommand command = {
					.Source = GetID(),
					.Target = GetOwner(),
					.Amount = 1
				};
				
				return Do(GetGame(), command);
			}
		}
		break;
		}

		return false;
	}
}