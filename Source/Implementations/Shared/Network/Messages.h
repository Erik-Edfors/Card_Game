#pragma once
#include "../Defines.h"
#include "MessageID.h"
#include <Implementations/CardGame/Defines.h>
#include <Implementations/CardGame/Commands.h>
#include <Core/CommonUtilities/Serialization/TypeMacros.h>

namespace shared
{
	struct NM_Ping
	{
		NETWORK_DEFINE_MESSAGE_ID("PING");
	};

	struct NM_KeepAlive
	{
		NETWORK_DEFINE_MESSAGE_ID("KEEP_ALIVE");
	};

	struct NM_ServerInfoRequest
	{
		NETWORK_DEFINE_MESSAGE_ID("REQ_SERVER_INFO");
	};

	struct NM_ServerInfo
	{
		NETWORK_DEFINE_MESSAGE_ID("SERVER_INFO");

		std::string Name;
		std::string Description;
	};
	NBS_DEFINE_TYPE(
		NM_ServerInfo,
		Name,
		Description
	);

	struct NM_ConnectionRequest
	{
		NETWORK_DEFINE_MESSAGE_ID("REQ_CONNECTION");
	};

	enum : unsigned char
	{
		NETWORK_DISCONNECT_UNKNOWN,
		NETWORK_DISCONNECT_END_OF_SESSION,
		NETWORK_DISCONNECT_TIMED_OUT,
		NETWORK_DISCONNECT_KICKED_BY_OPERATOR,
	};
	using NETWORK_DISCONNECT_REASON = unsigned char;

	struct NM_Disconnect
	{
		NETWORK_DEFINE_MESSAGE_ID("DISCONNECT");

		ClientID Client;
		NETWORK_DISCONNECT_REASON Reason;
	};

	struct NM_ConnectionResult
	{
		NETWORK_DEFINE_MESSAGE_ID("CONNECTION_RESULT");

		ClientID Client;
		bool Accepted;
	};

	struct NM_NameRequest
	{
		NETWORK_DEFINE_MESSAGE_ID("REQ_NAME");

		std::string Name;
	};
	NBS_DEFINE_TYPE(
		NM_NameRequest,
		Name
	);

	struct NM_NameChange
	{
		NETWORK_DEFINE_MESSAGE_ID("NAME_CHANGE");

		ClientID Client;
		std::string Name;
	};
	NBS_DEFINE_TYPE(
		NM_NameChange,
		Client,
		Name
	);

	struct NM_AssignPlayer
	{
		NETWORK_DEFINE_MESSAGE_ID("ASSIGN_PLAYER");

		cardgame::PlayerID PlayerID;
	};

	struct NM_StartGame
	{
		NETWORK_DEFINE_MESSAGE_ID("START_GAME");

		size_t NumPlayers;
	};

	struct NM_GameReady
	{
		NETWORK_DEFINE_MESSAGE_ID("GAME_READY");
	};

#pragma region Client Actions

	//------------------
	// CLIENT ACTIONS
	//------------------

	struct NM_SubmitPlayers
	{
		NETWORK_DEFINE_MESSAGE_ID("SUBMIT_PLAYERS");

		cardgame::PlayerID Players[cardgame::MAX_SELECTED_PLAYERS];
		size_t Count;
	};
	struct NM_SubmitCards
	{
		NETWORK_DEFINE_MESSAGE_ID("SUBMIT_CARDS");

		cardgame::CardID Cards[cardgame::MAX_SELECTED_CARDS];
		size_t Count;
	};
	struct NM_SubmitLocations
	{
		NETWORK_DEFINE_MESSAGE_ID("SUBMIT_LOCATIONS");

		cardgame::LocationID Locations[cardgame::MAX_SELECTED_LOCATIONS];
		size_t Count;
	};
	struct NM_SubmitAmount
	{
		NETWORK_DEFINE_MESSAGE_ID("SUBMIT_AMOUNT");

		int Amount;
	};
	struct NM_SubmitOption
	{
		NETWORK_DEFINE_MESSAGE_ID("SUBMIT_OPTION");

		size_t Index;
	};

	struct NM_PlayCardRequest
	{
		NETWORK_DEFINE_MESSAGE_ID("REQ_PLAY_CARD");

		cardgame::PlayerID Player;
		cardgame::CardID Card;
	};

	struct NM_EndTurnRequest
	{
		NETWORK_DEFINE_MESSAGE_ID("REQ_END_TURN");
	};

#pragma endregion

	struct NM_Command
	{
		NETWORK_DEFINE_MESSAGE_ID("COMMAND");

		cardgame::Command Command;
		size_t Index;
	};
}