#include "Shared.pch.h"
#include "Cards.h"

#include "ResourceCard.h"
#include "DamageCard.h"
#include "ShieldCard.h"
#include "SacrificeCard.h"
#include "DestroyCard.h"

#include <Implementations/CardGame/Game.h>
#include <Implementations/CardGame/CardFactory.h>

namespace shared
{
	void RegisterCards(cardgame::CardFactory& aCardFactory)
	{
		aCardFactory.RegisterCard<ResourceCard>("CORE_RESOURCE_CARD"_hash);
		aCardFactory.RegisterCard<DamageCard>("CORE_DAMAGE_CARD"_hash);
		aCardFactory.RegisterCard<ShieldCard>("CORE_SHIELD_CARD"_hash);
		aCardFactory.RegisterCard<SacrificeCard>("CORE_SACRIFICE_CARD"_hash);
		aCardFactory.RegisterCard<DestroyCard>("CORE_DESTROY_CARD"_hash);
	}
}
