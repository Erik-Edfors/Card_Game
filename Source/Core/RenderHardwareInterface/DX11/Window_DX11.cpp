#include "RenderHardwareInterface.pch.h"
#include "Window_DX11Impl.h"

#include <Core/imgui/imgui_impl_win32.h>

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

float graphics::Window_DX11Impl::GetLeft()
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.left);
}

float graphics::Window_DX11Impl::GetRight()
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right);
}

float graphics::Window_DX11Impl::GetTop()
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.top);
}

float graphics::Window_DX11Impl::GetBottom()
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom);
}

float graphics::Window_DX11Impl::GetWidth()
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right - rect.left);
}

float graphics::Window_DX11Impl::GetHeight()
{
	RECT rect = {};
	GetWindowRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom - rect.top);
}

float graphics::Window_DX11Impl::GetClientLeft()
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.left);
}

float graphics::Window_DX11Impl::GetClientRight()
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right);
}

float graphics::Window_DX11Impl::GetClientTop()
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.top);
}

float graphics::Window_DX11Impl::GetClientBottom()
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom);
}

float graphics::Window_DX11Impl::GetClientWidth()
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.right - rect.left);
}

float graphics::Window_DX11Impl::GetClientHeight()
{
	RECT rect = {};
	GetClientRect(myWindowHandle, &rect);

	return static_cast<float>(rect.bottom - rect.top);
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
		myWidth = LOWORD(lParam);
		myHeight = HIWORD(lParam);
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