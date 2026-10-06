#pragma once
#include "Defines.h"

namespace cardgame
{
	enum : unsigned short
	{
		COMMAND_PLAY,
		COMMAND_MOVE_CARD,
		COMMAND_CREATE_CARD,
	};
	using COMMAND_TYPE = unsigned short;

	struct PlayCommand
	{
		PlayerID Player;
		CardID Card;
	};

	struct MoveCardCommand
	{
		CardID Card;
		LocationID Location;
	};

	struct CreateCardCommand
	{
		PlayerID Owner;
		CardID ID;
		CardFactoryID FactoryID;
	};

	struct Command
	{
		COMMAND_TYPE Type;
		union
		{
			PlayCommand Play;
			MoveCardCommand MoveCard;
			CreateCardCommand CreateCard;
		};
	};
}