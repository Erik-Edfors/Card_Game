#include "FontRegistry.h"

namespace assets
{
	bool assets::FontRegistry::OnLoad(const std::filesystem::path& aPath, FontFile* outResult)
	{
		return assets::Load(aPath, outResult);
	}
}
