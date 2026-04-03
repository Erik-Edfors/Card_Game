#pragma once
#include <Core/RenderHardwareInterface/PImpl.hpp>

namespace graphics
{
	class DepthStencil;
}

class GraphicsEngine;

class ClearDepthStencilCommand
{
public:
	ClearDepthStencilCommand(GraphicsEngine* aEngine, graphics::DepthStencil* aDepthStencil, float aDepth = 1.f, unsigned char aStencil = 0u);

	void Execute();

private:
	GraphicsEngine* myEngine;
	graphics::PImpl<graphics::DepthStencil> myDepthStencil;
	float myDepth;
	unsigned char myStencil;
};