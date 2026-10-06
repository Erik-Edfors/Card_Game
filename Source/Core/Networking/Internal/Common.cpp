#include "Networking.pch.h"
#include "Common.h"
#include "../SocketAddress.h"

#undef __RETURN_CASE
#define __RETURN_CASE(Value, Return) case Value: return Return;
#undef __DEFAULT_RETURN_CASE
#define __DEFAULT_RETURN_CASE(Return) default: return Return;

namespace networking
{
	unsigned short GetWs2AddressFamily(NETWORKING_ADDRESS_FAMILY aAddressFamily) noexcept
	{
		switch (aAddressFamily)
		{
			__RETURN_CASE(NETWORKING_ADDRESS_FAMILY_UNSPECIFIED, AF_UNSPEC);
			__RETURN_CASE(NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4, AF_INET);
			__RETURN_CASE(NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6, AF_INET6);
			__DEFAULT_RETURN_CASE(AF_UNSPEC);
		}
	}

	NETWORKING_ADDRESS_FAMILY GetAddressFamily(unsigned short aAddressFamily) noexcept
	{
		switch (aAddressFamily)
		{
			__RETURN_CASE(AF_UNSPEC, NETWORKING_ADDRESS_FAMILY_UNSPECIFIED);
			__RETURN_CASE(AF_INET, NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4);
			__RETURN_CASE(AF_INET6, NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6);
			__DEFAULT_RETURN_CASE(NETWORKING_ADDRESS_FAMILY_UNSPECIFIED);
		}
	}
	int GetWs2SocketType(NETWORKING_SOCKET_TYPE aSocketType) noexcept
	{
		switch (aSocketType)
		{
			__RETURN_CASE(NETWORKING_SOCKET_TYPE_UNSPECIFIED, NULL);
			__RETURN_CASE(NETWORKING_SOCKET_TYPE_STREAM, SOCK_STREAM);
			__RETURN_CASE(NETWORKING_SOCKET_TYPE_DGRAM, SOCK_DGRAM);
			__RETURN_CASE(NETWORKING_SOCKET_TYPE_RAW, SOCK_RAW);
			__RETURN_CASE(NETWORKING_SOCKET_TYPE_RDM, SOCK_RDM);
			__RETURN_CASE(NETWORKING_SOCKET_TYPE_SEQPACKET, SOCK_SEQPACKET);
			__DEFAULT_RETURN_CASE(SOCK_STREAM);
		}
	}
	NETWORKING_SOCKET_TYPE GetSocketType(int aSocketType) noexcept
	{
		switch (aSocketType)
		{
			__RETURN_CASE(NULL, NETWORKING_SOCKET_TYPE_UNSPECIFIED);
			__RETURN_CASE(SOCK_STREAM, NETWORKING_SOCKET_TYPE_STREAM);
			__RETURN_CASE(SOCK_DGRAM, NETWORKING_SOCKET_TYPE_DGRAM);
			__RETURN_CASE(SOCK_RAW, NETWORKING_SOCKET_TYPE_RAW);
			__RETURN_CASE(SOCK_RDM, NETWORKING_SOCKET_TYPE_RDM);
			__RETURN_CASE(SOCK_SEQPACKET, NETWORKING_SOCKET_TYPE_SEQPACKET);
			__DEFAULT_RETURN_CASE(NETWORKING_SOCKET_TYPE_STREAM);
		}
	}
	int GetWs2InternetProtocol(NETWORKING_INTERNET_PROTOCOL aInternetProtocol) noexcept
	{
		switch (aInternetProtocol)
		{
			__RETURN_CASE(NETWORKING_INTERNET_PROTOCOL_UNSPECIFIED, NULL);
			__RETURN_CASE(NETWORKING_INTERNET_PROTOCOL_TCP, IPPROTO_TCP);
			__RETURN_CASE(NETWORKING_INTERNET_PROTOCOL_UDP, IPPROTO_UDP);
			__DEFAULT_RETURN_CASE(IPPROTO_UDP);
		}
	}
	NETWORKING_INTERNET_PROTOCOL GetInternetProtocol(int aInternetProtocol) noexcept
	{
		switch (aInternetProtocol)
		{
			__RETURN_CASE(NULL, NETWORKING_INTERNET_PROTOCOL_UNSPECIFIED);
			__RETURN_CASE(IPPROTO_TCP, NETWORKING_INTERNET_PROTOCOL_TCP);
			__RETURN_CASE(IPPROTO_UDP, NETWORKING_INTERNET_PROTOCOL_UDP);
			__DEFAULT_RETURN_CASE(NETWORKING_INTERNET_PROTOCOL_UDP);
		}
	}
	bool GetWs2Address(sockaddr_storage* outAddress, const SocketAddress& aAddress) noexcept
	{
		switch (aAddress.IPAddress.Family)
		{
		case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
		{
			sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(outAddress);
			addr->sin_family = GetWs2AddressFamily(aAddress.IPAddress.Family);
			addr->sin_port = htons(aAddress.Port);
			memcpy(&addr->sin_addr.S_un.S_addr, aAddress.IPAddress.Data, sizeof(addr->sin_addr.S_un.S_addr));
			return true;
		}
		case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
		{
			sockaddr_in6* addr = reinterpret_cast<sockaddr_in6*>(outAddress);
			addr->sin6_family = GetWs2AddressFamily(aAddress.IPAddress.Family);
			addr->sin6_port = htons(aAddress.Port);
			memcpy(&addr->sin6_addr.u.Byte, aAddress.IPAddress.Data, sizeof(addr->sin6_addr.u.Byte));
			return true;
		}
		default: return false;
		}
	}
	bool GetAddress(SocketAddress* outAddress, const sockaddr* aAddress) noexcept
	{
		switch (GetAddressFamily(aAddress->sa_family))
		{
		case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
		{
			*outAddress = {};
			const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(aAddress);
			outAddress->IPAddress.Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4;
			outAddress->Port = ntohs(addr->sin_port);
			memcpy(outAddress->IPAddress.Data, &addr->sin_addr.S_un.S_addr, sizeof(addr->sin_addr.S_un.S_addr));
			memcpy(outAddress->IPAddress.Data, &addr->sin_addr.S_un.S_addr, sizeof(addr->sin_addr.S_un.S_addr));
			return true;
		}
		case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
		{
			*outAddress = {};
			const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(aAddress);
			outAddress->IPAddress.Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6;
			outAddress->Port = ntohs(addr->sin6_port);
			memcpy(outAddress->IPAddress.Data, &addr->sin6_addr.u.Byte, sizeof(addr->sin6_addr.u.Byte));
			return true;
		}
		default: return false;
		}
	}
	bool GetAddress(SocketAddress* outAddress, const sockaddr* aAddress, size_t aAddressSize, NETWORKING_ADDRESS_FAMILY aFamily) noexcept
	{
		if (aAddress->sa_family != AF_UNSPEC && GetAddressFamily(aAddress->sa_family) != aFamily)	// address family is not unspecified or the same.
		{
			return false;
		}

		switch (aFamily)
		{
		case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
		{
			if (aAddressSize < sizeof(sockaddr_in))
			{
				return false;
			}
			*outAddress = {};
			const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(aAddress);
			outAddress->IPAddress.Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4;
			outAddress->Port = ntohs(addr->sin_port);
			memcpy(outAddress->IPAddress.Data, &addr->sin_addr.S_un.S_addr, sizeof(addr->sin_addr.S_un.S_addr));
			return true;
		}
		case NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
		{
			if (aAddressSize < sizeof(sockaddr_in6))
			{
				return false;
			}
			*outAddress = {};
			const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(aAddress);
			outAddress->IPAddress.Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6;
			outAddress->Port = ntohs(addr->sin6_port);
			memcpy(outAddress->IPAddress.Data, &addr->sin6_addr.u.Byte, sizeof(addr->sin6_addr.u.Byte));
			return true;
		}
		default: return false;
		}
	}
}

#undef __RETURN_CASE
#undef __DEFAULT_RETURN_CASE
