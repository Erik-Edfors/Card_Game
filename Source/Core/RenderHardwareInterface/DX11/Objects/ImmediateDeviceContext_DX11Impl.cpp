#include "RenderHardwareInterface.pch.h"
#include "ImmediateDeviceContext_DX11Impl.h"
#include "DeferredDeviceContext_DX11Impl.h"

#include "DX11/Common_DX11.h"
#include "DX11/GAssert_DX11.h"

#include "Texture2D_DX11Impl.h"

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "ImmediateDeviceContext_DX11Impl.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->ImmediateDeviceContext_DX11Impl.cpp"

namespace graphics
{
	ImmediateDeviceContext_DX11Impl::ImmediateDeviceContext_DX11Impl(GraphicsID aID, ID3D11DeviceContext* aImmediateDeviceContext) : 
		DeviceContext_DX11Impl(aID, aImmediateDeviceContext),
		ImmediateDeviceContext(aID),
		DeviceContext(aID)
	{
	}
	ImmediateDeviceContext_DX11Impl::~ImmediateDeviceContext_DX11Impl() = default;

	result::Code ImmediateDeviceContext_DX11Impl::Map(Texture2D* aTexture, unsigned int aSubResource, GRAPHICS_MAP aMapType, GRAPHICS_MAP_FLAGS aMapFlags, void** outData, unsigned int* outRowPitch, unsigned int* outDepthPitch)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Map"

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");

		ID3D11Texture2D* dxTexture = texture->GetTexture();
		GRAPHICS_ASSERT(dxTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " has no valid texture assigned.");

		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};

		HRESULT result = GetDeviceContext()->Map(
			dxTexture,
			aSubResource,
			dx11::GetD3D11Map(aMapType),
			dx11::GetD3D11MapFlag(aMapFlags),
			&mappedSubresource
		);

		if (outData != nullptr)
		{
			*outData = mappedSubresource.pData;
		}
		if (outRowPitch != nullptr)
		{
			*outRowPitch = mappedSubresource.RowPitch;
		}
		if (outDepthPitch != nullptr)
		{
			*outDepthPitch = mappedSubresource.DepthPitch;
		}

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11DeviceContext::Map, LOG_CATEGORY);
		result;

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code ImmediateDeviceContext_DX11Impl::Unmap(Texture2D* aTexture, unsigned int aSubResource)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Unmap"

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");

		ID3D11Texture2D* dxTexture = texture->GetTexture();
		GRAPHICS_ASSERT(dxTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " has no valid texture assigned.");

		GetDeviceContext()->Unmap(
			dxTexture,
			aSubResource
		);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code ImmediateDeviceContext_DX11Impl::ExecuteDeferredDeviceContext(DeferredDeviceContext* aDeferredDeviceContext, bool aRestoreImmediateContextAfter)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::ExecuteDeferredDeviceContext"

		DeferredDeviceContext_DX11Impl* deferred = dynamic_cast<DeferredDeviceContext_DX11Impl*>(aDeferredDeviceContext);
		GRAPHICS_ASSERT(deferred != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDeferredDeviceContext) " could not be cast to " STRINGIFY(DeferredDeviceContext_DX11Impl) "!");

		Microsoft::WRL::ComPtr<ID3D11CommandList> commandList;
		HRESULT result = deferred->GetDeviceContext()->FinishCommandList(false, &commandList);
		GRAPHICS_ASSERT_WIN32_API_CALL(result, STRINGIFY(ID3D11DeviceContext::FinishCommandList), LOG_CATEGORY);
		result;

		GetDeviceContext()->ExecuteCommandList(commandList.Get(), aRestoreImmediateContextAfter);

		return result::OK;
#undef LOG_CATEGORY
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY