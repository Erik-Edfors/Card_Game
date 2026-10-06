#pragma once
#include "Defines.h"
#include <string>

namespace networking
{
	struct IPAddress
	{
	public:
		NETWORKING_ADDRESS_FAMILY Family;
		unsigned char Data[16];
	};

	struct SocketAddress
	{
		Port Port;
		IPAddress IPAddress;
	};

	std::string to_string(const SocketAddress& aAddress);

	bool GetSocketAddress(SocketAddress* outAddress, std::string_view aHostName, std::string_view aServiceName, NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_SOCKET_TYPE aSocketType, NETWORKING_INTERNET_PROTOCOL aProtocol);
	bool GetSocketAddress(SocketAddress* outAddress, std::wstring_view aHostName, std::wstring_view aServiceName, NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_SOCKET_TYPE aSocketType, NETWORKING_INTERNET_PROTOCOL aProtocol);

	inline constexpr IPAddress NETWORKING_IPV4_ADDRESS_ANY = IPAddress{ .Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4, .Data = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	inline constexpr IPAddress NETWORKING_IPV6_ADDRESS_ANY = IPAddress{ .Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6, .Data = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

	inline constexpr IPAddress NETWORKING_IPV4_ADDRESS_BROADCAST = IPAddress{ .Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4, .Data = { 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

	inline constexpr IPAddress NETWORKING_IPV4_LOOPBACK = IPAddress{ .Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4, .Data = { 127, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	inline constexpr IPAddress NETWORKING_IPV6_LOOPBACK = IPAddress{ .Family = NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6, .Data = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } };

	inline constexpr bool operator==(const IPAddress& aLHS, const IPAddress& aRHS) noexcept
	{
		if (aLHS.Family != aRHS.Family)
		{
			return false;
		}
		size_t dataSize = 16;
		switch (aLHS.Family)
		{
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
			dataSize = 4;
			break;
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
			dataSize = 16;
			break;
		}
		for (size_t i = 0; i < dataSize; i++)
		{
			if (aLHS.Data[i] != aRHS.Data[i])
			{
				return false;
			}
		}

		return true;
	}
	inline constexpr bool operator!=(const IPAddress& aLHS, const IPAddress& aRHS) noexcept
	{
		if (aLHS.Family != aRHS.Family)
		{
			return true;
		}
		size_t dataSize = 16;
		switch (aLHS.Family)
		{
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
			dataSize = 4;
			break;
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
			dataSize = 16;
			break;
		}
		for (size_t i = 0; i < dataSize; i++)
		{
			if (aLHS.Data[i] != aRHS.Data[i])
			{
				return true;
			}
		}

		return false;
	}
	inline constexpr bool operator<(const IPAddress& aLHS, const IPAddress& aRHS) noexcept
	{
		if (aLHS.Family < aRHS.Family)
		{
			return true;
		}
		else if (aLHS.Family > aRHS.Family)
		{
			return false;
		}

		size_t dataSize = 16;
		switch (aLHS.Family)
		{
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
			dataSize = 4;
			break;
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
			dataSize = 16;
			break;
		}

		for (size_t i = 0; i < dataSize; i++)
		{
			if (aLHS.Data[i] < aRHS.Data[i])
			{
				return true;
			}
			else if (aLHS.Data[i] > aRHS.Data[i])
			{
				return false;
			}
		}

		return false;
	}
	inline constexpr bool operator>(const IPAddress& aLHS, const IPAddress& aRHS) noexcept
	{
		if (aLHS.Family > aRHS.Family)
		{
			return true;
		}
		else if (aLHS.Family < aRHS.Family)
		{
			return false;
		}

		size_t dataSize = 16;
		switch (aLHS.Family)
		{
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
			dataSize = 4;
			break;
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
			dataSize = 16;
			break;
		}

		for (size_t i = 0; i < dataSize; i++)
		{
			if (aLHS.Data[i] > aRHS.Data[i])
			{
				return true;
			}
			else if (aLHS.Data[i] < aRHS.Data[i])
			{
				return false;
			}
		}

		return false;
	}
	inline constexpr bool operator==(const SocketAddress& aLHS, const SocketAddress& aRHS) noexcept
	{
		return aLHS.Port == aRHS.Port && aLHS.IPAddress == aRHS.IPAddress;
	}
	inline constexpr bool operator!=(const SocketAddress& aLHS, const SocketAddress& aRHS) noexcept
	{
		return aLHS.Port != aRHS.Port || aLHS.IPAddress != aRHS.IPAddress;
	}
	inline constexpr bool operator<(const SocketAddress& aLHS, const SocketAddress& aRHS) noexcept
	{
		if (aLHS.IPAddress < aRHS.IPAddress)
		{
			return true;
		}
		else if (aLHS.IPAddress > aRHS.IPAddress)
		{
			return false;
		}

		if (aLHS.Port < aRHS.Port)
		{
			return true;
		}
		else if (aLHS.Port > aRHS.Port)
		{
			return false;
		}

		return false;
	}
	inline constexpr bool operator>(const SocketAddress& aLHS, const SocketAddress& aRHS) noexcept
	{
		if (aLHS.IPAddress > aRHS.IPAddress)
		{
			return true;
		}
		else if (aLHS.IPAddress < aRHS.IPAddress)
		{
			return false;
		}

		if (aLHS.Port > aRHS.Port)
		{
			return true;
		}
		else if (aLHS.Port < aRHS.Port)
		{
			return false;
		}

		return false;
	}
}

std::ostream& operator<<(std::ostream& aStream, const networking::SocketAddress& aAddress);

template<>
struct ::std::hash<networking::IPAddress>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr std::size_t operator()(const networking::IPAddress& aIPAddress) const noexcept
	{
		constexpr size_t ElementSize = sizeof(aIPAddress.Data[0]);
		constexpr size_t ElementsPerHash = sizeof(size_t) / ElementSize;

		size_t hash = static_cast<size_t>(aIPAddress.Family);
		size_t dataSize = 16;
		switch (aIPAddress.Family)
		{
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4:
			dataSize = 4;
			break;
		case networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_6:
			dataSize = 16;
			break;
		}
		for (size_t i = 0; i < dataSize; i++)
		{
			const size_t shift = i % ElementsPerHash;
			hash ^= static_cast<size_t>(aIPAddress.Data[i]) << shift * ElementSize;
		}
		return hash;
	}
};

template<>
struct ::std::hash<networking::SocketAddress>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr std::size_t operator()(const networking::SocketAddress& aSocketAddress) const noexcept
	{
		return static_cast<size_t>(aSocketAddress.Port) ^ ::std::hash<networking::IPAddress>()(aSocketAddress.IPAddress);
	}
};
