#include "Networking.pch.h"
#include "Socket_Win32.h"
#include "Common.h"

#define FILE_CATEGORY "Socket_Win32.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Socket_Win32"

namespace networking
{
	//enum _SOCKET_OVERLAPPED_OPERATION_TYPE
	//{
	//	SOCKET_OVERLAPPED_OPERATION_SEND,
	//	SOCKET_OVERLAPPED_OPERATION_SEND_TO,
	//	SOCKET_OVERLAPPED_OPERATION_RECIEVE,
	//	SOCKET_OVERLAPPED_OPERATION_RECIEVE_FROM,
	//};
	//using SOCKET_OVERLAPPED_OPERATION_TYPE = unsigned char;

	//struct _OverlappedOperation
	//{
	//	HANDLE Event;

	//	Socket_Win32* Socket;
	//	SOCKET InternalSocket;
	//	std::mutex InternalMutex;
	//	void* InternalBuffer;
	//	size_t InternalBufferSize;
	//	
	//	SOCKET_OVERLAPPED_OPERATION_TYPE Type;
	//	unsigned int Timeout;
	//	unsigned int CheckInterval;
	//	Socket::AsyncCallback Callback;
	//	SocketAddress Remote;
	//	void* UserData;
	//};

	//static void DispatchSendAsyncCallback(const _OverlappedOperation& aOverlappedOperation)
	//{
	//	Socket::CallbackData data = {
	//		.Socket = aOverlappedOperation.Socket,
	//		.OperationFinished = true,
	//		.Data = nullptr,
	//		.DataSize = 0,
	//		.RemoteAddress = {},	//TODO: add Socket::GetRemote and pass it here for completeness.
	//		.UserData = aOverlappedOperation.UserData
	//	};
	//	(aOverlappedOperation.Callback)(&data);
	//}

	//static void DispatchSendToAsyncCallback(const _OverlappedOperation& aOverlappedOperation)
	//{
	//	Socket::CallbackData data = {
	//		.Socket = aOverlappedOperation.Socket,
	//		.OperationFinished = true,
	//		.Data = nullptr,
	//		.DataSize = 0,
	//		.RemoteAddress = aOverlappedOperation.Remote,
	//		.UserData = aOverlappedOperation.UserData
	//	};
	//	(aOverlappedOperation.Callback)(&data);
	//}

	//static void DispatchRecieveAsyncCallback(const _OverlappedOperation& aOverlappedOperation)
	//{
	//	std::scoped_lock lk(aOverlappedOperation.InternalMutex);

	//	WSABUF buffer = {
	//		.len = static_cast<ULONG>(aOverlappedOperation.InternalBufferSize),
	//		.buf = reinterpret_cast<char*>(aOverlappedOperation.InternalBuffer)
	//	};
	//	WSARecv(aOverlappedOperation.InternalSocket, &buffer, 1, NULL, )

	//	Socket::CallbackData data = {
	//		.Socket = aOverlappedOperation.Socket,
	//		.OperationFinished = true,
	//		.Data = nullptr,
	//		.DataSize = 0,
	//		.RemoteAddress = aOverlappedOperation.Remote,
	//		.UserData = aOverlappedOperation.UserData
	//	};
	//	(aOverlappedOperation.Callback)(&data);
	//}

	//static void WaitAsyncForOverlappedOperation(_OverlappedOperation aOverlappedOperation)
	//{
	//	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	//	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < aOverlappedOperation.Timeout)
	//	{
	//		DWORD result = WaitForSingleObject(aOverlappedOperation.Event, aOverlappedOperation.CheckInterval);
	//		if (result != WAIT_TIMEOUT)
	//		{
	//			if (result == WAIT_OBJECT_0)
	//			{
	//				switch (aOverlappedOperation.Type)
	//				{
	//				case SOCKET_OVERLAPPED_OPERATION_SEND:
	//					DispatchSendAsyncCallback(aOverlappedOperation);
	//					break;
	//				}
	//				
	//			}
	//			else
	//			{

	//			}
	//			break;
	//		}
	//	}
	//	aOverlappedOperation.Socket->Release();
	//}



