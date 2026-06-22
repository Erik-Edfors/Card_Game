#pragma once
#include "GraphicsEngineDefs.h"
#include <Core/CommonUtilities/Registry.hpp>
#include <Core/RenderHardwareInterface/PImpl.hpp>

#include <map>
#include <vector>

namespace graphics
{
	class Device;
	class ImmediateDeviceContext;
	class ConstantBuffer;
}

class GraphicsRegistry;
class PipelineState;
class RenderLayer;
struct LightEntry;
struct PipelineStateDesc;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Initialize(graphics::Device* aDevice);

	void Render();

	graphics::Device* GetDevice() const noexcept;
	graphics::ImmediateDeviceContext* GetDeviceContext() const noexcept;

	bool UpdateConstantBuffer(CONSTANT_BUFFER aBuffer, void* aData, size_t aSize);
	graphics::ConstantBuffer* GetConstantBuffer(CONSTANT_BUFFER aBuffer);

	GraphicsRegistry* GetGraphicsRegistry(GRAPHICS_REGISTRY aGraphicsRegistry);
	CommonUtilities::Registry<LightEntryID, LightEntry>* GetLightRegistry(LIGHT_REGISTRY aLightRegistry);
	LightEntryID CreateLightEntryID();

	//PipelineState* GetState();
	//PipelineState* PushState(const PipelineStateDesc& aDesc);
	//PipelineState* PopState();

	template<class _RenderLayerTy>
	_RenderLayerTy* AddRenderLayer();

	template<class _RenderLayerTy>
	_RenderLayerTy* GetRenderLayer() const noexcept;

	template<class _RenderLayerTy>
	void EraseRenderLayer();

private:
	graphics::PImpl<graphics::Device> myDevice;
	graphics::PImpl<graphics::ImmediateDeviceContext> myDeviceContext;

	std::map<CONSTANT_BUFFER, graphics::PImpl<graphics::ConstantBuffer>> myConstantBuffers;
	std::unordered_map<GRAPHICS_REGISTRY, std::shared_ptr<GraphicsRegistry>> myGraphicsRegistries;
	std::unordered_map<LIGHT_REGISTRY, std::shared_ptr<CommonUtilities::Registry<LightEntryID, LightEntry>>> myLightRegistries;
	std::vector<std::unique_ptr<RenderLayer>> myRenderLayers;
	LightEntryID myNextLightEntryID;
	//std::vector<std::shared_ptr<PipelineState>> myPipelineStateStack;
};

template<class _RenderLayerTy>
inline _RenderLayerTy* GraphicsEngine::AddRenderLayer()
{
	myRenderLayers.push_back(std::make_unique<_RenderLayerTy>());

	myRenderLayers.back()->Initialize(this);

	return static_cast<_RenderLayerTy*>(myRenderLayers.back().get());
}

template<class _RenderLayerTy>
inline _RenderLayerTy* GraphicsEngine::GetRenderLayer() const noexcept
{
	for (const std::unique_ptr<RenderLayer>& renderLayer : myRenderLayers)
	{
		_RenderLayerTy* result = dynamic_cast<_RenderLayerTy*>(renderLayer.get());
		if (result != nullptr)
		{
			return result;
		}
	}
	return nullptr;
}

template<class _RenderLayerTy>
inline void GraphicsEngine::EraseRenderLayer()
{
	std::erase_if(
		myRenderLayers,
		[](const std::unique_ptr<RenderLayer>& aRenderLayer) {
			return dynamic_cast<_RenderLayerTy*>(aRenderLayer.get()) != nullptr;
		}
	);
}
