#pragma once
#include "../Defines.h"

struct sockaddr_storage;

namespace networking
{
	struct SocketAddress;

	unsigned short GetWs2AddressFamily(NETWORKING_ADDRESS_FAMILY aAddressFamily) noexcept;
	NETWORKING_ADDRESS_FAMILY GetAddressFamily(unsigned short aAddressFamily) noexcept;

	int GetWs2SocketType(NETWORKING_SOCKET_TYPE aSocketType) noexcept;
	NETWORKING_SOCKET_TYPE GetSocketType(int aSocketType) noexcept;

	int GetWs2InternetProtocol(NETWORKING_INTERNET_PROTOCOL aInternetProtocol) noexcept;
	NETWORKING_INTERNET_PROTOCOL GetInternetProtocol(int aInternetProtocol) noexcept;

	bool GetWs2Address(sockaddr_storage* outAddress, const SocketAddress& aAddress) noexcept;
	bool GetAddress(SocketAddress* outAddress, const sockaddr* aAddress) noexcept;
	bool GetAddress(SocketAddress* outAddress, const sockaddr* aAddress, size_t aAddressSize, NETWORKING_ADDRESS_FAMILY aFamily) noexcept;
}