#include "RenderHardwareInterface.pch.h"
#include "HullShader_DX11Impl.h"

graphics::HullShader_DX11Impl::HullShader_DX11Impl(GraphicsID aID, ID3D11HullShader* aHullShader) : 
	myHullShader(aHullShader),
	HullShader(aID)
{
}

void graphics::HullShader_DX11Impl::SetName(const char* aName)
{
	myHullShader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11HullShader* graphics::HullShader_DX11Impl::GetHullShader()
{
	return myHullShader.Get();
}
