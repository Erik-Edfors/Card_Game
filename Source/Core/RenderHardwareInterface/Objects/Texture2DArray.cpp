#include "RenderHardwareInterface.pch.h"
#include "Texture2DArray.h"

graphics::GRAPHICS_BIND_FLAGS graphics::Texture2DArray::GetFlags() const
{
	return myFlags;
}

unsigned int graphics::Texture2DArray::GetArraySize() const
{
	return mySize;
}

graphics::Texture2DArray::Texture2DArray(GraphicsID aID, GRAPHICS_BIND_FLAGS aFlags, unsigned int aSize) :
	myFlags(aFlags),
	mySize(aSize),
	GraphicsObject(aID)
{
}
