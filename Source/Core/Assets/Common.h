#pragma once

namespace assets
{
	struct FileHeader
	{
		char Type[4];
		unsigned char Version[4];
	};
}