#include "RenderHardwareInterface.pch.h"
#include "ConstantBuffer_DX11Impl.h"

graphics::ConstantBuffer_DX11Impl::ConstantBuffer_DX11Impl(GraphicsID aID, size_t aSize, ID3D11Buffer* aBuffer) : 
	myBuffer(aBuffer),
	ConstantBuffer(aID, aSize)
{
}
graphics::ConstantBuffer_DX11Impl::~ConstantBuffer_DX11Impl() = default;

void graphics::ConstantBuffer_DX11Impl::SetName(const char* aName)
{
	myBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11Buffer* graphics::ConstantBuffer_DX11Impl::GetBuffer()
{
	return myBuffer.Get();
}
