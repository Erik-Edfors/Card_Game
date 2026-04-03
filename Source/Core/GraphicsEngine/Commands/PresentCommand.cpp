#include "GraphicsEngine.pch.h"
#include "PresentCommand.h"
#include "../GraphicsEngine.h"
#include <Core/RenderHardwareInterface/Device.h>

PresentCommand::PresentCommand(GraphicsEngine* aEngine, graphics::GRAPHICS_PRESENT_FLAGS aFlags) : 
	myEngine(aEngine),
	myFlags(aFlags)
{
}

void PresentCommand::Execute()
{
	//myEngine->ResetState();
	myEngine->GetDevice()->Present(myFlags);
}
