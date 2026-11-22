#include "Shared.pch.h"
#include "NetworkManager.h"
#include "Messages.h"

#include <Core/CommonUtilities/Serialization/MemoryWrapper.hpp>

#define FILE_CATEGORY "NetworkManager.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->NetworkManager"

namespace shared
{
	NBS_DEFINE_TYPE(
		NetworkManager::MessageHeader,
		Index,
		Assured
	);

	NBS_DEFINE_TYPE(
		NetworkManager::NetworkMessage,
		Header,
		ID,
		Data
	);

	NetworkManager::NetworkManager() :
		mySocket(nullptr),
		myNextMessageIndex(1u),
		myAssuredTimeoutCallback(nullptr),
		myAssuredTimeoutCallbackUserData(nullptr)
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
#define LOG_CATEGORY CLASS_CATEGORY"::Bind"

		networking::SocketAddress address = networking::SocketAddress{
			.Port = aPort,
			.IPAddress = networking::NETWORKING_IPV4_ADDRESS_ANY
		};
		if (not mySocket->Bind(address))
		{
			LOG_WARNING(LOG_CATEGORY, "Failed to bind socket! Port '%u'", aPort);
			return false;
		}
		return true;

#undef LOG_CATEGORY
	}

	bool NetworkManager::IsBound() const
	{
		return mySocket->IsBound();
	}

	void NetworkManager::Update(size_t aMaxRecieve)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Update"

		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		// Can only recieve messages if bound, and is bound automatically after sending first message.
		// This means that no communication can happen before IsBound is true.
		if (IsBound())
		{
			networking::SocketAddress remote;
			void* messageData;
			size_t messageDataSize;

			size_t numMessages = 0;
			while (numMessages++ < aMaxRecieve && mySocket->RecieveFrom(&remote, &messageData, &messageDataSize))
			{
				if (not CheckHash(messageData, messageDataSize))
				{
					LOG_WARNING(LOG_CATEGORY, "A message failed checksum.");
					continue;
				}

				NetworkMessage message;
				{
					CommonUtilities::Serializer serializer;
					CommonUtilities::MemoryReader reader(messageData, messageDataSize);
					reader.SetCursor(sizeof(unsigned int));	// To skip hash

					if (not serializer.Read(message, reader, 4096))
					{
						LOG_WARNING(LOG_CATEGORY, "Failed to read message header!");
						continue;
					}
				}

				const MessageID id = message.ID.c_str();

				// Recieve acknowladgement of assured messages, and remove them.
				if (id == NM_AcknowladgeAssured::ID)
				{
					CommonUtilities::Serializer serializer;
					CommonUtilities::MemoryReader reader(message.Data.data(), message.Data.size());

					NM_AcknowladgeAssured acknowladgment = {};
					if (serializer.Read(acknowladgment, reader, 4096))
					{
						auto it = mySentAssuredMessages.find(acknowladgment.Index);
						if (it != mySentAssuredMessages.end() && it->second.Remote == remote)
						{
							LOG_INFO(LOG_CATEGORY, "Recieved Acknowladgement. %u", acknowladgment.Index);
							mySentAssuredMessages.erase(it);
						}
					}
				}

				bool alreadyRecieved = false;
				if (message.Header.Assured)
				{
					const NM_AcknowladgeAssured acknowladgement = {
							.Index = message.Header.Index
					};
					NetworkMessage acknowladgeMessage;
					if (CreateMessage(&acknowladgeMessage, acknowladgement, false))
					{
						if (SendTo(acknowladgeMessage, remote))
						{
							LOG_INFO(LOG_CATEGORY, "Sent acknowladgement for message %u.", message.Header.Index);
						}
						else
						{
							LOG_WARNING(LOG_CATEGORY, "Failed to send acknowladgement for message %u.", message.Header.Index);
						}
					}
					else
					{
						LOG_WARNING(LOG_CATEGORY, "Failed to send acknowladgement for message %u.", message.Header.Index);
					}

					alreadyRecieved = AlreadyRecieved(remote, message.Header.Index);
					if (not alreadyRecieved)
					{
						myRecievedAssuredMessages[remote][message.Header.Index] = RecievedAssuredMessageEntry{
							.TimeFirstRecieved = now,
							.Remote = remote,
						};
					}
					else
					{
						LOG_INFO(LOG_CATEGORY, "Recieved followup assured message %u.", message.Header.Index);
					}
				}

				if (not alreadyRecieved)
				{
					auto it = myCallbacks.find(id);
					if (it != myCallbacks.end())
					{
						const CallbackEntry& entry = it->second;
						entry.Callback(message.Data.data(), message.Data.size(), remote, entry.UserCallback, entry.UserData);
					}
				}
			}

			UpdateRecieved();

			UpdateSent();
		}

