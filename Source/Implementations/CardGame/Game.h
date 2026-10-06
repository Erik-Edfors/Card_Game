#pragma once
#include <Core/CommonUtilities/Events.hpp>
#include "Commands.h"
#include "CardFactory.h"

namespace cardgame
{
	class Game
	{
	public:
		Game();
		~Game();

		static constexpr CommonUtilities::StringHash EVENT_CARD_ENTERED = "EVENT_CARD_ENTERED";
		static constexpr CommonUtilities::StringHash EVENT_CARD_PLAYED = "EVENT_CARD_PLAYED";
		static constexpr CommonUtilities::StringHash EVENT_CARD_CREATED = "EVENT_CARD_CREATED";

		struct EventCardEntered { CardID Card; PlayerID Owner; LocationID Location; };
		struct EventCardPlayed { CardID Card; PlayerID Owner; };
		struct EventCardCreated { CardID Card; PlayerID Owner; };

		bool PushCommand(const Command& aCommand);

		CardFactory& GetCardFactory() noexcept;
		CommonUtilities::EventPusher& GetEventPusher() noexcept;

	private:
		bool Do(PlayCommand aCommand);
		bool Do(MoveCardCommand aCommand);
		bool Do(CreateCardCommand aCommand);

		CommonUtilities::EventManager myEventManager;

		std::vector<Command> myCommandStack;
		CardFactory myCardFactory;
	};
}