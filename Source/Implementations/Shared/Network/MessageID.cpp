#include "Shared.pch.h"
#include "MessageID.h"

std::ostream& operator<<(std::ostream& aStream, const MessageID& aMessageID)
{
	return aStream << aMessageID.GetString();
}
