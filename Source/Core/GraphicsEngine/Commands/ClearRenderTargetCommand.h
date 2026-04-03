#pragma once
#include <Core/RenderHardwareInterface/PImpl.hpp>

namespace graphics
{
	class RenderTarget;
}

class GraphicsEngine;

class ClearRenderTargetCommand
{
public:
	ClearRenderTargetCommand(GraphicsEngine* aEngine, graphics::RenderTarget* aRenderTarget, float aColor[4]);

	void Execute();

private:
	GraphicsEngine* myEngine;
	graphics::PImpl<graphics::RenderTarget> myRenderTarget;
	float myColor[4];
};