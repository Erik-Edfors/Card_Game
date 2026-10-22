#include "Shared.pch.h"
#include "Cards.h"

#include "ResourceCard.h"
#include "DamageCard.h"

#include <Implementations/CardGame/Game.h>
#include <Implementations/CardGame/CardFactory.h>

namespace shared
{
	void RegisterCards(cardgame::CardFactory& aCardFactory)
	{
		aCardFactory.RegisterCard<ResourceCard>("CORE_RESOURCE_CARD"_hash);
		aCardFactory.RegisterCard<DamageCard>("CORE_DAMAGE_CARD"_hash);
		//TODO: Register cards.
		//aCardFactory.RegisterCard<>();
	}
}
