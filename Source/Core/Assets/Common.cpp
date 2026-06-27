#include "Common.h"
#include <format>

namespace assets
{
	bool operator==(const FileHeader& aLHS, const FileHeader& aRHS) noexcept
	{
		return memcmp(&aLHS, &aRHS, sizeof(FileHeader)) == 0;
	}
	bool operator!=(const FileHeader& aLHS, const FileHeader& aRHS) noexcept
	{
		return memcmp(&aLHS, &aRHS, sizeof(FileHeader)) != 0;
	}

	std::string to_string(const FileHeader& aFileHeader) noexcept
	{
		return std::format("{}: {}.{}.{}.{}", std::string(aFileHeader.Type, 4), aFileHeader.Version[0], aFileHeader.Version[1], aFileHeader.Version[2], aFileHeader.Version[3]);
	}
}