#undef LOG_CATEGORY
	}
	bool NetworkManager::SendTo(const NetworkMessage& aMessage, const networking::SocketAddress& aRemote)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SendTo"

		const MessageIndex index = myNextMessageIndex++;

		NetworkMessage message = aMessage;
		message.Header.Index = index;

		const size_t messageSize = BufferMessage(message);
		if (messageSize == 0)
		{
			return false;
		}

		if (message.Header.Assured)
		{
			LOG_INFO(LOG_CATEGORY, "Sent assured %s %u to %s.", message.ID, message.Header.Index, to_string(aRemote));

			const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			mySentAssuredMessages[index] = SentAssuredMessageEntry{
				.TimeFirstSent = now,
				.TimeLastSent = now,
				.Remote = aRemote,
				.Message = std::move(message),
			};
		}

		return mySocket->SendTo(aRemote, myBuffer.data(), messageSize);

#undef LOG_CATEGORY
	}
	void NetworkManager::SetAssuredTimeoutCallback(AssuredTimeoutCallback aCallback, void* aUserData)
	{
		myAssuredTimeoutCallback = aCallback;
		myAssuredTimeoutCallbackUserData = aUserData;
	}
	bool NetworkManager::AlreadyRecieved(const networking::SocketAddress& aRemote, MessageIndex aIndex) const noexcept
	{
		auto remoteIt = myRecievedAssuredMessages.find(aRemote);
		if (remoteIt == myRecievedAssuredMessages.end())
		{
			return false;
		}
		const std::map<MessageIndex, RecievedAssuredMessageEntry>& map = remoteIt->second;

		auto it = map.find(aIndex);
		if (it == map.end())
		{
			return false;
		}

		return true;
	}
	void NetworkManager::UpdateRecieved()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateRecieved"

		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		auto remoteIt = myRecievedAssuredMessages.begin();
		while (remoteIt != myRecievedAssuredMessages.end())
		{
			std::map<MessageIndex, RecievedAssuredMessageEntry>& map = remoteIt->second;
			auto it = map.begin();
			while (it != map.end())
			{
				RecievedAssuredMessageEntry& entry = it->second;

				if (std::chrono::duration<float>(now - entry.TimeFirstRecieved).count() > 30.f)	//TODO: Use user set timeout.
				{
					it = map.erase(it);
					continue;
				}

				it = std::next(it);
			}
			if (map.empty())
			{
				remoteIt = myRecievedAssuredMessages.erase(remoteIt);
				continue;
			}
			remoteIt = std::next(remoteIt);
		}

#undef LOG_CATEGORY
	}
	void NetworkManager::UpdateSent()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateSent"

		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		auto it = mySentAssuredMessages.begin();
		while (it != mySentAssuredMessages.end())
		{
			SentAssuredMessageEntry& entry = it->second;

			if (std::chrono::duration<float>(now - entry.TimeFirstSent).count() > 30.f)	//TODO: Use user set timeout.
			{
				if (myAssuredTimeoutCallback != nullptr)
				{
					myAssuredTimeoutCallback(
						AssuredTimeoutCallbackData{
							.Remote = entry.Remote,
							.Index = entry.Message.Header.Index,
							.UserData = myAssuredTimeoutCallbackUserData
						}
					);
				}
				LOG_WARNING(LOG_CATEGORY, "Assured message %u timed out! Remote: %s.", it->first, to_string(entry.Remote));
				it = mySentAssuredMessages.erase(it);
				continue;
			}
			else if (std::chrono::duration<float>(now - entry.TimeLastSent).count() > 0.5f)
			{
				ResendAssured(entry);
				entry.TimeLastSent = now;
			}

			it = std::next(it);
		}

#undef LOG_CATEGORY
	}
	bool NetworkManager::ResendAssured(const SentAssuredMessageEntry& aEntry)
	{
		const size_t messageSize = BufferMessage(aEntry.Message);
		if (messageSize == 0)
		{
			return false;
		}

		return mySocket->SendTo(aEntry.Remote, myBuffer.data(), messageSize);
	}
	size_t NetworkManager::BufferMessage(const NetworkMessage& aMessage)
	{
		CommonUtilities::MemoryWriter writer(myBuffer.data(), myBuffer.size());
		CommonUtilities::Serializer serializer;

		writer.SetCursor(sizeof(unsigned int));

		if (not serializer.Write(aMessage, writer))
		{
			return 0;
		}

		const size_t messageSize = writer.GetCursor();

		const unsigned int hash = CreateHash(myBuffer.data() + sizeof(unsigned int), messageSize - sizeof(unsigned int));

		writer.SetCursor(0);
		if (not serializer.Write(hash, writer))
		{
			return 0;
		}

		return messageSize;
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

#undef FILE_CATEGORY
#undef CLASS_CATEGORY