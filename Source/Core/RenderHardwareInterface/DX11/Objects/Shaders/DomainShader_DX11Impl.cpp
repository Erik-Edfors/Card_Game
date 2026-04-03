#include "RenderHardwareInterface.pch.h"
#include "DomainShader_DX11Impl.h"

graphics::DomainShader_DX11Impl::DomainShader_DX11Impl(GraphicsID aID, ID3D11DomainShader* aDomainShader) : 
	myDomainShader(aDomainShader),
	DomainShader(aID)
{
}

void graphics::DomainShader_DX11Impl::SetName(const char* aName)
{
	myDomainShader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11DomainShader* graphics::DomainShader_DX11Impl::GetDomainShader()
{
	return myDomainShader.Get();
}
