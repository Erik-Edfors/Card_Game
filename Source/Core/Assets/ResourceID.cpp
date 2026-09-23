#include "ResourceID.h"

namespace assets
{
	
}

std::ostream& operator<<(std::ostream& aStream, const assets::ResourceID& aResourceID)
{
	return aStream << aResourceID.GetString();
}
