#pragma once
#include "Registry.hpp"
#include "FileFormats/Font.h"

namespace assets
{
	class FontRegistry : public Registry<FontFile>
	{
	protected:
		virtual bool OnLoad(const std::filesystem::path& aPath, FontFile* outResult) override;
	};
}