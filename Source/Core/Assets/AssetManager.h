#pragma once
#include "ResourceID.h"

#include <fstream>
#include <unordered_map>
#include <filesystem>

namespace assets
{
	class AssetManager
	{
	public:
		static bool Initialize(const std::filesystem::path& aAssetsPath);

		static bool Shutdown();

		static std::filesystem::path NormalizeFilePath(const std::filesystem::path& aPath);

		static const std::filesystem::path& GetAssetsPath();

		static assets::ResourceID GetID(const char* aNameSpace, const std::filesystem::path& aPath);

	private:
		struct AssetRegistry
		{
			void* Asset;
		};

		std::filesystem::path myAssetsPath;
	};
}