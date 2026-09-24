#include "Networking.pch.h"
#include "Networking.h"
#include <iostream>

namespace networking
{
	static WSAData locData;

	bool Startup()
	{
		logger::Logger::SetOutputStream(&std::cout);
		int result = WSAStartup(MAKEWORD(2, 2), &locData);
		return result == 0;
	}
	bool Shutdown()
	{
		logger::Logger::Wait();
		int result = WSACleanup();
		return result == 0;
	}
}
