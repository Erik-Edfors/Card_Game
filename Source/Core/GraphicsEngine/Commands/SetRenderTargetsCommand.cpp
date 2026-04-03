#include "GraphicsEngine.pch.h"
#include "SetRenderTargetsCommand.h"
//
//SetRenderTargetsCommand::SetRenderTargetsCommand(graphics::PipelineState* aState, graphics::RenderTarget* const* aRenderTargets, size_t aNumRenderTargets) :
//	myState(aState),
//	myNumRenderTargets(aNumRenderTargets)
//{
//	for (size_t i = 0; i < aNumRenderTargets; i++)
//	{
//		myRenderTargets[i] = aRenderTargets[i];
//	}
//}
//
//void SetRenderTargetsCommand::Execute()
//{
//	graphics::RenderTarget* renderTargets[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS] = {};
//	for (size_t i = 0; i < myNumRenderTargets; i++)
//	{
//		renderTargets[i] = myRenderTargets[i].Get();
//	}
//
//	myState->SetRenderTargets(renderTargets, myNumRenderTargets);
//}
