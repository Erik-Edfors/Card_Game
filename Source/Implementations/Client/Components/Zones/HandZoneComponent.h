#pragma once
#include "CardZoneComponent.h"

namespace client
{
	class HandZoneComponent : public CardZoneComponent
	{
	public:
		constexpr static CommonUtilities::StringHash EVENT_CARD_PLAYED = "CARD_PLAYED"_hash;
		constexpr static CommonUtilities::StringHash EVENT_CARD_DISCARDED = "CARD_DISCARDED"_hash;

		struct EventCardPlayed
		{
			unsigned int CostPayed;
			ComponentRef<CardComponent> Card;
		};
		struct EventCardDiscarded
		{
			ComponentRef<CardComponent> Card;
		};

	private:
	};
}