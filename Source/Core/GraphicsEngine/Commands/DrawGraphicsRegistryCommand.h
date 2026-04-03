#pragma once
#include <Core/GraphicsEngine/GraphicsRegistry.h>
#include <Core/GraphicsEngine/Objects/Camera.h>

class GraphicsEngine;
class GraphicsRegistry;

class DrawGraphicsRegistryCommand
{
public:
	DrawGraphicsRegistryCommand(GraphicsEngine* aGraphicsEngine, GraphicsRegistry* aGraphicsRegistry, const gengine::Camera& aCamera, GRAPHICS_REGISTRY_DRAW_FLAG aFlags);
	~DrawGraphicsRegistryCommand();

	void Execute();

private:
	GraphicsEngine* myGraphicsEngine;
	GraphicsRegistry* myGraphicsRegistry;
	gengine::Camera myCamera;
	GRAPHICS_REGISTRY_DRAW_FLAG myFlags;
};