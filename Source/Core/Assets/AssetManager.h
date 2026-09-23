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

		//template<class _Ty>
		//static _Ty* Load(const std::filesystem::path& aFile);

		static std::filesystem::path NormalizeFilePath(const std::filesystem::path& aPath);

		static const std::filesystem::path& GetAssetsPath();

		static assets::ResourceID GetID(const char* aNameSpace, const std::filesystem::path& aPath);

	private:
		struct AssetRegistry
		{
			void* Asset;
		};

		//static size_t OpenFileContents(const std::filesystem::path& aPath, const void** outContents);
		//static void FreeFileContents(void* aFileContents);

		std::filesystem::path myAssetsPath;

		//static std::unordered_map<std::filesystem::path, AssetRegistry> myAssets;
	};

	//template<class _Ty>
	//inline _Ty* AssetManager::Load(const std::filesystem::path& aFile)
	//{
	//	std::filesystem::path path = NormalizeFilePath(aFile);

	//	{
	//		auto it = myAssets.find(path);
	//		if (it != myAssets.end())
	//		{
	//			return it->second.Asset;
	//		}
	//	}

	//	void* contents = OpenFileContents(path);

	//	//TODO: assign asset with deleter.
	//	myAssets[path] = AssetRegistry{
	//		.Asset = nullptr
	//	};

	//	FreeFileContents(contents);

	//	return myAssets[path].Asset.get();
	//}
}