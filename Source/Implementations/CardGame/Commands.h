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
		COMMAND_REMOVE_CARD,
		COMMAND_CREATE_LOCATION,
		COMMAND_REMOVE_LOCATION,
		COMMAND_CREATE_PLAYER,
		COMMAND_REMOVE_PLAYER,
		COMMAND_END_TURN,
		COMMAND_START,
		COMMAND_HEAL_PLAYER,
		COMMAND_DAMAGE_PLAYER,
		COMMAND_ADD_PLAYER_RESOURCES,
		COMMAND_REMOVE_PLAYER_RESOURCES,
		COMMAND_SET_NAME,

		COMMAND_SET_SEED,
		COMMAND_GENERATE_RANDOM_UNIFORM_INTEGER
	};
	using COMMAND_TYPE = unsigned short;

	struct SubmitPlayersCommand
	{
		struct Result
		{
			
		};

		PlayerID Authority;
		PlayerID Players[MAX_SELECTED_PLAYERS];
		size_t Count;
	};
	struct SubmitCardsCommand
	{
		struct Result
		{

		};

		PlayerID Authority;
		CardID Cards[MAX_SELECTED_CARDS];
		size_t Count;
	};
	struct SubmitLocationsCommand
	{
		struct Result
		{

		};

		PlayerID Authority;
		LocationID Locations[MAX_SELECTED_LOCATIONS];
		size_t Count;
	};
	struct SubmitAmountCommand
	{
		struct Result
		{

		};

		PlayerID Authority;
		int Amount;
	};
	struct SubmitOptionCommand
	{
		struct Result
		{

		};

		PlayerID Authority;
		size_t Index;
	};

	// Plays a card, if playable, and the player can pay its cost.
	struct PlayCommand
	{
		struct Result
		{
			LocationID NewLocation;
		};

		PlayerID Authority;
		PlayerID Player;
		CardID Card;
		bool IgnoreCost;
	};

	// Moves the location of a card.
	struct MoveCardCommand
	{
		struct Result
		{

		};

		CardID Card;
		LocationID Location;
	};

	// Creates a card in a specified location, owned by a specified player.
	struct CreateCardCommand
	{
		struct Result
		{
			CardID Card;
		};

		PlayerID Owner;
		LocationID Location;
		CardFactoryID FactoryID;
	};
	struct RemoveCardCommand
	{
		struct Result
		{

		};

		CardID Card;
	};
	
	struct CreateLocationCommand
	{
		struct Result
		{
			LocationID Location;
		};
	};
	struct RemoveLocationCommand
	{
		struct Result
		{
			size_t NumRemovedCards;
		};

		LocationID Location;
		bool RemoveCards;
	};
	
	struct CreatePlayerCommand
	{
		struct Result
		{
			PlayerID Player;
		};
	};
	struct RemovePlayerCommand
	{
		struct Result
		{
			size_t NumRemovedCards;
		};

		PlayerID Player;
	};

	// Begins the first turn
	struct StartCommand
	{
		struct Result
		{
		};
	};

	// Ends the current turn and begins the next one.
	struct EndTurnCommand
	{
		struct Result
		{
		};
	};

	// Heals an amount of health of a player.
	struct HealPlayerCommand
	{
		struct Result
		{
		};

		CardID Source;
		PlayerID Target;
		Health Amount;
	};

	// Deals an amount of damage to a player.
	struct DamagePlayerCommand
	{
		struct Result
		{
		};

		CardID Source;
		PlayerID Target;
		Health Amount;
	};

	struct AddPlayerResourcesCommand
	{
		struct Result
		{
		};

		CardID Source;
		PlayerID Target;
		ResourceCount Amount;
	};

	struct RemovePlayerResourcesCommand
	{
		struct Result
		{
		};

		CardID Source;
		PlayerID Target;
		ResourceCount Amount;
	};

	struct SetNameCommand
	{
		struct Result
		{

		};

		PlayerID Player;
		char Name[MAX_PLAYER_NAME_SIZE];
	};

	struct SetSeedCommand
	{
		struct Result
		{

		};

		unsigned int Seed;
	};

	struct GenerateUniformRandomIntegerCommand
	{
		struct Result
		{
			int Number;
		};

		int Min;
		int Max;
	};

	struct Command
	{
		union Result
		{
			SubmitPlayersCommand::Result SubmitPlayers;
			SubmitCardsCommand::Result SubmitCards;
			SubmitLocationsCommand::Result SubmitLocations;
			SubmitAmountCommand::Result SubmitAmount;
			SubmitOptionCommand::Result SubmitOption;

			PlayCommand::Result Play;
			MoveCardCommand::Result MoveCard;
			CreateCardCommand::Result CreateCard;
			RemoveCardCommand::Result RemoveCard;
			CreateLocationCommand::Result CreateLocation;
			RemoveLocationCommand::Result RemoveLocation;
			CreatePlayerCommand::Result CreatePlayer;
			RemovePlayerCommand::Result RemovePlayer;
			StartCommand::Result Start;
			EndTurnCommand::Result EndTurn;
			HealPlayerCommand::Result HealPlayer;
			DamagePlayerCommand::Result DamagePlayer;
			AddPlayerResourcesCommand::Result AddPlayerResources;
			RemovePlayerResourcesCommand::Result RemovePlayerResources;
			SetNameCommand::Result SetName;

			SetSeedCommand::Result SetSeed;
			GenerateUniformRandomIntegerCommand::Result GenerateUniformRandomInteger;
		};

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
			RemoveCardCommand RemoveCard;
			CreateLocationCommand CreateLocation;
			RemoveLocationCommand RemoveLocation;
			CreatePlayerCommand CreatePlayer;
			RemovePlayerCommand RemovePlayer;
			StartCommand Start;
			EndTurnCommand EndTurn;
			HealPlayerCommand HealPlayer;
			DamagePlayerCommand DamagePlayer;
			AddPlayerResourcesCommand AddPlayerResources;
			RemovePlayerResourcesCommand RemovePlayerResources;
			SetNameCommand SetName;

			SetSeedCommand SetSeed;
			GenerateUniformRandomIntegerCommand GenerateUniformRandomInteger;
		} Command;
	};
}