	Socket_Win32::Socket_Win32(
		SOCKET aSocket,
		NETWORKING_ADDRESS_FAMILY aAddressFamily,
		NETWORKING_SOCKET_TYPE aSocketType,
		NETWORKING_INTERNET_PROTOCOL aInternetProtocol,
		size_t aBufferSize
	) :
		mySocket(aSocket),
		Socket(aAddressFamily, aSocketType, aInternetProtocol, aBufferSize)
	{
		UpdateLocal();
		UpdateRemote();
	}
	Socket_Win32::Socket_Win32(
		SOCKET aSocket,
		NETWORKING_ADDRESS_FAMILY aAddressFamily,
		NETWORKING_SOCKET_TYPE aSocketType,
		NETWORKING_INTERNET_PROTOCOL aInternetProtocol,
		const SocketAddress& aLocal,
		const SocketAddress& aRemote,
		size_t aBufferSize
	) :
		mySocket(aSocket),
		Socket(aAddressFamily, aSocketType, aInternetProtocol, aBufferSize)
	{
		SetLocal(aLocal);
		SetRemote(aRemote);
	}
	Socket_Win32::~Socket_Win32()
	{
		if (mySocket != INVALID_SOCKET)
		{
			closesocket(mySocket);
		}
	}
	bool Socket_Win32::Connect(const SocketAddress& aAddress, unsigned int aTimeout)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Connect"

		if (IsConnected())
		{
			return false;
		}

		sockaddr_storage address = {};
		if (not GetWs2Address(&address, aAddress))	// Could not convert address
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to connect! Could not convert SocketAddress!");
			return false;
		}

		int result;
		{
			std::scoped_lock lk(GetMutex());

			result = WSAConnect(mySocket, reinterpret_cast<sockaddr*>(&address), sizeof(address), NULL, NULL, NULL, NULL);
			if (result == SOCKET_ERROR)
			{
				int connError = WSAGetLastError();
				if (connError == WSAEWOULDBLOCK)
				{
					fd_set setW = {};
					fd_set setE = {};

					FD_SET(mySocket, &setW);
					FD_SET(mySocket, &setE);

					timeval timeOut = {};
					unsigned int seconds = aTimeout / 1000u;
					timeOut.tv_sec = seconds;
					timeOut.tv_usec = (aTimeout - seconds * 1000u) * 1000u;

					result = select(0, NULL, &setW, &setE, &timeOut);
					if (result == 0)
					{
						// select timed out. (Socket is still connecting, so it is closed).
						closesocket(mySocket);
						mySocket = NULL;
						return false;
					}
					else if (result == SOCKET_ERROR)
					{
						LOG_ERROR_L(LOG_CATEGORY, "Failed to wait for Connect! 'select' returned socket error. Error: %s", GetWinsockErrorMessage(WSAGetLastError()));
						return false;
					}
					if (FD_ISSET(mySocket, &setE))
					{
						int error = 0;
						int errorLen = sizeof(error);
						result = getsockopt(mySocket, SOL_SOCKET, SO_ERROR, (char*)&error, &errorLen);
						if (result == SOCKET_ERROR)
						{
							LOG_ERROR_L(LOG_CATEGORY, "Failed to connect! Socket was set in an error state. Error code unavailable as 'getsockopt' returned socket error. Error: %s", GetWinsockErrorMessage(WSAGetLastError()));
							return false;
						}
						LOG_ERROR_L(LOG_CATEGORY, "Failed to connect! Error: %s", GetWinsockErrorMessage(error));
						return false;
					}
				}
				else
				{
					LOG_ERROR_L(LOG_CATEGORY, "Failed to connect! 'WSAConnect' returned socket error. Error: %s", GetWinsockErrorMessage(connError));
					return false;
				}

			}

			UpdateRemote();
		}

		return true;
