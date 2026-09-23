#include "AssetManager.h"
#include <fstream>
#include <filesystem>

namespace assets
{
	//std::unordered_map<std::filesystem::path, AssetManager::AssetRegistry> AssetManager::myAssets;
	std::filesystem::path locAssetPath;

	//static constexpr size_t locBufferSize = 1'048'576;
	//static char locBuffer[locBufferSize];

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

	//size_t AssetManager::OpenFileContents(const std::filesystem::path& aPath, const void** outContents)
	//{
	//	std::ifstream file(aPath, std::ifstream::binary);
	//
	//	if (not file.is_open())
	//	{
	//		//TODO: log
	//		return nullptr;
	//	}
	//
	//	constexpr size_t readSize = 4096;
	//	size_t remainder = locBufferSize;
	//	bool finished = false;
	//	while (finished == false && remainder > 0)
	//	{
	//		file.read(locBuffer, std::min(readSize, remainder));
	//		remainder -= file.gcount();
	//		finished = file.eof();
	//
	//		if (file.fail())
	//		{
	//			return nullptr;
	//		}
	//	}
	//	if (not finished)
	//	{
	//		return nullptr;
	//	}
	//
	//	return locBuffer;
	//}
	//
	//void AssetManager::FreeFileContents(void* aFileContents)
	//{
	//	if (aFileContents != nullptr)
	//	{
	//		delete[] aFileContents;
	//	}
	//}
}