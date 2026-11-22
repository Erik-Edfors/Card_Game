#pragma once

namespace cardgame
{
	enum : unsigned char
	{
		TASK_TARGET_PLAYERS,
		TASK_TARGET_CARDS,
		TASK_TARGET_LOCATIONS,
		TASK_AMOUNT,
		TASK_OPTION,
	};
	using TASK_TYPE = unsigned char;

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

	constexpr unsigned int MAX_PLAYER_NAME_SIZE = 16;
}