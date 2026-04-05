#pragma once
#include "DeviceContext_DX11Impl.h"
#include "../../Objects/ImmediateDeviceContext.h"
#include <wrl.h>

struct ID3D11DeviceContext;

namespace graphics
{
#pragma warning(push)
#pragma warning(disable : 4250)
	class ImmediateDeviceContext_DX11Impl : public ImmediateDeviceContext, public DeviceContext_DX11Impl
	{
	public:
		ImmediateDeviceContext_DX11Impl(GraphicsID aID, ID3D11DeviceContext* aImmediateDeviceContext);
		~ImmediateDeviceContext_DX11Impl();

		result::Code Map(
			Texture2D* aTexture,
			unsigned int aSubResource,
			GRAPHICS_MAP aMapType,
			GRAPHICS_MAP_FLAGS aMapFlags,
			void** outData,
			unsigned int* outRowPitch,
			unsigned int* outDepthPitch
		) override;

		result::Code Unmap(
			Texture2D* aTexture,
			unsigned int aSubResource
		) override;

	private:
	};
#pragma warning(pop)
}