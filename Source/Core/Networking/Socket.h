#pragma once
#include "Defines.h"
#include "NetworkObject.h"
#include "SocketAddress.h"
#include <mutex>

namespace networking
{
	class Socket : public NetworkObject
	{
	public:
		virtual ~Socket();

		static bool CreateSocket(
			Socket** outSocket,
			NETWORKING_ADDRESS_FAMILY aAddressFamily,
			NETWORKING_SOCKET_TYPE aSocketType,
			NETWORKING_INTERNET_PROTOCOL aInternetProtocol,
			size_t aBufferSize
		);

		virtual bool Connect(const SocketAddress& aAddress, unsigned int aTimeout = 0) = 0;
		virtual bool Accept(Socket** outSocket, size_t aBufferSize) = 0;
		virtual bool Bind(const SocketAddress& aAddress) = 0;
		virtual bool Listen(unsigned int aBacklog) = 0;

		virtual bool Send(const void* aMessage, size_t aSize) = 0;
		virtual bool SendTo(const SocketAddress& aAddress, const void* aMessage, size_t aSize) = 0;

		virtual bool Recieve(void** outMessage, size_t* outSize) = 0;
		virtual bool RecieveFrom(SocketAddress* outAddress, void** outMessage, size_t* outSize, bool* outConnReset = nullptr) = 0;

		bool IsConnected() const noexcept;
		bool IsBound() const noexcept;

		const SocketAddress& GetLocal() const noexcept;
		const SocketAddress& GetRemote() const noexcept;

		void AddRef() noexcept override;
		void Release() noexcept override;
		unsigned int GetRefCount() const noexcept override;

		NETWORKING_ADDRESS_FAMILY GetAddressFamily() const noexcept;
		NETWORKING_SOCKET_TYPE GetSocketType() const noexcept;
		NETWORKING_INTERNET_PROTOCOL GetInternetProtocol() const noexcept;

		size_t GetBufferSize() const noexcept;

	protected:
		Socket(
			NETWORKING_ADDRESS_FAMILY aAddressFamily,
			NETWORKING_SOCKET_TYPE aSocketType,
			NETWORKING_INTERNET_PROTOCOL aInternetProtocol,
			size_t aBufferSize
		);

		void SetLocal(const SocketAddress& aLocal);
		void SetRemote(const SocketAddress& aRemote);

		std::mutex& GetMutex() noexcept;

		char* GetBuffer() const noexcept;

	private:
		NETWORKING_ADDRESS_FAMILY myAddressFamily;
		NETWORKING_SOCKET_TYPE mySocketType;
		NETWORKING_INTERNET_PROTOCOL myInternetProtocol;

		std::mutex myMutex;

		SocketAddress myRemote;
		SocketAddress myLocal;

		unsigned int myRefCount;
		char* myBuffer;
		size_t myBufferSize;
	};
}