#pragma once
#include <Core/Engine/ComponentSystem/Components/Component.h>
#include <Core/CommonUtilities/Events.hpp>

namespace client
{
	class CardComponent;

	class CardZoneComponent : public Component
	{
	public:
		constexpr static CommonUtilities::StringHash EVENT_CARD_ENTER = "CARD_ENTER"_hash;
		constexpr static CommonUtilities::StringHash EVENT_CARD_EXIT = "CARD_EXIT"_hash;

		struct EventCardEnter
		{
			ComponentRef<CardComponent> Card;
		};
		struct EventCardExit
		{
			ComponentRef<CardComponent> Card;
		};

		CardZoneComponent() noexcept;
		~CardZoneComponent() noexcept;

	private:
		CommonUtilities::EventManager myEventManager;
	};
}