#include "RenderHardwareInterface.pch.h"
#include "ComputeShader_DX11Impl.h"

graphics::ComputeShader_DX11Impl::ComputeShader_DX11Impl(GraphicsID aID, ID3D11ComputeShader* aComputeShader) : 
	myComputeShader(aComputeShader),
	ComputeShader(aID)
{
}

void graphics::ComputeShader_DX11Impl::SetName(const char* aName)
{
	myComputeShader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11ComputeShader* graphics::ComputeShader_DX11Impl::GetComputeShader()
{
	return myComputeShader.Get();
}
