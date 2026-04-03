#include "RenderHardwareInterface.pch.h"
#include "IndexBuffer_DX11Impl.h"

graphics::IndexBuffer_DX11Impl::IndexBuffer_DX11Impl(GraphicsID aID, size_t aSize, ID3D11Buffer* aBuffer) : 
	myBuffer(aBuffer),
	IndexBuffer(aID, aSize)
{
}
graphics::IndexBuffer_DX11Impl::~IndexBuffer_DX11Impl() = default;

void graphics::IndexBuffer_DX11Impl::SetName(const char* aName)
{
	myBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11Buffer* graphics::IndexBuffer_DX11Impl::GetBuffer()
{
	return myBuffer.Get();
}
