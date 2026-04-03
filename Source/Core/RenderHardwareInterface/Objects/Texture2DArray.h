#pragma once
#include "GraphicsObject.h"

namespace graphics
{
	class Texture2DArray : public GraphicsObject
	{
	public:
		GRAPHICS_BIND_FLAGS GetFlags() const;

		unsigned int GetArraySize() const;

		virtual result::Code GetSize(
			unsigned int* outWidth,
			unsigned int* outHeight
		) const = 0;

	protected:
		Texture2DArray(GraphicsID aID, GRAPHICS_BIND_FLAGS aFlags, unsigned int aSize);

	private:
		GRAPHICS_BIND_FLAGS myFlags;
		unsigned int mySize;
	};
}