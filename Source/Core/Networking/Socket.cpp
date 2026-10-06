#include "Networking.pch.h"
#include "Socket.h"
#include "SocketAddress.h"
#include "Internal/Socket_Win32.h"
#include "Internal/Common.h"
#include <string>

#define FILE_CATEGORY "Socket.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Socket"

namespace networking
{
	Socket::Socket(NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_SOCKET_TYPE aSocketType, NETWORKING_INTERNET_PROTOCOL aInternetProtocol, size_t aBufferSize) :
		myRefCount(0u),
		myAddressFamily(aAddressFamily),
		mySocketType(aSocketType),
		myInternetProtocol(aInternetProtocol),
		myBuffer(new char[aBufferSize]),
		myBufferSize(aBufferSize),
		myLocal({}),
		myRemote({})
	{
	}
	Socket::~Socket()
	{
		delete[] myBuffer;
	}

	bool Socket::CreateSocket(Socket** outSocket, NETWORKING_ADDRESS_FAMILY aAddressFamily, NETWORKING_SOCKET_TYPE aSocketType, NETWORKING_INTERNET_PROTOCOL aInternetProtocol, size_t aBufferSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateSocket"

		SOCKET sock = WSASocket(
			GetWs2AddressFamily(aAddressFamily),
			GetWs2SocketType(aSocketType),
			GetWs2InternetProtocol(aInternetProtocol),
			NULL,
			0,
			WSA_FLAG_OVERLAPPED
		);
		if (sock == INVALID_SOCKET)
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to create socket! Call to 'WSASocket' failed! Error: %s", GetWinsockErrorMessage(WSAGetLastError()));
			return false;
		}
		
		ULONG nonBlocking = 1;
		if (ioctlsocket(sock, FIONBIO, &nonBlocking) == SOCKET_ERROR)
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to create socket! Call to 'ioctlsocket' failed! Error: %s", GetWinsockErrorMessage(WSAGetLastError()));
			return false;
		}

		//SOCKET sock = socket(GetWs2AddressFamily(aAddressFamily), GetWs2SocketType(aSocketType), GetWs2InternetProtocol(aInternetProtocol));
		if (sock == INVALID_SOCKET)	//Failed to create socket.
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to create socket! Call to socket failed! Error: %s", GetWinsockErrorMessage(WSAGetLastError()));
			return false;
		}

		if (*outSocket != nullptr)
		{
			(*outSocket)->Release();
		}
		(*outSocket) = new Socket_Win32(sock, aAddressFamily, aSocketType, aInternetProtocol, aBufferSize);
		(*outSocket)->AddRef();

		return true;
#undef LOG_CATEGORY
	}
	bool Socket::IsConnected() const noexcept
	{
		return myRemote != SocketAddress{};
	}
	bool Socket::IsBound() const noexcept
	{
		return myLocal != SocketAddress{};
	}
	const SocketAddress& Socket::GetLocal() const noexcept
	{
		return myLocal;
	}
	const SocketAddress& Socket::GetRemote() const noexcept
	{
		return myRemote;
	}
	void Socket::AddRef() noexcept
	{
		std::scoped_lock lk(GetMutex());

		++myRefCount;
	}
	void Socket::Release() noexcept
	{
		bool shouldDelete = false;
		{
			std::scoped_lock lk(GetMutex());
			--myRefCount;
			shouldDelete = myRefCount == 0;
		}
		if (shouldDelete)
		{
			delete this;
		}
	}
	unsigned int Socket::GetRefCount() const noexcept
	{
		return myRefCount;
	}
	NETWORKING_ADDRESS_FAMILY Socket::GetAddressFamily() const noexcept
	{
		return myAddressFamily;
	}
	NETWORKING_SOCKET_TYPE Socket::GetSocketType() const noexcept
	{
		return mySocketType;
	}
	NETWORKING_INTERNET_PROTOCOL Socket::GetInternetProtocol() const noexcept
	{
		return myInternetProtocol;
	}
	size_t Socket::GetBufferSize() const noexcept
	{
		return myBufferSize;
	}
	void Socket::SetLocal(const SocketAddress& aLocal)
	{
		myLocal = aLocal;
	}
	void Socket::SetRemote(const SocketAddress& aRemote)
	{
		myRemote = aRemote;
	}
	std::mutex& Socket::GetMutex() noexcept
	{
		return myMutex;
	}
	char* Socket::GetBuffer() const noexcept
	{
		return myBuffer;
	}
}

#undef CLASS_CATEGORY
#undef FILE_CATEGORY
