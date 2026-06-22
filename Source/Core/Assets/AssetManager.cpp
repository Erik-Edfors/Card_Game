#include <Core/Logger/Logger.h>
#include "AssetManager.h"

#define FILE_CATEGORY "AssetManager.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->AssetManager"

namespace assets
{
	AssetManager::AssetManager() noexcept
	{
	}

	AssetManager::~AssetManager() noexcept
	{
	}

	bool AssetManager::Initialize(const wchar_t* aDirectory)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Initialize"
		myDirectory = std::filesystem::path(aDirectory).lexically_normal();

		std::error_code error;
		for (const std::filesystem::directory_entry& it : std::filesystem::recursive_directory_iterator(myDirectory, error))
		{
			if (it.is_regular_file())
			{
				const std::filesystem::path path = it.path();
				const std::wstring name = path.filename().wstring();

				if (myAssetPaths.contains(name))
				{
					const std::filesystem::path previous = myAssetPaths[name];

					LOG_WARNING(LOG_CATEGORY, "Duplicate asset name encountered! The latter will be unavailable. '%s' and '%s'.", previous.string(), path.string());
					continue;
				}

				myAssetPaths[name] = path;
			}
		}

		if (error)
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize AssetManager! Directory iteration flagged an error. Error: %s", error.message());
			return false;
		}

		return true;
#undef LOG_CATEGORY
	}

	size_t AssetManager::Clear(ASSET_RETENTION aRetention)
	{
		size_t count = 0;
		auto it = myAssets.begin();
		while (it != myAssets.end())
		{
			const __AssetEntry& entry = it->second;

			if (entry.Asset->GetRetention() <= aRetention)
			{
				it = myAssets.erase(it);
				++count;
			}
			else
			{
				it = std::next(it);
			}
		}
		return count;
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY