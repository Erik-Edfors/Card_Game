#pragma once
//#define WIN32_LEAN_AND_MEAN
//#define NOMINMAX
//#include <Windows.h>
#include <Core/RenderHardwareInterface/Result.h>

struct HWND__;
typedef HWND__* HWND;
typedef unsigned int UINT;
typedef unsigned long long WPARAM;
typedef long long LPARAM;

namespace graphics
{
	class Window;

	namespace dx11
	{
		result::Code CreateGraphicsWindow(Window** outWindow, HWND aWindowHandle);

		bool ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}