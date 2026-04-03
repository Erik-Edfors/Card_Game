#include "GraphicsEngine.pch.h"
#include "DrawGraphicsRegistryCommand.h"

DrawGraphicsRegistryCommand::DrawGraphicsRegistryCommand(GraphicsEngine* aGraphicsEngine, GraphicsRegistry* aGraphicsRegistry, const gengine::Camera& aCamera, GRAPHICS_REGISTRY_DRAW_FLAG aFlags) :
	myGraphicsEngine(aGraphicsEngine),
	myGraphicsRegistry(aGraphicsRegistry),
	myCamera(aCamera),
	myFlags(aFlags)
{
}

DrawGraphicsRegistryCommand::~DrawGraphicsRegistryCommand() = default;

void DrawGraphicsRegistryCommand::Execute()
{
	myGraphicsRegistry->Draw(myGraphicsEngine, myCamera, myFlags);
}
