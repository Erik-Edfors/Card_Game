#pragma once
//#include <Core/RenderHardwareInterface/GraphicsDefs.h>
//#include <Core/RenderHardwareInterface/PImpl.hpp>
//
//namespace graphics
//{
//	class RenderTarget;
//	class PipelineState;
//}
//
//class GraphicsEngine;
//
//class SetRenderTargetsCommand
//{
//public:
//	SetRenderTargetsCommand(graphics::PipelineState* aState, graphics::RenderTarget* const* aRenderTargets, size_t aNumRenderTargets);
//
//	void Execute();
//
//private:
//	graphics::PImpl<graphics::PipelineState> myState;
//	graphics::PImpl<graphics::RenderTarget> myRenderTargets[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS];
//	size_t myNumRenderTargets;
//};