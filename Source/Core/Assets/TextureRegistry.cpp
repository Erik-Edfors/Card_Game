#include "Assets.pch.h"
#include "TextureRegistry.h"
#include <Core/RenderHardwareInterface/DDSLoad.h>
#include <Core/RenderHardwareInterface/Device.h>

#define FILE_CATEGORY "TextureRegistry.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->TextureRegistry"

assets::TextureRegistry::TextureRegistry(graphics::Device* aDevice) :
	myDevice(aDevice)
{
}

bool assets::TextureRegistry::OnLoad(const std::filesystem::path& aPath, TextureAsset* outResult)
{
#define LOG_CATEGORY CLASS_CATEGORY"::OnLoad"
#define MAKE_ERROR_MSG(details) "Failed to load texture '%s'! " details

	TextureAsset asset;

	const std::string name = aPath.filename().replace_extension().string();

	std::string extension = aPath.extension().string();
	std::transform(extension.begin(), extension.end(), extension.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });

	if (extension != ".dds")
	{
		LOG_ERROR(LOG_CATEGORY, MAKE_ERROR_MSG("File must have '.dds' extension."), aPath.string());
		return false;
	}

	graphics::result::Code result = graphics::CreateTexture2DFromFile(
		myDevice.Get(),
		std::format("{} Texture", name).c_str(),
		aPath.c_str(),
		&asset.Texture,
		graphics::GRAPHICS_BIND_SHADER_RESOURCE,
		graphics::GRAPHICS_RESOURCE_NONE,
		graphics::GRAPHICS_USAGE_DEFAULT,
		graphics::GRAPHICS_CPU_ACCESS_NONE
	);

	if (GRAPHICS_FAILED(result))
	{
		const graphics::result::Message msg = graphics::result::GetErrorMessage(result);
		LOG_ERROR(LOG_CATEGORY, MAKE_ERROR_MSG("%s: %s"), aPath.string(), msg.Type, msg.Desc);
		msg;
		return false;
	}

	{
		const graphics::ShaderResourceDesc desc = {
			.Name = std::format("{} ShaderResource", name).c_str(),
			.Texture = asset.Texture.Get(),
			.Format = graphics::GRAPHICS_FORMAT_UNKNOWN,
			.ViewDimension = graphics::GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION_TEXTURE2D,
			.FirstSlice = 0,
			.NumSlices = 1,
			.MipLevels = -1,
			.MostDetailedMip = 0
		};

		result = myDevice->CreateShaderResource(&asset.ShaderResource, &desc);
		if (GRAPHICS_FAILED(result))
		{
			const graphics::result::Message msg = graphics::result::GetErrorMessage(result);
			LOG_ERROR(LOG_CATEGORY, MAKE_ERROR_MSG("%s: %s"), aPath.string(), msg.Type, msg.Desc);
			msg;
			return false;
		}
	}

	*outResult = std::move(asset);

	return true;

#undef MAKE_ERROR_MSG
#undef LOG_CATEGORY
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
