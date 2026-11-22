#pragma once
#include "RefObject.hpp"

namespace graphics
{
	class Window : public RefObject
	{
	public:
		virtual ~Window() = default;

		virtual float GetLeft() const = 0;
		virtual float GetRight() const = 0;
		virtual float GetTop() const = 0;
		virtual float GetBottom() const = 0;

		virtual float GetWidth() const = 0;
		virtual float GetHeight() const = 0;

		virtual float GetClientLeft() const = 0;
		virtual float GetClientRight() const = 0;
		virtual float GetClientTop() const = 0;
		virtual float GetClientBottom() const = 0;

		virtual float GetClientWidth() const = 0;
		virtual float GetClientHeight() const = 0;

		virtual float GetMouseX() const = 0;
		virtual float GetMouseY() const = 0;

	protected:
		Window() = default;
	};
}