#include "RenderHardwareInterface.pch.h"
#include "VertexShader_DX11Impl.h"

graphics::VertexShader_DX11Impl::VertexShader_DX11Impl(GraphicsID aID, ID3D11VertexShader* aVertexShader) : 
	myVertexShader(aVertexShader),
	VertexShader(aID)
{
}

void graphics::VertexShader_DX11Impl::SetName(const char* aName)
{
	myVertexShader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11VertexShader* graphics::VertexShader_DX11Impl::GetVertexShader()
{
	return myVertexShader.Get();
}
