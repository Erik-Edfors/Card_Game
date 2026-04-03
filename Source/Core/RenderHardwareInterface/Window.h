#pragma once
#include "RefObject.hpp"

namespace graphics
{
	class Window : public RefObject
	{
	public:
		virtual ~Window() = default;

		virtual float GetLeft() = 0;
		virtual float GetRight() = 0;
		virtual float GetTop() = 0;
		virtual float GetBottom() = 0;

		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;

		virtual float GetClientLeft() = 0;
		virtual float GetClientRight() = 0;
		virtual float GetClientTop() = 0;
		virtual float GetClientBottom() = 0;

		virtual float GetClientWidth() = 0;
		virtual float GetClientHeight() = 0;

	protected:
		Window() = default;
	};
}