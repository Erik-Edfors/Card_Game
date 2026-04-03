#include "RenderHardwareInterface.pch.h"
#include "GeometryShader_DX11Impl.h"

graphics::GeometryShader_DX11Impl::GeometryShader_DX11Impl(GraphicsID aID, ID3D11GeometryShader* aGeometryShader) : 
	myGeometryShader(aGeometryShader),
	GeometryShader(aID)
{
}

void graphics::GeometryShader_DX11Impl::SetName(const char* aName)
{
	myGeometryShader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11GeometryShader* graphics::GeometryShader_DX11Impl::GetGeometryShader()
{
	return myGeometryShader.Get();
}
