#pragma once
#include "Defines.h"

namespace cardgame
{
	enum : unsigned short
	{
		COMMAND_PLAY,
		COMMAND_MOVE_CARD,
		COMMAND_CREATE_CARD,
		COMMAND_END_TURN,
		COMMAND_START,
		COMMAND_DAMAGE_PLAYER,
		COMMAND_HEAL_PLAYER,
		COMMAND_ADD_PLAYER_RESOURCES,
		COMMAND_REMOVE_PLAYER_RESOURCES,
		COMMAND_SET_NAME
	};
	using COMMAND_TYPE = unsigned short;

	// Plays a card, if playable, and the player can pay its cost.
	struct PlayCommand
	{
		PlayerID Player;
		CardID Card;
		bool IgnoreCost;
	};

	// Moves the location of a card.
	struct MoveCardCommand
	{
		CardID Card;
		LocationID Location;
	};

	// Creates a card in a specified location, owned by a specified player.
	struct CreateCardCommand
	{
		PlayerID Owner;
		LocationID Location;
		CardFactoryID FactoryID;
	};

	// Begins the first turn
	struct StartCommand
	{

	};

	// Ends the current turn and begins the next one.
	struct EndTurnCommand
	{

	};

	// Heals an amount of health of a player.
	struct HealPlayerCommand
	{
		CardID Source;
		PlayerID Target;
		Health Amount;
	};

	// Deals an amount of damage to a player.
	struct DamagePlayerCommand
	{
		CardID Source;
		PlayerID Target;
		Health Amount;
	};

	struct AddPlayerResourcesCommand
	{
		CardID Source;
		PlayerID Target;
		ResourceCount Amount;
	};

	struct RemovePlayerResourcesCommand
	{
		CardID Source;
		PlayerID Target;
		ResourceCount Amount;
	};

	struct SetNameCommand
	{
		PlayerID Player;
		char Name[24];
	};

	struct Command
	{
		COMMAND_TYPE Type;
		union
		{
			PlayCommand Play;
			MoveCardCommand MoveCard;
			CreateCardCommand CreateCard;
			StartCommand Start;
			EndTurnCommand EndTurn;
			DamagePlayerCommand DamagePlayer;
			SetNameCommand SetName;
		} Command;
	};
}