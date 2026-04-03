#include "RenderHardwareInterface.pch.h"
#include "PixelShader_DX11Impl.h"

graphics::PixelShader_DX11Impl::PixelShader_DX11Impl(GraphicsID aID, ID3D11PixelShader* aPixelShader) : 
	myPixelShader(aPixelShader),
	PixelShader(aID)
{
}

void graphics::PixelShader_DX11Impl::SetName(const char* aName)
{
	myPixelShader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11PixelShader* graphics::PixelShader_DX11Impl::GetPixelShader()
{
	return myPixelShader.Get();
}
