#include <Networking.pch.h>
#include "SocketAddress.h"
#include <sstream>

namespace networking
{
	std::string to_string(const SocketAddress& aAddress)
	{
		std::stringstream ss;
		ss << aAddress;
		return ss.str();
	}
}

std::ostream& operator<<(std::ostream& aStream, const networking::SocketAddress& aAddress)
{
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
