#include "RenderHardwareInterface.pch.h"
#include "DepthStencilState.h"

graphics::DepthStencilState::DepthStencilState(GraphicsID aID, const DepthStencilStateDesc& aDesc) : 
	myDesc(aDesc),
	GraphicsObject(aID)
{
}

graphics::DepthStencilState::~DepthStencilState() = default;

const graphics::DepthStencilStateDesc& graphics::DepthStencilState::GetDesc() const
{
	return myDesc;
}
