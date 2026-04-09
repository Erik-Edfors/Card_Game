#pragma once
#include "Component.h"
#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>

class SpriteComponent : Component
{
public:

protected:
	//void OnInit() override;
	//void OnUpdate() override;
private:
	SpriteRenderLayer* myRenderLayer;
};