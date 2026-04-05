#include "RenderHardwareInterface.pch.h"
#include "DeferredDeviceContext_DX11Impl.h"

namespace graphics
{
	DeferredDeviceContext_DX11Impl::DeferredDeviceContext_DX11Impl(GraphicsID aID, ID3D11DeviceContext* aDeferredDeviceContext) : 
		DeviceContext_DX11Impl(aID, aDeferredDeviceContext),
		DeferredDeviceContext(aID),
		DeviceContext(aID)
	{
	}
	DeferredDeviceContext_DX11Impl::~DeferredDeviceContext_DX11Impl()
	{
	}
}