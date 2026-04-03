#include "RenderHardwareInterface.pch.h"
#include "SamplerState_DX11Impl.h"

graphics::SamplerState_DX11Impl::SamplerState_DX11Impl(GraphicsID aID, ID3D11SamplerState* aSamplerState) : 
	mySamplerState(aSamplerState),
	SamplerState(aID)
{
}

void graphics::SamplerState_DX11Impl::SetName(const char* aName)
{
	mySamplerState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11SamplerState* graphics::SamplerState_DX11Impl::GetSamplerState()
{
	return mySamplerState.Get();
}

const ID3D11SamplerState* graphics::SamplerState_DX11Impl::GetSamplerState() const
{
	return mySamplerState.Get();
}
