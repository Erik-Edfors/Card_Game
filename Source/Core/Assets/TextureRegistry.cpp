#include "TextureRegistry.h"
#include <Core/RenderHardwareInterface/DDSLoad.h>
#include <Core/RenderHardwareInterface/Device.h>

assets::TextureRegistry::TextureRegistry(graphics::Device* aDevice) :
	myDevice(aDevice)
{
}

bool assets::TextureRegistry::OnLoad(const std::filesystem::path& aPath, TextureAsset* outResult)
{
	TextureAsset asset;

	const std::string name = aPath.filename().replace_extension().string();

	std::string extension = aPath.extension().string();
	std::transform(extension.begin(), extension.end(), extension.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });

	if (extension != ".dds")
	{
		//TODO: Log wrong extension
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
		//TODO: Log result
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
			//TODO: Log result
			return false;
		}
	}

	*outResult = std::move(asset);

	return true;
}
