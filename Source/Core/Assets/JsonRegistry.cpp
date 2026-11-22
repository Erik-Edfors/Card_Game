#include "Assets.pch.h"
#include "JsonRegistry.h"
#include <Core/Logger/Logger.h>

#pragma warning(push)
#pragma warning(disable : 5054)
#include <External/rapidjson/document.h>
#include <External/rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>
#pragma warning(pop)

#define FILE_CATEGORY "JsonRegistry.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->JsonRegistry"

namespace assets
{
	bool JsonRegistry::OnLoad(const std::filesystem::path& aPath, JsonAsset* outResult)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::OnLoad"

		std::ifstream file(aPath);

		if (not file.is_open() || file.fail())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to load asset! Failed to open file.");
			return false;
		}

		rapidjson::IStreamWrapper stream(file);
		rapidjson::Document document;
		document.ParseStream(stream);

		if (document.HasParseError())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to load asset! Failed to parse json. Error: %s", rapidjson::GetParseError_En(document.GetParseError()));
			return false;
		}

		*outResult = JsonAsset{
			.Document = std::move(document)
		};

		return true;

#undef LOG_CATEGORY
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY