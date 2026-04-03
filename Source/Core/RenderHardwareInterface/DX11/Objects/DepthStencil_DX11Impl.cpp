#include "RenderHardwareInterface.pch.h"
#include "DepthStencil_DX11Impl.h"
#include "../../Objects/Texture2D.h"
#include "../../Device.h"

#undef FILE_CATEGORY
#define FILE_CATEGORY "DepthStencil_DX11Impl.cpp"
#undef CLASS_CATEGORY
#define CLASS_CATEGORY FILE_CATEGORY"->DepthStencil_DX11Impl"

graphics::DepthStencil_DX11Impl::DepthStencil_DX11Impl(Device* aDevice, GraphicsID aID, ID3D11DepthStencilView* aDepthStencilView, const DepthStencilDesc& aDesc) :
	myDevice(aDevice),
	myDepthStencilView(aDepthStencilView),
	myLastTextureVersion(aDesc.Texture == nullptr ? 0u : aDesc.Texture->GetVersion()),
	DepthStencil(aID, aDesc)
{
}
graphics::DepthStencil_DX11Impl::~DepthStencil_DX11Impl() = default;

void graphics::DepthStencil_DX11Impl::SetName(const char* aName)
{
	myDepthStencilView->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11DepthStencilView* graphics::DepthStencil_DX11Impl::GetDepthStencilView()
{
#define LOG_CATEGORY CLASS_CATEGORY"::GetDepthStencilView"

	unsigned int textureVersion = GetTexture() == nullptr ? 0u : GetTexture()->GetVersion();
	if (myLastTextureVersion != textureVersion)
	{
		PImpl<DepthStencil> newDS;
		result::Code result = myDevice->CreateDepthStencil(
			&newDS,
			&GetDesc()
		);
		if (GRAPHICS_FAILED(result))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to update DepthStencil '%s'! Texture has a new version and DepthStencil failed to update itself.", GetName());
			return myDepthStencilView.Get();
		}
		DepthStencil_DX11Impl* newRTImpl = static_cast<DepthStencil_DX11Impl*>(newDS.Get());
		myDepthStencilView = newRTImpl->GetDepthStencilView();

		myLastTextureVersion = textureVersion;
	}

	return myDepthStencilView.Get();

#undef LOG_CATEGORY
}
