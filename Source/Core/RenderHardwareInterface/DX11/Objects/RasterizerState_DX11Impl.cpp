#include "RenderHardwareInterface.pch.h"
#include "RasterizerState_DX11Impl.h"

graphics::RasterizerState_DX11Impl::RasterizerState_DX11Impl(GraphicsID aID, ID3D11RasterizerState* aRasterizerState, const RasterizerStateDesc& aDesc) :
	myRasterizerState(aRasterizerState),
	RasterizerState(aID, aDesc)
{
}

graphics::RasterizerState_DX11Impl::~RasterizerState_DX11Impl() = default;

void graphics::RasterizerState_DX11Impl::SetName(const char* aName)
{
	myRasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11RasterizerState* graphics::RasterizerState_DX11Impl::GetRasterizerState()
{
	return myRasterizerState.Get();
}

const ID3D11RasterizerState* graphics::RasterizerState_DX11Impl::GetRasterizerState() const
{
	return myRasterizerState.Get();
}
