#include "RenderHardwareInterface.pch.h"
#include "Buffer.h"

size_t graphics::Buffer::GetSize() const
{
	return mySize;
}

graphics::Buffer::Buffer(GraphicsID aID, size_t aSize) :
	mySize(aSize),
	GraphicsObject(aID)
{
}
