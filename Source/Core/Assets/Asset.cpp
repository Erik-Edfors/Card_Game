#include "Asset.h"

namespace assets
{
	void Asset::SetRetention(ASSET_RETENTION aRetention) noexcept
	{
		myRetention = aRetention;
	}

	ASSET_RETENTION Asset::GetRetention() const noexcept
	{
		return myRetention;
	}
}
