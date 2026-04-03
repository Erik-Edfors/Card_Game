#include "RenderHardwareInterface.pch.h"
#include "RenderTarget.h"
#include "Texture2D.h"

graphics::RenderTarget::RenderTarget(GraphicsID aID, const RenderTargetDesc& aDesc) :
	myDesc(aDesc),
	myTexture(aDesc.Texture),
	GraphicsObject(aID)
{
}

graphics::RenderTarget::~RenderTarget() = default;

graphics::Texture2D* graphics::RenderTarget::GetTexture()
{
	return myTexture.Get();
}
const graphics::RenderTargetDesc& graphics::RenderTarget::GetDesc() const
{
	return myDesc;
}
