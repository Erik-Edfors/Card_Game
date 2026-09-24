#include "Networking.pch.h"
#include "ErrorHandling.h"

std::string networking::GetWinsockErrorMessage(int aError)
{
	LPSTR messageBuffer = NULL;

	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		aError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		NULL
	);

	std::string msg(messageBuffer, size);
	LocalFree(messageBuffer);

	return msg;
}
