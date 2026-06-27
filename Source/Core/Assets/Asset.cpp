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
	void Asset::Acquire() noexcept
	{
		++myReferenceCount;
	}
	void Asset::Release() noexcept
	{
		if (myReferenceCount > 0)
		{
			--myReferenceCount;
		}
	}
	size_t Asset::GetReferenceCount() const noexcept
	{
		return myReferenceCount;
	}
	Asset::Asset() : 
		myRetention(ASSET_RETENTION_WEAK),
		myReferenceCount(0)
	{
	}
}
