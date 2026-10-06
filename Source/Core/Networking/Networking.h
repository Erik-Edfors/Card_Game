#pragma once
#include "Defines.h"

namespace networking
{
	bool Initialize();
	bool Shutdown();

	constexpr size_t GetNetworkOverhead(NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_INTERNET_PROTOCOL aProtocol) noexcept;



	constexpr size_t GetNetworkOverhead(NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_INTERNET_PROTOCOL aProtocol) noexcept
	{
		size_t overhead = 0u;
		switch (aAddressFamily)
		{
			case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4: overhead += 20u; break;
			case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6: overhead += 40u; break;
			default: break;
		}
		switch (aProtocol)
		{
			case NETWORKING_INTERNET_PROTOCOL_UDP: overhead += 8u; break;
			case NETWORKING_INTERNET_PROTOCOL_TCP: overhead += 20u; break;
		}
		return overhead;
	}
}