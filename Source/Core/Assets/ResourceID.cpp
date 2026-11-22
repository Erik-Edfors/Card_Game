#include "Assets.pch.h"
#include "ResourceID.h"

std::ostream& operator<<(std::ostream& aStream, const assets::ResourceID& aResourceID)
{
	return aStream << aResourceID.GetString();
}
