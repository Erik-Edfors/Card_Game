#include "GraphicsEngine.pch.h"
#include "DrawCommand.h"
#include "../GraphicsEngine.h"
#include <Core/RenderHardwareInterface/Device.h>

DrawCommand::DrawCommand(GraphicsEngine* aGraphicsEngine, unsigned int aCount, unsigned int aWhere) : 
	myGraphicsEngine(aGraphicsEngine),
	myCount(aCount),
	myWhere(aWhere)
{
}

DrawCommand::~DrawCommand() = default;

void DrawCommand::Execute()
{
	myGraphicsEngine->GetDevice()->Draw(myCount, myWhere);
}
