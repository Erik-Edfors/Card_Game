#pragma once
#include "Registry.hpp"
#include <Core/RenderHardwareInterface/PImpl.hpp>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>

namespace graphics
{
	class Device;
}

namespace assets
{
	struct TextureAsset
	{
		graphics::PImpl<graphics::Texture2D> Texture;
		graphics::PImpl<graphics::ShaderResource> ShaderResource;
	};

	class TextureRegistry : public Registry<TextureAsset>
	{
	public:
		TextureRegistry(graphics::Device* aDevice);

	protected:
		virtual bool OnLoad(const std::filesystem::path& aPath, TextureAsset* outResult) override;

	private:
		graphics::PImpl<graphics::Device> myDevice;
	};
}