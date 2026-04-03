#include "RenderHardwareInterface.pch.h"
#include "BlendState_DX11Impl.h"

graphics::BlendState_DX11Impl::BlendState_DX11Impl(GraphicsID aID, ID3D11BlendState* aBlendState) : 
	myBlendState(aBlendState),
	BlendState(aID)
{
}

graphics::BlendState_DX11Impl::~BlendState_DX11Impl() = default;

void graphics::BlendState_DX11Impl::SetName(const char* aName)
{
	myBlendState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11BlendState* graphics::BlendState_DX11Impl::GetBlendState()
{
	return myBlendState.Get();
}
