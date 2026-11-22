#include "Assets.pch.h"
#include "AssetManager.h"
#include <fstream>
#include <filesystem>

namespace assets
{
	std::filesystem::path locAssetPath;

	bool AssetManager::Initialize(const std::filesystem::path& aAssetsPath)
	{
		locAssetPath = NormalizeFilePath(aAssetsPath);
		return true;
	}

	bool AssetManager::Shutdown()
	{
		return true;
	}

	std::filesystem::path AssetManager::NormalizeFilePath(const std::filesystem::path& aPath)
	{
		return aPath.lexically_normal().relative_path();
	}

	const std::filesystem::path& AssetManager::GetAssetsPath()
	{
		return locAssetPath;
	}

	assets::ResourceID AssetManager::GetID(const char* aNameSpace, const std::filesystem::path& aPath)
	{
		const std::filesystem::path path = NormalizeFilePath(aPath).replace_extension();

		std::string id = path.string();
		size_t toReplace = 0;
		while ((toReplace = id.find('\\', toReplace)) != std::string::npos)
		{
			id.replace(toReplace, 1, "/");
		}

		id = std::format("{}:{}", aNameSpace, id);

		auto toLower = [](char c) { return static_cast<char>(std::tolower(c)); };
		std::transform(id.begin(), id.end(), id.begin(), toLower);

		return assets::ResourceID(id);
	}
}