#undef LOG_CATEGORY
	}
	//	bool Socket_Win32::ConnectAsync(const SocketAddress& aAddress, unsigned int aTimeout, AsyncCallback aCallback, void* aUserData)
	//	{
	//#define LOG_CATEGORY CLASS_CATEGORY"::ConnectAsync"
	//
	//		std::thread(ConnectSocketAsync, this, aAddress, aTimeout, aCallback, aUserData).detach();
	//
	//		return true;
	//
	//#undef LOG_CATEGORY
	//	}
	bool Socket_Win32::Bind(const SocketAddress& aAddress)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Bind"

		if (IsBound())
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to bind! Socket is already bound.");
			return false;
		}

		sockaddr_storage address = {};
		if (not GetWs2Address(&address, aAddress))	// Could not convert address
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to bind! Could not convert SocketAddress.");
			return false;
		}

		int result;
		int error = NULL;
		{
			std::scoped_lock lk(GetMutex());
			result = bind(mySocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
			if (result == SOCKET_ERROR)
			{
				error = WSAGetLastError();
			}
			UpdateLocal();
		}
		if (result == SOCKET_ERROR)	// Failed to bind socket.
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to bind! 'bind' returned socket error. Error: %s", GetWinsockErrorMessage(error));
			return false;
		}

		return true;
#undef LOG_CATEGORY
	}
	bool Socket_Win32::Listen(unsigned int aBacklog)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Listen"

		if (not IsBound())
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to listen! Socket is not bound.");
			return false;
		}

		int result;
		{
			std::scoped_lock lk(GetMutex());
			result = listen(mySocket, static_cast<int>(aBacklog));
		}
		if (result == SOCKET_ERROR)
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to listen! 'listen' returned socket error. Error: %s", GetWinsockErrorMessage(WSAGetLastError()));
			return false;
		}
		return true;
#undef LOG_CATEGORY
	}
	bool Socket_Win32::Accept(Socket** outSocket, size_t aBufferSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Accept"
		//TODO: assert socket is listening.

		sockaddr_storage remoteAddress = {};
		int addressSize = sizeof(remoteAddress);
		SOCKET remote;
		{
			std::scoped_lock lk(GetMutex());
			remote = WSAAccept(
				mySocket,
				reinterpret_cast<sockaddr*>(&remoteAddress),
				&addressSize,
				NULL,
				NULL
			);
		}

		if (remote == INVALID_SOCKET)	// Remote is invalid
		{
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				return false;
			}
			LOG_ERROR_L(LOG_CATEGORY, "Failed to accept! 'accept' returned invalid socket. Error: %s", GetWinsockErrorMessage(error));
			return false;
		}

		SocketAddress address = {};
		if (not GetAddress(&address, reinterpret_cast<sockaddr*>(&remoteAddress), addressSize, GetAddressFamily()))	// Failed to get address
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to accept! Could not convert remote SocketAddress!");
			closesocket(remote);
			return false;
		}

		(*outSocket) = new Socket_Win32(remote, GetAddressFamily(), GetSocketType(), GetInternetProtocol(), aBufferSize);
		(*outSocket)->AddRef();

		return true;
#undef LOG_CATEGORY
	}
	bool Socket_Win32::Send(const void* aMessage, size_t aSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Send"

		int result;
		int error = NULL;
		{
			std::scoped_lock lk(GetMutex());
			result = send(mySocket, reinterpret_cast<const char*>(aMessage), static_cast<int>(aSize), NULL);
			if (result == SOCKET_ERROR)
			{
				error = WSAGetLastError();
			}
			UpdateLocal();
		}
		if (result == SOCKET_ERROR)
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to send! 'send' returned socket error. Error: %s", GetWinsockErrorMessage(error));
			return false;
		}
		return true;
#undef LOG_CATEGORY
	}
	bool Socket_Win32::SendTo(const SocketAddress& aAddress, const void* aMessage, size_t aSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SendTo"

		sockaddr_storage address = {};
		if (not GetWs2Address(&address, aAddress))	// Could not convert address
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to send! Could not convert SocketAddress!");
			return false;
		}

		int result;
		int error = NULL;
		{
			std::scoped_lock lk(GetMutex());
			result = sendto(
				mySocket,
				reinterpret_cast<const char*>(aMessage),
				static_cast<int>(aSize),
				NULL,
				reinterpret_cast<sockaddr*>(&address),
				sizeof(address)
			);
			if (result == SOCKET_ERROR)
			{
				error = WSAGetLastError();
			}
			UpdateLocal();
		}
		if (result == SOCKET_ERROR)
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to send! 'send' returned socket error. Error: %s", GetWinsockErrorMessage(error));
			return false;
		}
		return true;
#undef LOG_CATEGORY
	}
	bool Socket_Win32::Recieve(void** outMessage, size_t* outSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Recieve"

		int result;
		int error = NULL;
		{
			std::scoped_lock lk(GetMutex());

			result = recv(mySocket, GetBuffer(), static_cast<int>(GetBufferSize()), NULL);
			if (result == SOCKET_ERROR)
			{
				error = WSAGetLastError();
			}
			UpdateLocal();
		}

		if (result == SOCKET_ERROR)
		{
			*outMessage = nullptr;
			*outSize = 0;

			LOG_ERROR_L(LOG_CATEGORY, "Failed to recieve! 'recv' returned socket error. Error: %s", GetWinsockErrorMessage(error));
			return false;
		}

		(*outMessage) = GetBuffer();
		*outSize = result;

		return true;
#undef LOG_CATEGORY
	}
	bool Socket_Win32::RecieveFrom(SocketAddress* outAddress, void** outMessage, size_t* outSize, bool* outConnReset)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::RecieveFrom"

		if (not IsBound())
		{
			LOG_ERROR_L(LOG_CATEGORY, "Failed to recieve! Socket was unbound.");
			return false;
		}

		sockaddr_storage remote = {};
		int remoteSize = static_cast<int>(sizeof(remote));

		int result;
		int error = NULL;
		{
			std::scoped_lock lk(GetMutex());

			WSABUF buffer = {
				.len = static_cast<ULONG>(GetBufferSize()),
				.buf = GetBuffer()
			};

			DWORD recievedSize = 0;
			DWORD flags = NULL;
			result = WSARecvFrom(mySocket, &buffer, 1, &recievedSize, &flags, reinterpret_cast<sockaddr*>(&remote), &remoteSize, NULL, NULL);
			if (result == SOCKET_ERROR)
			{
				error = WSAGetLastError();
			}
			UpdateLocal();

			*outSize = static_cast<size_t>(recievedSize);
		}

		if (result == SOCKET_ERROR)
		{
			*outMessage = nullptr;
			*outSize = 0;

			if (error == WSAECONNRESET)
			{
				if (outConnReset != nullptr)
				{
					*outConnReset = true;
				}
			}
			else if (error != WSAEWOULDBLOCK)
			{
				LOG_ERROR_L(LOG_CATEGORY, "Failed to recieve! 'recvfrom' returned socket error. Error: %s", GetWinsockErrorMessage(error));
			}
			return false;
		}

		SocketAddress remoteAddress = {};
		if (not GetAddress(&remoteAddress, reinterpret_cast<sockaddr*>(&remote), remoteSize, GetAddressFamily()))
		{
			*outMessage = nullptr;
			*outSize = 0;

			LOG_ERROR_L(LOG_CATEGORY, "Failed to recieve! Could not convert remote SocketAddress!");
			return false;
		}

		(*outMessage) = GetBuffer();
		*outAddress = remoteAddress;

		return true;
#undef LOG_CATEGORY
	}
	void Socket_Win32::UpdateLocal()
	{
		if (not IsBound())
		{
			sockaddr_storage addr = {};
			int size = sizeof(addr);
			int result = getsockname(mySocket, (sockaddr*)&addr, &size);
			if (result != SOCKET_ERROR)
			{
				SocketAddress local = {};
				if (GetAddress(&local, (sockaddr*)&addr, size, GetAddressFamily()))
				{
					SetLocal(local);
				}
			}
		}
	}
	void Socket_Win32::UpdateRemote()
	{
		if (not IsConnected())
		{
			sockaddr_storage addr = {};
			int size = sizeof(addr);
			int result = getpeername(mySocket, (sockaddr*)&addr, &size);
			if (result != SOCKET_ERROR)
			{
				SocketAddress local = {};
				if (GetAddress(&local, (sockaddr*)&addr, size, GetAddressFamily()))
				{
					SetLocal(local);
				}
			}
		}
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
