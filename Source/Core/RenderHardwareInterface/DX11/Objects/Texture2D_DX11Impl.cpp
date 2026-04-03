#include "RenderHardwareInterface.pch.h"
#include "Texture2D_DX11Impl.h"
#include "../Common_DX11.h"

graphics::Texture2D_DX11Impl::Texture2D_DX11Impl(GraphicsID aID, ID3D11Texture2D* aTexture, const Texture2DDesc& aDesc) : 
	myTexture(aTexture),
	Texture2D(aID, aDesc)
{
}

graphics::Texture2D_DX11Impl::~Texture2D_DX11Impl() = default;

void graphics::Texture2D_DX11Impl::SetName(const char* aName)
{
	Texture2D::SetName(aName);
	myTexture->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
}

void graphics::Texture2D_DX11Impl::SetTexture(ID3D11Texture2D* aTexture)
{
	myTexture = aTexture;

	_incrementVersion();

	Texture2DDesc desc = GetDesc();

	D3D11_TEXTURE2D_DESC dx11Desc = {};
	myTexture->GetDesc(&dx11Desc);

	desc.BindFlags = dx11::GetGraphicsTextureFlags(static_cast<D3D11_BIND_FLAG>(dx11Desc.BindFlags));
	desc.ResourceFlags = dx11::GetGraphicsResourceFlags(static_cast<D3D11_RESOURCE_MISC_FLAG>(dx11Desc.MiscFlags));
	desc.Width = dx11Desc.Width;
	desc.Height = dx11Desc.Height;
	desc.MipLevels = dx11Desc.MipLevels;
	desc.NumTextures = dx11Desc.ArraySize;
	desc.Data = nullptr;
	desc.NumData = 0;
	desc.Format = dx11::GetGraphicsFormat(dx11Desc.Format);
	desc.Usage = dx11::GetGraphicsUsage(dx11Desc.Usage);
	desc.CPUAccessFlags = dx11::GetGraphicsCPUAccessFlags(static_cast<D3D11_CPU_ACCESS_FLAG>(dx11Desc.CPUAccessFlags));

	SetDesc(desc);
}

ID3D11Texture2D* graphics::Texture2D_DX11Impl::GetTexture()
{
	return myTexture.Get();
}

unsigned int graphics::Texture2D_DX11Impl::_incrementVersion()
{
	SetVersion(GetVersion() + 1u);
	return GetVersion();
}
