#pragma once
#include <Core/RenderHardwareInterface/PImpl.hpp>

class GraphicsEngine;
struct PipelineStateDesc;

namespace graphics
{
	class Device;
	class ImmediateDeviceContext;
	class DeferredDeviceContext;
}

class RenderLayer
{
	friend class GraphicsEngine;
public:
	RenderLayer();
	virtual ~RenderLayer();

	GraphicsEngine* GetGraphicsEngine() const noexcept;

protected:
	virtual void OnInit() {};
	virtual void OnPrepare() {};
	virtual void OnRender() {};

	graphics::DeferredDeviceContext* GetContext() const noexcept;

private:
	void Initialize(GraphicsEngine* aGraphicsEngine);

	void Prepare();
	void Render();

	graphics::PImpl<graphics::DeferredDeviceContext> myContext;
	GraphicsEngine* myGraphicsEngine;
};