#pragma once
#include "../GraphicsEngineDefs.h"
#include <Core/RenderHardwareInterface/GraphicsDefs.h>
#include <Core/RenderHardwareInterface/PImpl.hpp>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>
#include <Core/RenderHardwareInterface/Objects/BlendState.h>
#include <vector>

namespace graphics
{
	class ShaderResource;
	class BlendState;
}

struct MaterialResource
{
	graphics::PImpl<graphics::ShaderResource> Resource;
	unsigned int Slot;
	graphics::GRAPHICS_SHADER_STAGE_FLAGS Stages;
};

struct Material
{
	float AlbedoTint[4];
	float EmissiveColor[3];
	float EmissionIntensity;
	float Roughness;
	float Metalness;
	GRAPHICS_REGISTRY Registry;
	graphics::PImpl<graphics::BlendState> BlendState;
	unsigned int Flags;

	std::vector<MaterialResource> Resources;
};