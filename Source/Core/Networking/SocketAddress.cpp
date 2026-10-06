#include <Networking.pch.h>
#include "SocketAddress.h"
#include "Internal/Common.h"
#include <sstream>

namespace networking
{
	std::string to_string(const SocketAddress& aAddress)
	{
		std::stringstream ss;
		ss << aAddress;
		return ss.str();
	}

	bool GetSocketAddress(SocketAddress* outAddress, std::string_view aHostName, std::string_view aServiceName, NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_SOCKET_TYPE aSocketType, NETWORKING_INTERNET_PROTOCOL aProtocol)
	{
		const ADDRINFOA hints = {
			.ai_flags = NULL,
			.ai_family = GetWs2AddressFamily(aAddressFamily),
			.ai_socktype = GetWs2SocketType(aSocketType),
			.ai_protocol = GetWs2InternetProtocol(aProtocol),
			.ai_addrlen = NULL,
			.ai_canonname = NULL,
			.ai_addr = NULL,
			.ai_next = NULL
		};

		ADDRINFOA* results = NULL;

		if (GetAddrInfoA(aHostName.data(), aServiceName.data(), &hints, &results) != 0)
		{
			return false;
		}

		bool succeeded = GetAddress(outAddress, results->ai_addr);

		FreeAddrInfoA(results);

		return succeeded;
	}

	bool GetSocketAddress(SocketAddress* outAddress, std::wstring_view aHostName, std::wstring_view aServiceName, NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_SOCKET_TYPE aSocketType, NETWORKING_INTERNET_PROTOCOL aProtocol)
	{
		const ADDRINFOW hints = {
			.ai_flags = NULL,
			.ai_family = GetWs2AddressFamily(aAddressFamily),
			.ai_socktype = GetWs2SocketType(aSocketType),
			.ai_protocol = GetWs2InternetProtocol(aProtocol),
			.ai_addrlen = NULL,
			.ai_canonname = NULL,
			.ai_addr = NULL,
			.ai_next = NULL
		};

		ADDRINFOW* results = NULL;

		if (GetAddrInfoW(aHostName.data(), aServiceName.data(), &hints, &results) != 0)
		{
			return false;
		}

		bool succeeded = GetAddress(outAddress, results->ai_addr);

		FreeAddrInfoW(results);

		return succeeded;
	}
}

std::ostream& operator<<(std::ostream& aStream, const networking::SocketAddress& aAddress)
{
	if (aAddress == networking::SocketAddress{})
	{
		aStream << "N/A";
		return aStream;
	}

	switch (aAddress.IPAddress.Family)
	{
	case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
	{
		for (size_t i = 0; i < 3; i++)
		{
			aStream << static_cast<unsigned short>(aAddress.IPAddress.Data[i]) << '.';
		}
		aStream << static_cast<unsigned short>(aAddress.IPAddress.Data[3]) << ':' << aAddress.Port;
	}
	break;
	case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
	{
		aStream << '[' << std::hex;
		char numSemicolonLast = 0;
		bool shortened = false;
		for (size_t i = 0; i < 7; i++)
		{
			const unsigned short combined = (static_cast<unsigned short>(aAddress.IPAddress.Data[2 * i]) << 8) + static_cast<unsigned short>(aAddress.IPAddress.Data[2 * i + 1]);
			if (shortened == true || combined != 0)
			{
				aStream << combined;
				if (numSemicolonLast == 2)
				{
					shortened = true;
				}
				numSemicolonLast = 0;
			}
			if (numSemicolonLast < 2)
			{
				aStream << ':';
				++numSemicolonLast;
			}
		}
		const unsigned short combined = (static_cast<unsigned short>(aAddress.IPAddress.Data[14]) << 8) + static_cast<unsigned short>(aAddress.IPAddress.Data[15]);
		if (combined != 0)
		{
			aStream << combined;
		}
		aStream << "]:" << std::dec << aAddress.Port;

	}
	break;
	default:
		aStream << "un.kn.ow.n:family";
		break;
	}
	return aStream;
}
