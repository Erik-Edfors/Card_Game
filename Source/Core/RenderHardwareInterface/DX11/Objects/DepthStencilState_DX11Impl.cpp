#include "RenderHardwareInterface.pch.h"
#include "DepthStencilState_DX11Impl.h"

graphics::DepthStencilState_DX11Impl::DepthStencilState_DX11Impl(GraphicsID aID, ID3D11DepthStencilState* aDepthStencilState, const DepthStencilStateDesc& aDesc) : 
	myDepthStencilState(aDepthStencilState),
	DepthStencilState(aID, aDesc)
{
}
graphics::DepthStencilState_DX11Impl::~DepthStencilState_DX11Impl() = default;

void graphics::DepthStencilState_DX11Impl::SetName(const char* aName)
{
	myDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11DepthStencilState* graphics::DepthStencilState_DX11Impl::GetDepthStencilState()
{
	return myDepthStencilState.Get();
}

const ID3D11DepthStencilState* graphics::DepthStencilState_DX11Impl::GetDepthStencilState() const
{
	return myDepthStencilState.Get();
}
