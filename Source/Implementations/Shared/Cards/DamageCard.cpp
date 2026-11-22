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
		return IsInHand() && GetGame().GetNumPlayers() > 1;
	}

	const char* DamageCard::GetName() const
	{
		return "Damage";
	}

	const char* DamageCard::GetDescription() const
	{
		return "Cost: 2 RP.\n\nDeal 2 Damage to Target Opponent.";
	}

	void DamageCard::OnInit()
	{
		BeginListen(cardgame::Game::EVENT_CARD_PLAYED);
		BeginListen(cardgame::Game::EVENT_TASK_FINISHED);

		myTargetTask.Initialize(
			GetOwner(), 
			GetOwner()
		);
	}

	bool DamageCard::OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData)
	{
		switch (aEvent)
		{
		case cardgame::Game::EVENT_CARD_PLAYED:
		{
			cardgame::Game::EventCardPlayed data = std::any_cast<cardgame::Game::EventCardPlayed>(aData);
			if (data.Card == GetID())
			{
				myTargetTask.Reset();
				return PushTask(GetGame(), &myTargetTask);
			}
		}
		break;
		case cardgame::Game::EVENT_TASK_FINISHED:
		{
			if (myTargetTask.IsFinished())
			{
				const cardgame::PlayerID target = myTargetTask.GetTarget();
				myTargetTask.Reset();

				const cardgame::Player* player = GetGame().GetPlayer(GetOwner());

				const cardgame::DamagePlayerCommand damagePlayer = {
					.Source = GetID(),
					.Target = target,
					.Amount = 2
				};
				const cardgame::MoveCardCommand moveCard = {
					.Card = GetID(),
					.Location = player->GetDiscardLocation()
				};

				if (Do(GetGame(), damagePlayer))
				{
					Do(GetGame(), moveCard);

					return true;
				}
			}
		}
		break;
		}

		return false;
	}
	void DamageCard::OnDestroyed()
	{
		RetractTask(GetGame(), &myTargetTask);
	}
}