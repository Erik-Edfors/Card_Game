#include "Common.h"
#include <memory>

namespace assets
{
	bool operator ==(const FileHeader& aLHS, const FileHeader& aRHS)
	{
		return memcmp(&aLHS, &aRHS, sizeof(FileHeader)) == 0;
	}
	bool operator!=(const FileHeader& aLHS, const FileHeader& aRHS)
	{
		return memcmp(&aLHS, &aRHS, sizeof(FileHeader)) != 0;
	}
}