#pragma once
#include "../Window.h"
#include <windef.h>

namespace graphics
{
	class Window_DX11Impl : public Window
	{
	public:
		Window_DX11Impl(HWND aWindowHandle);
		~Window_DX11Impl();

		float GetLeft() override;
		float GetRight() override;
		float GetTop() override;
		float GetBottom() override;

		float GetWidth() override;
		float GetHeight() override;

		float GetClientLeft() override;
		float GetClientRight() override;
		float GetClientTop() override;
		float GetClientBottom() override;

		float GetClientWidth() override;
		float GetClientHeight() override;

		HWND GetWindowHandle() const;

		bool ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND myWindowHandle;
		UINT myWidth;
		UINT myHeight;
	};
}