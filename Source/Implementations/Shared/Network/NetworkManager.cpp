#include "Shared.pch.h"
#include "NetworkManager.h"

#include <Core/CommonUtilities/Serialization/MemoryWrapper.hpp>

namespace shared
{
	NBS_DEFINE_TYPE(
		NetworkManager::MessageHeader,
		Assured
	);

	NBS_DEFINE_TYPE(
		NetworkManager::NetworkMessage,
		Header,
		ID,
		Data
	);

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

	bool NetworkManager::Initialize(size_t aMaxMessageSize)
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

	bool NetworkManager::Bind(networking::Port aPort)
	{
		networking::SocketAddress address = networking::SocketAddress{
			.Port = aPort,
			.IPAddress = networking::NETWORKING_IPV4_ADDRESS_ANY
		};
		if (not mySocket->Bind(address))
		{
			return false;
		}
		return true;
	}

	void NetworkManager::Update(size_t aMaxRecieve)
	{
		CommonUtilities::Serializer serializer;

		networking::SocketAddress address;
		void* messageData;
		size_t messageDataSize;

		size_t numMessages = 0;
		while (numMessages++ < aMaxRecieve && mySocket->RecieveFrom(&address, &messageData, &messageDataSize))
		{
			if (not CheckHash(messageData, messageDataSize))
			{
				//TODO: Log, failed to read message hash.
				continue;
			}

			CommonUtilities::MemoryReader reader(messageData, messageDataSize);
			reader.SetCursor(sizeof(unsigned int));	// To skip hash

			NetworkMessage message;
			if (not serializer.Read(message, reader, 4096))
			{
				//TODO: Log failed to read header.
				continue;
			}

			const MessageID id = message.ID.c_str();

			auto it = myCallbacks.find(id);
			if (it == myCallbacks.end())
			{
				//TODO: Log, no callback found.
				continue;
			}

			const CallbackEntry& entry = it->second;
			entry.Callback(message.Data.data(), message.Data.size(), address, entry.UserCallback, entry.UserData);
		}
	}
	bool NetworkManager::SendTo(const NetworkMessage& aMessage, const networking::SocketAddress& aRemote)
	{
		CommonUtilities::MemoryWriter writer(myBuffer.data(), myBuffer.size());
		CommonUtilities::Serializer serializer;

		writer.SetCursor(sizeof(unsigned int));

		if (not serializer.Write(aMessage, writer))
		{
			return false;
		}

		const size_t messageSize = writer.GetCursor();

		const unsigned int hash = CreateHash(myBuffer.data() + sizeof(unsigned int), messageSize - sizeof(unsigned int));

		writer.SetCursor(0);
		if (not serializer.Write(hash, writer))
		{
			return false;
		}

		return mySocket->SendTo(aRemote, myBuffer.data(), messageSize);
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
