#include "RenderHardwareInterface.pch.h"
#include "Window_DX11Impl.h"

#include <Core/imgui/imgui_impl_win32.h>

#include <windowsx.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

graphics::Window_DX11Impl::Window_DX11Impl(HWND aWindowHandle) :
	myWindowHandle(aWindowHandle),
	myWidth(0),
	myHeight(0),
	myImGuiEnabled(false)
{
	RECT size = {};
	GetWindowRect(aWindowHandle, &size);
	myWidth = static_cast<UINT>(size.right - size.left);
	myHeight = static_cast<UINT>(size.bottom - size.top);
}

graphics::Window_DX11Impl::~Window_DX11Impl() = default;

float graphics::Window_DX11Impl::GetLeft() const
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.left);
}

float graphics::Window_DX11Impl::GetRight() const
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right);
}

float graphics::Window_DX11Impl::GetTop() const
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.top);
}

float graphics::Window_DX11Impl::GetBottom() const
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom);
}

float graphics::Window_DX11Impl::GetWidth() const
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right - rect.left);
}

float graphics::Window_DX11Impl::GetHeight() const
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom - rect.top);
}

float graphics::Window_DX11Impl::GetClientLeft() const
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.left);
}

float graphics::Window_DX11Impl::GetClientRight() const
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right);
}

float graphics::Window_DX11Impl::GetClientTop() const
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.top);
}

float graphics::Window_DX11Impl::GetClientBottom() const
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom);
}

float graphics::Window_DX11Impl::GetClientWidth() const
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right - rect.left);
}

float graphics::Window_DX11Impl::GetClientHeight() const
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom - rect.top);
}

float graphics::Window_DX11Impl::GetMouseX() const
{
	return static_cast<float>(myMouseX);
}

float graphics::Window_DX11Impl::GetMouseY() const
{
	return static_cast<float>(myMouseY);
}

void graphics::Window_DX11Impl::SetImGuiEnabled(bool aEnabled) noexcept
{
	myImGuiEnabled = aEnabled;
}

bool graphics::Window_DX11Impl::IsImGuiEnabled() const noexcept
{
	return myImGuiEnabled;
}

HWND graphics::Window_DX11Impl::GetWindowHandle() const
{
	return myWindowHandle;
}

bool graphics::Window_DX11Impl::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	hWnd;
	wParam;

	switch (uMsg)
	{
	case WM_SIZE:
	{
		myWidth = LOWORD(lParam);
		myHeight = HIWORD(lParam);
	}
	break;
	case WM_MOUSEMOVE:
	{
		RECT rect;
		GetClientRect(myWindowHandle, &rect);

		myMouseX = GET_X_LPARAM(lParam) - static_cast<INT>(rect.left);
		myMouseY = GET_Y_LPARAM(lParam) - static_cast<INT>(rect.top);
	}
	break;
	}

	if (IsImGuiEnabled())
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) != FALSE)
		{
			return true;
		}
	}

	return false;
}