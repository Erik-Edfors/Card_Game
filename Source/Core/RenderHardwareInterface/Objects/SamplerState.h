#pragma once
#include "GraphicsObject.h"

namespace graphics
{
	struct SamplerStateDesc
	{
		std::string Name;
		GRAPHICS_SAMPLER_FILTER Filter;
		GRAPHICS_TEXTURE_ADDRESS_MODE AddressU;
		GRAPHICS_TEXTURE_ADDRESS_MODE AddressV;
		GRAPHICS_TEXTURE_ADDRESS_MODE AddressW;
		float MipLODBias;
		unsigned int MaxAnistropy;
		GRAPHICS_COMPARISON Comparison;
		float BorderColor[4];
		float MinLOD;
		float MaxLOD;
	};

	class SamplerState : public GraphicsObject
	{
	public:

	protected:
		SamplerState(GraphicsID aID);

	private:
	};
}