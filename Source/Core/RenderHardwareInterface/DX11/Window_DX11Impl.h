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

		float GetLeft() const override;
		float GetRight() const override;
		float GetTop() const override;
		float GetBottom() const override;

		float GetWidth() const override;
		float GetHeight() const override;

		float GetClientLeft() const override;
		float GetClientRight() const override;
		float GetClientTop() const override;
		float GetClientBottom() const override;

		float GetClientWidth() const override;
		float GetClientHeight() const override;

		float GetMouseX() const override;
		float GetMouseY() const override;

		void SetImGuiEnabled(bool aEnabled) noexcept;
		bool IsImGuiEnabled() const noexcept;

		HWND GetWindowHandle() const;

		bool ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND myWindowHandle;
		INT myMouseX;
		INT myMouseY;
		UINT myWidth;
		UINT myHeight;
		bool myImGuiEnabled;
	};
}