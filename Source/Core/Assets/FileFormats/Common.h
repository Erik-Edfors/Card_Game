#pragma once

namespace assets
{
	struct FileHeader
	{
		char Identifier[4];
		unsigned char Version[4];
	};

	bool operator ==(const FileHeader& aLHS, const FileHeader& aRHS);
	bool operator !=(const FileHeader& aLHS, const FileHeader& aRHS);
}