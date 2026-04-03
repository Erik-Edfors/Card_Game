#include "RenderHardwareInterface.pch.h"
#include "RenderTarget_DX11Impl.h"
#include "../../Objects/Texture2D.h"
#include "../../Device.h"

#undef FILE_CATEGORY
#define FILE_CATEGORY "RenderTarget_DX11Impl.cpp"
#undef CLASS_CATEGORY
#define CLASS_CATEGORY FILE_CATEGORY"->RenderTarget_DX11Impl"

graphics::RenderTarget_DX11Impl::RenderTarget_DX11Impl(Device* aDevice, GraphicsID aID, ID3D11RenderTargetView* aRenderTargetView, const RenderTargetDesc& aDesc) : 
	myDevice(aDevice),
	myRenderTargetView(aRenderTargetView),
	myLastTextureVersion(aDesc.Texture == nullptr ? 0 : aDesc.Texture->GetVersion()),
	RenderTarget(aID, aDesc)
{
}
graphics::RenderTarget_DX11Impl::~RenderTarget_DX11Impl() = default;

void graphics::RenderTarget_DX11Impl::SetName(const char* aName)
{
	myRenderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11RenderTargetView* graphics::RenderTarget_DX11Impl::GetRenderTargetView()
{
#define LOG_CATEGORY CLASS_CATEGORY"::GetRenderTargetView"

	unsigned int textureVersion = GetTexture() == nullptr ? 0u : GetTexture()->GetVersion();
	if (myLastTextureVersion != textureVersion)
	{
		PImpl<RenderTarget> newRT;
		result::Code result = myDevice->CreateRenderTarget(
			&newRT,
			&GetDesc()
		);
		if (GRAPHICS_FAILED(result))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to update RenderTarget '%s'! Texture has a new version and RenderTarget failed to update itself.", GetName());
			return myRenderTargetView.Get();
		}
		RenderTarget_DX11Impl* newRTImpl = static_cast<RenderTarget_DX11Impl*>(newRT.Get());
		myRenderTargetView = newRTImpl->GetRenderTargetView();

		myLastTextureVersion = textureVersion;
	}

	return myRenderTargetView.Get();

#undef LOG_CATEGORY
}
