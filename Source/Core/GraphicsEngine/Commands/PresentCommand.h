#pragma once
#include <Core/RenderHardwareInterface/GraphicsDefs.h>

class GraphicsEngine;

class PresentCommand
{
public:
	PresentCommand(GraphicsEngine* aEngine, graphics::GRAPHICS_PRESENT_FLAGS aFlags);

	void Execute();

private:
	GraphicsEngine* myEngine;
	graphics::GRAPHICS_PRESENT_FLAGS myFlags;
};