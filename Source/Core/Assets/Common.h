#pragma once
#include <string>

namespace assets
{
	struct FileHeader
	{
		char Type[4];
		unsigned char Version[4];
	};

	bool operator==(const FileHeader& aLHS, const FileHeader& aRHS) noexcept;
	bool operator!=(const FileHeader& aLHS, const FileHeader& aRHS) noexcept;

	std::string to_string(const FileHeader& aFileHeader) noexcept;
}