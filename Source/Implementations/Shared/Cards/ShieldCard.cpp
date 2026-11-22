#include "Shared.pch.h"
#include "ShieldCard.h"

#include <Implementations/CardGame/Game.h>

namespace shared
{
	ShieldCard::ShieldCard()
	{
	}
	ShieldCard::~ShieldCard()
	{
	}
	cardgame::ResourceCount ShieldCard::GetPlayCost() const
	{
		return 1;
	}
	bool ShieldCard::CanBePlayed() const
	{
		return IsInHand();
	}
	const char* ShieldCard::GetName() const
	{
		return "Shield";
	}
	const char* ShieldCard::GetDescription() const
	{
		return "Cost: 1 RP.\n\nThe next time you would take Damage, Heal an equal amount.";
	}
	void ShieldCard::OnInit()
	{
		BeginListen(cardgame::Game::EVENT_PLAYER_DAMAGED);
	}
	bool ShieldCard::OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData)
	{
		switch (aEvent)
		{
		case cardgame::Game::EVENT_PLAYER_DAMAGED:
			if (IsInPlay())
			{
				cardgame::Game::EventPlayerDamaged data = std::any_cast<cardgame::Game::EventPlayerDamaged>(aData);

				if (data.Target == GetOwner())
				{
					if (RepeatLock("HEAL_OWNER"))
					{
						const cardgame::Player* player = GetGame().GetPlayer(GetOwner());

						const cardgame::HealPlayerCommand healPlayer = {
							.Source = GetID(),
							.Target = GetOwner(),
							.Amount = data.Amount
						};
						const cardgame::MoveCardCommand moveCard = {
							.Card = GetID(),
							.Location = player->GetDiscardLocation()
						};

						if (Do(GetGame(), healPlayer))
						{
							Do(GetGame(), moveCard);

							return true;
						}
					}
				}
			}
			break;
		}
		return false;
	}
}
