#include "RenderHardwareInterface.pch.h"
#include "DepthStencil.h"
#include "Texture2D.h"


graphics::DepthStencil::DepthStencil(GraphicsID aID, const DepthStencilDesc& aDesc) :
	myDesc(aDesc),
	myTexture(aDesc.Texture),
	GraphicsObject(aID)
{
}

graphics::DepthStencil::~DepthStencil() = default;

graphics::Texture2D* graphics::DepthStencil::GetTexture()
{
	return myTexture.Get();
}

const graphics::DepthStencilDesc& graphics::DepthStencil::GetDesc() const
{
	return myDesc;
}

