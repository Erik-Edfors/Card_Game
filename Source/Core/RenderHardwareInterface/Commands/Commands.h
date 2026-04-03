#pragma once
#include "Result.h"
#include "GraphicsDefs.h"
#include "PImpl.hpp"

namespace graphics
{
	class Device;

	class ShaderResource;

	class Command
	{
	public:
		virtual ~Command() = default;

		virtual result::Code Execute(Device* aDevice) = 0;
	};

	class BindShaderResourceCommand : public Command
	{
	public:
		BindShaderResourceCommand(ShaderResource* aShaderResource, unsigned int aSlot, GRAPHICS_SHADER_STAGE_FLAGS aStages);
		~BindShaderResourceCommand();

		result::Code Execute(Device* aDevice) override;

	private:
		PImpl<ShaderResource> myShaderResource;
		unsigned int mySlot;
		GRAPHICS_SHADER_STAGE_FLAGS myStages;
	};
}