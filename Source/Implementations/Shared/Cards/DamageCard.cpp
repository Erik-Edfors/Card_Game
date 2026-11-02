#include "Shared.pch.h"
#include "DamageCard.h"
#include <Implementations/CardGame/Game.h>
#include <Implementations/CardGame/GameObject.h>
#include <Implementations/CardGame/Game.h>
#include <Implementations/CardGame/Game.h>
#include <Implementations/CardGame/Game.h>

namespace shared
{
	DamageCard::DamageCard()
	{
	}
	DamageCard::~DamageCard()
	{
	}

	cardgame::ResourceCount DamageCard::GetPlayCost() const
	{
		return 2;
	}

	bool DamageCard::CanBePlayed() const
	{
		return IsInHand();
	}

	const char* DamageCard::GetName() const
	{
		return "Damage";
	}

	const char* DamageCard::GetDescription() const
	{
		return "Deals 2 Damage to each Opponent.";
	}

	void DamageCard::OnInit()
	{
		BeginListen(cardgame::Game::EVENT_CARD_PLAYED);
		BeginListen(cardgame::Game::EVENT_TASK_SUBMITTED);
	}

	bool DamageCard::OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData)
	{
		switch (aEvent)
		{
		case cardgame::Game::EVENT_CARD_PLAYED:
		{
			cardgame::Game::EventCardPlayed data = std::any_cast<cardgame::Game::EventCardPlayed>(aData);
			if (data.Card == GetID() && RepeatLock("PLAYED"))
			{
				myTargetTask = {};
				myTargetTask.SetOwner(GetOwner());

				return PushTask(GetGame(), &myTargetTask);
			}
		}
		break;
		case cardgame::Game::EVENT_TASK_SUBMITTED:
		{
			if (myTargetTask.IsFinished())
			{
				cardgame::DamagePlayerCommand command = {
					.Source = GetID(),
					.Target = myTargetTask.GetTarget(),
					.Amount = 2
				};
				myTargetTask = {};

				return Do(GetGame(), command);
			}
		}
		break;
		}

		return false;
	}
}