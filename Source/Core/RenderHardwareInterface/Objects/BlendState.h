#pragma once
#include "GraphicsObject.h"
#include "../PImpl.hpp"

namespace graphics
{
	class Texture2D;

	typedef uint8_t GRAPHICS_RENDER_TARGET_MASK;

	struct RenderTargetBlendDesc
	{
		bool EnableBlend;

		GRAPHICS_BLEND SourceBlend;
		GRAPHICS_BLEND DestinationBlend;
		GRAPHICS_BLEND_OPERATION BlendOperation;

		GRAPHICS_BLEND AlphaSourceBlend;
		GRAPHICS_BLEND AlphaDestinationBlend;
		GRAPHICS_BLEND_OPERATION AlphaBlendOperation;

		GRAPHICS_RENDER_TARGET_MASK RenderTargetWriteMask;
	};

	struct BlendStateDesc
	{
		std::string Name;
		bool EnableAlphaToCoverage;
		bool EnableIndependentBlend;
		RenderTargetBlendDesc RenderTargetBlendDescriptions[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS];
	};

	class BlendState : public GraphicsObject
	{
	public:
		~BlendState();

	protected:
		BlendState(GraphicsID aID);
	};
}