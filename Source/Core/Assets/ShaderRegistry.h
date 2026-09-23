#pragma once
#include "Registry.hpp"
#include <Core/RenderHardwareInterface/PImpl.hpp>
#include <Core/RenderHardwareInterface/Objects/Shaders/Shader.h>

namespace graphics
{
	class Device;
}

namespace assets
{
	struct ShaderAsset
	{
		graphics::PImpl<graphics::Shader> Shader;
		std::unique_ptr<char[]> ByteCode;
		size_t ByteCodeSize;
	};

	class ShaderRegistry : public Registry<ShaderAsset>
	{
	public:
		ShaderRegistry(graphics::Device* aDevice);

	protected:
		virtual bool OnLoad(const std::filesystem::path& aPath, ShaderAsset* outResult) override;

	private:
		graphics::PImpl<graphics::Device> myDevice;
	};
}