#pragma once
#include "GraphicsDefs.h"

namespace graphics
{
	class Device;
	class Window;
	class Texture2D;

	result::Code CreateDevice(Device** outDevice, Window* aViewport, GRAPHICS_DEVICE_FLAGS aFlags);
}