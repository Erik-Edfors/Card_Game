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
	virtual void OnRender() {};
	virtual void OnPreRender() {};
	virtual void OnPostRender() {};

	graphics::DeferredDeviceContext* GetContext() const noexcept;

private:
	void Initialize(GraphicsEngine* aGraphicsEngine);

	void PreRender();
	void PostRender();
	void Render();

	graphics::PImpl<graphics::DeferredDeviceContext> myContext;
	GraphicsEngine* myGraphicsEngine;
};