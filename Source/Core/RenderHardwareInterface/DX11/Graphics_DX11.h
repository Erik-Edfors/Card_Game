#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Core/RenderHardwareInterface/Result.h>

namespace graphics
{
	class Window;

	namespace dx11
	{
		result::Code CreateGraphicsWindow(Window** outWindow, HWND aWindowHandle);

		bool ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}