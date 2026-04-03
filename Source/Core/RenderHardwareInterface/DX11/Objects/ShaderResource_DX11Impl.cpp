#include "RenderHardwareInterface.pch.h"
#include "ShaderResource_DX11Impl.h"
#include "../../Device.h"
#include "../../Objects/Texture2D.h"

#undef FILE_CATEGORY
#define FILE_CATEGORY "ShaderResource_DX11Impl.cpp"
#undef CLASS_CATEGORY
#define CLASS_CATEGORY FILE_CATEGORY"->ShaderResource_DX11Impl"

graphics::ShaderResource_DX11Impl::ShaderResource_DX11Impl(Device* aDevice, GraphicsID aID, ID3D11ShaderResourceView* aShaderResourceView, const ShaderResourceDesc& aDesc) : 
	myDevice(aDevice),
	myShaderResourceView(aShaderResourceView),
	myLastTextureVersion(aDesc.Texture == nullptr ? 0u : aDesc.Texture->GetVersion()),
	ShaderResource(aID, aDesc)
{
}
graphics::ShaderResource_DX11Impl::~ShaderResource_DX11Impl() = default;

void graphics::ShaderResource_DX11Impl::SetName(const char* aName)
{
	myShaderResourceView->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11ShaderResourceView* graphics::ShaderResource_DX11Impl::GetShaderResourceView()
{
#define LOG_CATEGORY CLASS_CATEGORY"::GetShaderResourceView"

	unsigned int textureVersion = GetTexture() == nullptr ? 0u : GetTexture()->GetVersion();
	if (myLastTextureVersion != textureVersion)
	{
		PImpl<ShaderResource> newSR;
		result::Code result = myDevice->CreateShaderResource(
			&newSR,
			&GetDesc()
		);
		if (GRAPHICS_FAILED(result))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to update ShaderResource '%s'! Texture has a new version and ShaderResource failed to update itself.", GetName());
			return myShaderResourceView.Get();
		}
		ShaderResource_DX11Impl* newRTImpl = static_cast<ShaderResource_DX11Impl*>(newSR.Get());
		myShaderResourceView = newRTImpl->GetShaderResourceView();

		myLastTextureVersion = textureVersion;
	}

	return myShaderResourceView.Get();

#undef LOG_CATEGORY
}
