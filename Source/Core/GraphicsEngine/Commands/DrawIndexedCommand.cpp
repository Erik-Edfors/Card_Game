#include "GraphicsEngine.pch.h"
#include "DrawIndexedCommand.h"
#include "../GraphicsEngine.h"
#include <Core/RenderHardwareInterface/Device.h>

DrawIndexedCommand::DrawIndexedCommand(GraphicsEngine* aGraphicsEngine, unsigned int aCount, unsigned int aWhere, unsigned int aVertexWhere) :
	myGraphicsEngine(aGraphicsEngine),
	myCount(aCount),
	myWhere(aWhere),
	myVertexWhere(aVertexWhere)
{
}
DrawIndexedCommand::~DrawIndexedCommand() = default;

void DrawIndexedCommand::Execute()
{
	myGraphicsEngine->GetDevice()->DrawIndexed(myCount, myWhere, myVertexWhere);
}
