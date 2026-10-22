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
				size_t numPlayers = GetGame().GetNumPlayers();
				const cardgame::Player** players = new const cardgame::Player * [numPlayers];
				numPlayers = GetGame().GetPlayers(players, numPlayers);

				for (size_t i = 0; i < numPlayers; i++)
				{
					const cardgame::Player& player = *players[i];
					if (GetOwner() != player.GetID())
					{
						cardgame::DamagePlayerCommand command = {
							.Source = GetID(),
							.Target = player.GetID(),
							.Amount = 2
						};

						Do(GetGame(), command);
					}
				}

				const cardgame::Player* owner = GetGame().GetPlayer(GetOwner());

				cardgame::MoveCardCommand command = {
					.Card = GetID(),
					.Location = owner->GetDiscardLocation()
				};
				Do(GetGame(), command);

				delete[] players;

				return true;
			}
		}
		break;
		}

		return false;
	}
}