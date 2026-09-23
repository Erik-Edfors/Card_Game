#pragma once
#include "AssetManager.h"
#include "ResourceID.h"

#include <filesystem>
#include <unordered_map>

namespace assets
{
	template<class _Ty>
	class Registry
	{
	public:
		size_t Load(const char* aNameSpace, const std::filesystem::path& aPath);
		const _Ty* Get(const ResourceID& aResourceID);
	protected:
		virtual bool OnLoad(const std::filesystem::path& aPath, _Ty* outResult) = 0;
	private:
		std::unordered_map<ResourceID, std::unique_ptr<_Ty>> myResources;
	};

	template<class _Ty>
	size_t Registry<_Ty>::Load(const char* aNameSpace, const std::filesystem::path& aPath)
	{
		size_t loaded = 0;
		std::error_code error;
		std::unique_ptr<_Ty> allocated = std::make_unique<_Ty>();

		for (const auto& entry : std::filesystem::recursive_directory_iterator(aPath, error))
		{
			if (entry.is_regular_file())
			{
				ResourceID id = AssetManager::GetID(aNameSpace, entry.path().lexically_relative(aPath));
				if (myResources.find(id) != myResources.end())
				{
					//TODO: Log collision.
					continue;
				}

				if (OnLoad(AssetManager::NormalizeFilePath(entry.path()), allocated.get()))
				{
					myResources[id] = std::move(allocated);
					allocated = std::make_unique<_Ty>();
					++loaded;
				}
				else
				{
					//TODO: Log
					*allocated = _Ty();
				}
			}
		}
		if (error)
		{
			//TODO: Log;
			return loaded;
		}
		return loaded;
	}

	template<class _Ty>
	const _Ty* Registry<_Ty>::Get(const ResourceID& aResourceID)
	{
		auto it = myResources.find(aResourceID);
		if (it != myResources.end())
		{
			return it->second.get();
		}
		return nullptr;
	}
}