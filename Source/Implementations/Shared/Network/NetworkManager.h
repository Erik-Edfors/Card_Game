#pragma once
#include "../Defines.h"
#include "MessageID.h"
#include <Core/Networking/Socket.h>
#include <Core/CommonUtilities/Serialization/Serialization.hpp>
#include <Core/CommonUtilities/Serialization/Serializer.hpp>
#include <Core/CommonUtilities/Serialization/MemoryWrapper.hpp>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace shared
{
	class NetworkManager
	{
	public:
		struct MessageHeader
		{
			MessageIndex Index;
			bool Assured;
		};

		struct NetworkMessage
		{
			MessageHeader Header;
			std::string ID;
			std::vector<char> Data;
		};

		struct AssuredTimeoutCallbackData
		{
			networking::SocketAddress Remote;
			MessageIndex Index;
			void* UserData;
		};

		using AssuredTimeoutCallback = void(*)(const AssuredTimeoutCallbackData& aData);

		template<class _MessageTy>
		struct MessageCallbackData
		{
			_MessageTy Message;
			networking::SocketAddress Address;
			void* UserData;
		};

		template<class _MessageTy>
		using MessageCallback = void(*)(const MessageCallbackData<_MessageTy>& aData);

		NetworkManager();
		~NetworkManager();

		NetworkManager(NetworkManager&&) = delete;

		bool Initialize(size_t aMaxMessageSize);
		bool Shutdown();

		bool Bind(networking::Port aPort);
		bool IsBound() const;

		void Update(size_t aMaxRecieve);

		bool SendTo(const NetworkMessage& aMessage, const networking::SocketAddress& aRemote);

		void SetAssuredTimeoutCallback(AssuredTimeoutCallback aCallback, void* aUserData);

		template<class _MessageTy>
		bool CreateMessage(NetworkMessage* outMessage, const _MessageTy& aMessage, bool aAssured);

		template<class _MessageTy>
		void SetCallback(MessageCallback<_MessageTy> aCallback, void* aUserData);

	protected:

	private:
		using UntypedCallback = void(*)(const void* aData, size_t aDataSize, const networking::SocketAddress& aRemote, void* aUserCallback, void* aUserData);

		struct CallbackEntry
		{
			UntypedCallback Callback;
			void* UserCallback;
			void* UserData;
		};

		struct SentAssuredMessageEntry
		{
			std::chrono::steady_clock::time_point TimeFirstSent;
			std::chrono::steady_clock::time_point TimeLastSent;
			networking::SocketAddress Remote;
			NetworkMessage Message;
		};

		struct RecievedAssuredMessageEntry
		{
			std::chrono::steady_clock::time_point TimeFirstRecieved;
			networking::SocketAddress Remote;
		};

		bool AlreadyRecieved(const networking::SocketAddress& aRemote, MessageIndex aIndex) const noexcept;
		void UpdateRecieved();
		void UpdateSent();

		bool ResendAssured(const SentAssuredMessageEntry& aEntry);

		size_t BufferMessage(const NetworkMessage& aMessage);

		static unsigned int CreateHash(const void* aMessage, size_t aMessageSize) noexcept;
		static bool CheckHash(const void* aMessage, size_t aMessageSize) noexcept;

		template<class _MessageTy>
		static void ProcessRecievedMessage(const void* aData, size_t aDataSize, const networking::SocketAddress& aRemote, void* aUserCallback, void* aUserData);

		MessageIndex myNextMessageIndex;

		std::map<MessageIndex, SentAssuredMessageEntry> mySentAssuredMessages;
		std::map<networking::SocketAddress, std::map<MessageIndex, RecievedAssuredMessageEntry>> myRecievedAssuredMessages;

		std::unordered_map<MessageID, CallbackEntry> myCallbacks;
		CommonUtilities::Serializer mySerializer;

		AssuredTimeoutCallback myAssuredTimeoutCallback;
		void* myAssuredTimeoutCallbackUserData;

		networking::Socket* mySocket;

		std::vector<char> myBuffer;
	};


	template<class _MessageTy>
	inline bool NetworkManager::CreateMessage(NetworkMessage* outMessage, const _MessageTy& aMessage, bool aAssured)
	{
		CommonUtilities::MemoryWriter writer(myBuffer.data(), myBuffer.size());
		CommonUtilities::Serializer serializer;

		if (not serializer.Write(aMessage, writer))
		{
			return false;
		}
		const size_t messageSize = writer.GetCursor();

		std::vector<char> data;
		data.resize(messageSize);
		memcpy(data.data(), myBuffer.data(), messageSize);

		*outMessage = NetworkMessage{
			.Header = MessageHeader{
				.Index = 0u,
				.Assured = aAssured
			},
			.ID = _MessageTy::ID.GetString(),
			.Data = std::move(data)
		};

		return true;

		//CommonUtilities::MemoryWriter writer(myBuffer.data(), myBuffer.size());
		//CommonUtilities::Serializer serializer;

		//writer.SetCursor(sizeof(unsigned int));

		//MessageHeader header = {
		//	.Assured = aAssured
		//};

		//if (not serializer.Write(header, writer))
		//{
		//	return false;
		//}

		//if (not serializer.Write(_MessageTy::ID.GetString(), writer))
		//{
		//	return false;
		//}

		//if (not serializer.Write(aMessage, writer))
		//{
		//	return false;
		//}

		//const size_t messageSize = writer.GetCursor();

		//const unsigned int hash = CreateHash(myBuffer.data() + sizeof(unsigned int), messageSize - sizeof(unsigned int));

		//writer.SetCursor(0);
		//if (not serializer.Write(hash, writer))
		//{
		//	return false;
		//}

		//outMessage->Data.resize(messageSize);
		//memcpy(outMessage->Data.data(), myBuffer.data(), messageSize);

		//return true;
	}

	template<class _MessageTy>
	inline void NetworkManager::SetCallback(MessageCallback<_MessageTy> aCallback, void* aUserData)
	{
		if (aCallback == nullptr)
		{
			myCallbacks.erase(_MessageTy::ID);
		}
		else
		{
			myCallbacks[_MessageTy::ID] = CallbackEntry{
				.Callback = &ProcessRecievedMessage<_MessageTy>,
				.UserCallback = aCallback,
				.UserData = aUserData
			};
		}
	}

	template<class _MessageTy>
	inline void NetworkManager::ProcessRecievedMessage(const void* aData, size_t aDataSize, const networking::SocketAddress& aRemote, void* aUserCallback, void* aUserData)
	{
		CommonUtilities::MemoryReader reader(aData, aDataSize);
		CommonUtilities::Serializer serializer;

		_MessageTy message = {};
		if (not serializer.Read(message, reader, 4096))
		{
			return;
		}

		MessageCallback<_MessageTy> callback = reinterpret_cast<MessageCallback<_MessageTy>>(aUserCallback);

		MessageCallbackData<_MessageTy> data = {
			.Message = std::move(message),
			.Address = aRemote,
			.UserData = aUserData
		};

		callback(data);
	}
}