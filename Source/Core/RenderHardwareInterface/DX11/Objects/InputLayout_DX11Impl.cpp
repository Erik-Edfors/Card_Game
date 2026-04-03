#include "RenderHardwareInterface.pch.h"
#include "InputLayout_DX11Impl.h"

graphics::InputLayout_DX11Impl::InputLayout_DX11Impl(GraphicsID aID, const LayoutDefinition& aLayoutDefinition, ID3D11InputLayout* aInputLayout) :
	myInputLayout(aInputLayout),
	InputLayout(aID, aLayoutDefinition)
{
}
graphics::InputLayout_DX11Impl::~InputLayout_DX11Impl() = default;

void graphics::InputLayout_DX11Impl::SetName(const char* aName)
{
	myInputLayout->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	SetLocalName(aName);
}

ID3D11InputLayout* graphics::InputLayout_DX11Impl::GetInputLayout()
{
	return myInputLayout.Get();
}

const ID3D11InputLayout* graphics::InputLayout_DX11Impl::GetInputLayout() const
{
	return myInputLayout.Get();
}
