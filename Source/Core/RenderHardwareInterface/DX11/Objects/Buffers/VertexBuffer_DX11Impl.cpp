#include "RenderHardwareInterface.pch.h"
#include "VertexBuffer_DX11Impl.h"

graphics::VertexBuffer_DX11Impl::VertexBuffer_DX11Impl(GraphicsID aID, size_t aSize, ID3D11Buffer* aBuffer) : 
	myBuffer(aBuffer),
	VertexBuffer(aID, aSize)
{
}
graphics::VertexBuffer_DX11Impl::~VertexBuffer_DX11Impl() = default;

void graphics::VertexBuffer_DX11Impl::SetName(const char* aName)
{
	myBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11Buffer* graphics::VertexBuffer_DX11Impl::GetBuffer()
{
	return myBuffer.Get();
}
