#pragma once
#include "DeviceContext_DX11Impl.h"
#include "../../Objects/DeferredDeviceContext.h"
#include <wrl.h>

struct ID3D11DeviceContext;

namespace graphics
{
#pragma warning(push)
#pragma warning(disable : 4250)
	class DeferredDeviceContext_DX11Impl : public DeferredDeviceContext, public DeviceContext_DX11Impl
	{
	public:
		DeferredDeviceContext_DX11Impl(GraphicsID aID, ID3D11DeviceContext* aDeferredDeviceContext);
		~DeferredDeviceContext_DX11Impl();
	private:
	};
#pragma warning(pop)
}