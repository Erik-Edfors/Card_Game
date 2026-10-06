#pragma once
#include "Registry.hpp"

#define RAPIDJSON_NOMEMBERITERATORCLASS
#pragma warning(push)
#pragma warning(disable : 5054)
#include <External/rapidjson/document.h>
#pragma warning(pop)

namespace assets
{
	struct JsonAsset
	{
		rapidjson::Document Document;
	};

	class JsonRegistry : public Registry<JsonAsset>
	{
	protected:
		virtual bool OnLoad(const std::filesystem::path& aPath, JsonAsset* outResult) override;
	};
}