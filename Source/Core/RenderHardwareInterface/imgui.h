#pragma once
#include "Window.h"
#include "Device.h"

namespace graphics
{
	bool ImGuiInitialize(Device* aDevice);
	bool ImGuiShutdown();

	bool ImGuiNewFrame();
	bool ImGuiRender();
}