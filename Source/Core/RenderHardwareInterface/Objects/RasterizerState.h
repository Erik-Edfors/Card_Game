#pragma once
#include "GraphicsObject.h"

namespace graphics
{
	struct RasterizerStateDesc
	{
		std::string Name;
		GRAPHICS_FILL_MODE FillMode;
		GRAPHICS_CULL_MODE CullMode;
		GRAPHICS_WINDING_ORDER WindingOrder;
		int DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;
		bool EnableDepthClip;
		bool EnableScissor;
		bool EnableMultisample;
		bool EnableAntialiasedLine;
	};

	class RasterizerState : public GraphicsObject
	{
	public:
	protected:
		RasterizerState(GraphicsID aID, const RasterizerStateDesc& aDesc);
	private:
		RasterizerStateDesc myDesc;
	};
}