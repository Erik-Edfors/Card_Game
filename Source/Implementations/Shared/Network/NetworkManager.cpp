#include "Shared.pch.h"
#include "NetworkManager.h"

#include <Core/CommonUtilities/Serialization/MemoryWrapper.hpp>

namespace shared
{
	NetworkManager::NetworkManager() :
		mySocket(nullptr)
	{
	}
	NetworkManager::~NetworkManager()
	{
		if (mySocket != nullptr)
		{
			mySocket->Release();
			mySocket = nullptr;
		}
	}

	bool NetworkManager::Initialize(size_t aMaxMessageSize, networking::Port aPort)
	{
		if (not networking::Socket::CreateSocket(
			&mySocket,
			networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4,
			networking::NETWORKING_SOCKET_TYPE_DGRAM,
			networking::NETWORKING_INTERNET_PROTOCOL_UDP,
			aMaxMessageSize
		))
		{
			return false;
		}

		networking::SocketAddress address = networking::SocketAddress{
			.Port = aPort,
			.IPAddress = networking::NETWORKING_IPV4_ADDRESS_ANY
		};
		if (not mySocket->Bind(address))
		{
			return false;
		}

		myBuffer.resize(aMaxMessageSize);

		return true;
	}
	bool NetworkManager::Shutdown()
	{
		if (mySocket != nullptr)
		{
			mySocket->Release();
			mySocket = nullptr;
		}
		return true;
	}

	void NetworkManager::Update(size_t aMaxRecieve)
	{
		CommonUtilities::Serializer serializer;

		networking::SocketAddress address;
		void* message;
		size_t messageSize;

		size_t numMessages = 0;
		while (numMessages++ < aMaxRecieve && mySocket->RecieveFrom(&address, &message, &messageSize))
		{
			if (not CheckHash(message, messageSize))
			{
				//TODO: Log, failed to read message hash.
				continue;
			}

			CommonUtilities::MemoryReader reader(message, messageSize);
			reader.SetCursor(sizeof(unsigned int));	// To skip hash

			MessageHeader header;
			if (not serializer.Read(header, reader, sizeof(header)))
			{
				//TODO: Log failed to read header.
				continue;
			}

			std::string rawID;
			if (not serializer.Read(rawID, reader, 64))
			{
				//TODO: Log, failed to read message id.
				continue;
			}
			const MessageID id = rawID.c_str();

			auto it = myCallbacks.find(id);
			if (it == myCallbacks.end())
			{
				//TODO: Log, no callback found.
				continue;
			}

			const CallbackEntry& entry = it->second;
			entry.Callback(reinterpret_cast<char*>(message) + reader.GetCursor(), messageSize - reader.GetCursor(), address, entry.UserCallback, entry.UserData);
		}
	}
	bool NetworkManager::SendTo(const NetworkMessage& aMessage, const networking::SocketAddress& aRemote)
	{
		return mySocket->SendTo(aRemote, aMessage.Data.data(), aMessage.Data.size());
	}
	unsigned int NetworkManager::CreateHash(const void* aMessage, size_t aMessageSize) noexcept
	{
		return CommonUtilities::CRC32(reinterpret_cast<const char*>(aMessage), aMessageSize);
	}
	bool NetworkManager::CheckHash(const void* aMessage, size_t aMessageSize) noexcept
	{
		constexpr size_t hashSize = sizeof(unsigned int);

		if (aMessageSize < hashSize)
		{
			//Message is too short to contain a hash.
			return false;
		}

		unsigned int hash = *reinterpret_cast<const unsigned int*>(aMessage);

		const unsigned int expectedHash = CommonUtilities::CRC32(reinterpret_cast<const char*>(aMessage) + hashSize, aMessageSize - hashSize);

		return expectedHash == hash;
	}
}
