#include "GraphicsEngine.pch.h"
#include "RenderLayer.h"
#include "GraphicsEngine.h"
#include "Objects/PipelineState.h"

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/DeferredDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/ImmediateDeviceContext.h>

RenderLayer::RenderLayer() : 
	myGraphicsEngine(nullptr)
{
}
RenderLayer::~RenderLayer()
{
}
GraphicsEngine* RenderLayer::GetGraphicsEngine() const noexcept
{
	return myGraphicsEngine;
}
graphics::DeferredDeviceContext* RenderLayer::GetContext() const noexcept
{
	return myContext.Get();
}
void RenderLayer::Initialize(GraphicsEngine* aGraphicsEngine)
{
	myGraphicsEngine = aGraphicsEngine;
	aGraphicsEngine->GetDevice()->CreateDeferredDeviceContext(&myContext);
}
void RenderLayer::Prepare()
{
	OnPrepare();
}

void RenderLayer::Render()
{
	OnRender();
}

