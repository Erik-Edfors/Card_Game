#pragma once
#include "MessageID.h"
#include <Core/CommonUtilities/Serialization/TypeMacros.h>

namespace shared
{
	struct NM_Ping
	{
		NETWORK_DEFINE_MESSAGE_ID("PING");
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

		NETWORK_DISCONNECT_REASON Reason;
	};

	struct NM_ConnectionResult
	{
		NETWORK_DEFINE_MESSAGE_ID("CONNECTION_RESULT");

		bool Accepted;
	};
}