#include "RenderHardwareInterface.pch.h"
#include "Texture2DArray_DX11Impl.h"

graphics::Texture2DArray_DX11Impl::Texture2DArray_DX11Impl(GraphicsID aID, ID3D11Texture2D* aTexture, GRAPHICS_BIND_FLAGS aFlags, unsigned int aSize) :
	myTexture(aTexture),
	Texture2DArray(aID, aFlags, aSize)
{
}
graphics::Texture2DArray_DX11Impl::~Texture2DArray_DX11Impl() = default;

void graphics::Texture2DArray_DX11Impl::SetName(const char* aName)
{
	myTexture->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11Texture2D* graphics::Texture2DArray_DX11Impl::GetTexture()
{
	return myTexture.Get();
}

graphics::result::Code graphics::Texture2DArray_DX11Impl::GetSize(unsigned int* outWidth, unsigned int* outHeight) const
{
	D3D11_TEXTURE2D_DESC desc = {};
	myTexture->GetDesc(&desc);

	if (outWidth)
	{
		*outWidth = desc.Width;
	}
	if (outHeight)
	{
		*outHeight = desc.Height;
	}

	return graphics::result::OK;
}
