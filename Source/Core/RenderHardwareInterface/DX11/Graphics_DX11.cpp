#include "RenderHardwareInterface.pch.h"
#include "Graphics_DX11.h"
#include "DX11/Window_DX11Impl.h"

#include <unordered_map>
#include "PImpl.hpp"

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "Graphics_Win32.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->"

namespace graphics
{
	static std::unordered_map<HWND, PImpl<graphics::Window_DX11Impl>> locWindows;

	void removeUnreferencedViewports()
	{
		auto it = locWindows.begin();
		while (it != locWindows.end())
		{
			if (it->second->ReferenceCount() == 1)
			{
				it = locWindows.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	bool addViewport(graphics::Window_DX11Impl* aWindow)
	{
		removeUnreferencedViewports();
		const HWND handle = aWindow->GetWindowHandle();
		if (locWindows.count(handle) == 0)
		{
			locWindows.emplace(aWindow->GetWindowHandle(), aWindow);
			return true;
		}
		return false;
	}

	result::Code graphics::dx11::CreateGraphicsWindow(Window** outViewport, HWND aWindowHandle)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateGraphicsWindow"

		GRAPHICS_ASSERT(outViewport != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, "outViewport was nullptr!");
		GRAPHICS_ASSERT(aWindowHandle != NULL, LOG_CATEGORY, result::INVALID_ARGUMENT, "aWindowHandle was NULL!");

		Window_DX11Impl* window = new Window_DX11Impl(aWindowHandle);

		if (addViewport(window) == false)
		{
			delete window;
			LOG_ERROR(LOG_CATEGORY, std::string(result::INVALID_STATE_DESC) + " aWindowHandle already has an associated window!");
			return result::INVALID_STATE;
		}

		*outViewport = window;
		window->AddReference();

		return result::OK;
#undef LOG_CATEGORY
	}

	bool graphics::dx11::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::ProcessMessage"

		removeUnreferencedViewports();
		auto it = locWindows.find(hWnd);
		if (it != locWindows.end())
		{
			return it->second->ProcessMessage(hWnd, uMsg, wParam, lParam);
		}

		return false;
#undef LOG_CATEGORY
	}
}
