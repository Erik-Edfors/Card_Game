#include "RenderHardwareInterface.pch.h"
#include "DeviceContext.h"

namespace graphics
{
	DeviceContext::DeviceContext(GraphicsID aID) :
		GraphicsObject(aID)
	{
		aID;
	}
	DeviceContext::~DeviceContext() = default;
}