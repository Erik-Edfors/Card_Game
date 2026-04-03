#include "GraphicsEngine.pch.h"
#include "ClearDepthStencilCommand.h"
#include "../GraphicsEngine.h"
#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>
#include <Core/RenderHardwareInterface/Device.h>

ClearDepthStencilCommand::ClearDepthStencilCommand(GraphicsEngine* aEngine, graphics::DepthStencil* aDepthStencil, float aDepth, unsigned char aStencil) :
	myEngine(aEngine),
	myDepthStencil(aDepthStencil),
	myDepth(aDepth),
	myStencil(aStencil)
{
}

void ClearDepthStencilCommand::Execute()
{
	myEngine->GetDevice()->ClearDepthStencil(myDepthStencil.Get(), myDepth, myStencil);
}
