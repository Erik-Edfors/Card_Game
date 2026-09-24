#pragma once
#include "../Socket.h"
#include "../SocketAddress.h"
#include <WinSock2.h>

namespace networking
{
	class Socket_Win32 : public Socket
	{
	public:
		Socket_Win32(
			SOCKET aSocket,
			NETWORKING_ADDRESS_FAMILY aAddressFamily,
			NETWORKING_SOCKET_TYPE aSocketType,
			NETWORKING_INTERNET_PROTOCOL aInternetProtocol,
			size_t aBufferSize
		);
		Socket_Win32(
			SOCKET aSocket,
			NETWORKING_ADDRESS_FAMILY aAddressFamily,
			NETWORKING_SOCKET_TYPE aSocketType,
			NETWORKING_INTERNET_PROTOCOL aInternetProtocol,
			const SocketAddress& aLocal,
			const SocketAddress& aRemote,
			size_t aBufferSize
		);
		~Socket_Win32();

		bool Connect(const SocketAddress& aAddress, unsigned int aTimeout = 0) override;
		bool Accept(Socket** outSocket, size_t aBufferSize) override;
		bool Bind(const SocketAddress& aAddress) override;
		bool Listen(unsigned int aBacklog) override;

		bool Send(const void* aMessage, size_t aSize) override;		
		bool SendTo(const SocketAddress& aAddress, const void* aMessage, size_t aSize) override;
		
		bool Recieve(void** outMessage, size_t* outSize) override;		
		bool RecieveFrom(SocketAddress* outAddress, void** outMessage, size_t* outSize, bool* outConnReset = nullptr) override;

	private:
		void UpdateLocal();
		void UpdateRemote();

		SOCKET mySocket;
	};
}