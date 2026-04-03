#include "GraphicsEngine.pch.h"
#include "ClearRenderTargetCommand.h"
#include "../GraphicsEngine.h"
#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/RenderTarget.h>

ClearRenderTargetCommand::ClearRenderTargetCommand(GraphicsEngine* aEngine, graphics::RenderTarget* aRenderTarget, float aColor[4]) : 
	myEngine(aEngine),
	myRenderTarget(aRenderTarget)
{
	for (size_t i = 0; i < 4; i++)
	{
		myColor[i] = aColor[i];
	}
}

void ClearRenderTargetCommand::Execute()
{
	myEngine->GetDevice()->ClearRenderTarget(myRenderTarget.Get(), myColor);
}
