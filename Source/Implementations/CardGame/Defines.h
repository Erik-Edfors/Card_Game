#pragma once

namespace cardgame
{
	using CardFactoryID = unsigned int;
	using PlayerID = unsigned int;
	using CardID = unsigned int;
	using LocationID = unsigned int;
	using TurnIndex = unsigned int;
	using ResourceCount = signed long;
	using Health = signed long;

	constexpr unsigned int INVALID_PLAYER_ID = 0;
	constexpr unsigned int INVALID_CARD_ID = 0;
	constexpr unsigned int INVALID_LOCATION_ID = 0;

	constexpr int MAX_SELECTED_PLAYERS = 16;
	constexpr int MAX_SELECTED_CARDS = 16;
	constexpr int MAX_SELECTED_LOCATIONS = 16;
}