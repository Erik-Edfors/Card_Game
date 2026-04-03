#include "RenderHardwareInterface.pch.h"
#include "Commands.h"
#include "Device.h"
#include "Objects\ShaderResource.h"

graphics::BindShaderResourceCommand::BindShaderResourceCommand(ShaderResource* aShaderResource, unsigned int aSlot, GRAPHICS_SHADER_STAGE_FLAGS aStages) : 
	myShaderResource(aShaderResource),
	mySlot(aSlot),
	myStages(aStages)
{
}
graphics::BindShaderResourceCommand::~BindShaderResourceCommand() = default;
graphics::result::Code graphics::BindShaderResourceCommand::Execute(Device* aDevice)
{
	return aDevice->BindShaderResources(&myShaderResource, 1u, mySlot, myStages);
}
