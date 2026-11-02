#pragma once
#include "Defines.h"

namespace cardgame
{
	enum : unsigned short
	{
		COMMAND_SUBMIT_PLAYERS,
		COMMAND_SUBMIT_CARDS,
		COMMAND_SUBMIT_LOCATIONS,
		COMMAND_SUBMIT_AMOUNT,
		COMMAND_SUBMIT_OPTION,

		COMMAND_PLAY,
		COMMAND_MOVE_CARD,
		COMMAND_CREATE_CARD,
		COMMAND_END_TURN,
		COMMAND_START,
		COMMAND_HEAL_PLAYER,
		COMMAND_DAMAGE_PLAYER,
		COMMAND_ADD_PLAYER_RESOURCES,
		COMMAND_REMOVE_PLAYER_RESOURCES,
		COMMAND_SET_NAME
	};
	using COMMAND_TYPE = unsigned short;

	struct SubmitPlayersCommand
	{
		PlayerID Players[MAX_SELECTED_PLAYERS];
		size_t Count;
	};
	struct SubmitCardsCommand
	{
		CardID Cards[MAX_SELECTED_CARDS];
		size_t Count;
	};
	struct SubmitLocationsCommand
	{
		LocationID Locations[MAX_SELECTED_LOCATIONS];
		size_t Count;
	};
	struct SubmitAmountCommand
	{
		int Amount;
	};
	struct SubmitOptionCommand
	{
		size_t Index;
	};

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
			SubmitPlayersCommand SubmitPlayers;
			SubmitCardsCommand SubmitCards;
			SubmitLocationsCommand SubmitLocations;
			SubmitAmountCommand SubmitAmount;
			SubmitOptionCommand SubmitOption;

			PlayCommand Play;
			MoveCardCommand MoveCard;
			CreateCardCommand CreateCard;
			StartCommand Start;
			EndTurnCommand EndTurn;
			HealPlayerCommand HealPlayer;
			DamagePlayerCommand DamagePlayer;
			AddPlayerResourcesCommand AddPlayerResources;
			RemovePlayerResourcesCommand RemovePlayerResources;
			SetNameCommand SetName;
		} Command;
	};
}