#pragma once
#include "MessageID.h"
#include <Core/Networking/Socket.h>
#include <Core/CommonUtilities/Serialization/Serialization.hpp>
#include <Core/CommonUtilities/Serialization/Serializer.hpp>
#include <Core/CommonUtilities/Serialization/MemoryWrapper.hpp>
#include <unordered_map>

namespace shared
{
	class NetworkManager
	{
	public:
		struct MessageHeader
		{
			bool Assured;
		};

		struct NetworkMessage
		{
			MessageHeader Header;
			std::string ID;
			std::vector<char> Data;
		};

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

		void Update(size_t aMaxRecieve);

		bool SendTo(const NetworkMessage& aMessage, const networking::SocketAddress& aRemote);

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

		static unsigned int CreateHash(const void* aMessage, size_t aMessageSize) noexcept;
		static bool CheckHash(const void* aMessage, size_t aMessageSize) noexcept;

		template<class _MessageTy>
		static void ProcessRecievedMessage(const void* aData, size_t aDataSize, const networking::SocketAddress& aRemote, void* aUserCallback, void* aUserData);

		std::unordered_map<MessageID, CallbackEntry> myCallbacks;
		CommonUtilities::Serializer mySerializer;

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