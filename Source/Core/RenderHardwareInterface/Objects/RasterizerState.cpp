#include "RenderHardwareInterface.pch.h"
#include "RasterizerState.h"

graphics::RasterizerState::RasterizerState(GraphicsID aID, const RasterizerStateDesc& aDesc) : 
	myDesc(aDesc),
	GraphicsObject(aID)
{
}
