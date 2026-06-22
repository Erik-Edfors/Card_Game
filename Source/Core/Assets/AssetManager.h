#pragma once
#include "Asset.h"
#include <filesystem>
#include <Core/CommonUtilities/StringHash.hpp>
#include <unordered_map>
#include <memory>
#include <fstream>

namespace assets
{
	class AssetManager
	{
	public:
		AssetManager() noexcept;
		~AssetManager() noexcept;

		bool Initialize(const wchar_t* aDirectory);

		template<class _AssetTy>
		_AssetTy* Get(const wchar_t* aName, ASSET_RETENTION aRetention = ASSET_RETENTION_WEAK);

		size_t Clear(ASSET_RETENTION aRetention);

	private:
		struct __AssetEntry
		{
			std::unique_ptr<Asset> Asset;
			const std::type_info* Type;
			std::filesystem::path Path;
		};

		std::unordered_map<std::wstring, __AssetEntry> myAssets;
		std::unordered_map<std::wstring, std::filesystem::path> myAssetPaths;

		std::filesystem::path myDirectory;
	};

	template<class _AssetTy>
	inline _AssetTy* AssetManager::Get(const wchar_t* aName, ASSET_RETENTION aRetention)
	{
		constexpr const std::type_info& type = typeid(_AssetTy);

		auto it = myAssets.find(aName);
		if (it != myAssets.end())
		{
			__AssetEntry& entry = it->second;

			if (entry.Asset == nullptr)
			{
				_AssetTy* rawAsset = new _AssetTy();
				rawAsset->SetRetention(aRetention);
				std::unique_ptr<Asset> asset = std::unique_ptr<_AssetTy>(rawAsset);

				if (not rawAsset->Load(entry.Path.c_str()))
				{
					return nullptr;
				}

				entry.Asset = std::move(asset);
				entry.Type = &type;
				return entry.Asset.get();
			}
			else if (*entry.Type == type)
			{
				if (aRetention > entry.Asset->GetRetention())
				{
					entry.Asset->SetRetention(aRetention);
				}
				return entry.Asset.get();
			}
			else
			{
				return nullptr;
			}
		}
	}
}