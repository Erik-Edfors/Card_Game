#pragma once
#include "GraphicsEngineDefs.h"
#include <Core/CommonUtilities/Registry.hpp>
#include <Core/RenderHardwareInterface/PImpl.hpp>

#include <map>
#include <vector>

namespace graphics
{
	class Device;
	class ConstantBuffer;
}

class GraphicsRegistry;
class PipelineState;
struct LightEntry;
struct PipelineStateDesc;

class GraphicsEngine
{
public:
	GraphicsEngine(graphics::Device* aDevice);
	~GraphicsEngine();

	void Initialize();

	graphics::Device* GetDevice();

	bool UpdateConstantBuffer(CONSTANT_BUFFER aBuffer, void* aData, size_t aSize);
	graphics::ConstantBuffer* GetConstantBuffer(CONSTANT_BUFFER aBuffer);

	GraphicsRegistry* GetGraphicsRegistry(GRAPHICS_REGISTRY aGraphicsRegistry);
	CommonUtilities::Registry<LightEntryID, LightEntry>* GetLightRegistry(LIGHT_REGISTRY aLightRegistry);
	LightEntryID CreateLightEntryID();

	PipelineState* GetState();
	PipelineState* PushState(const PipelineStateDesc& aDesc);
	PipelineState* PopState();

private:
	graphics::PImpl<graphics::Device> myDevice;
	std::map<CONSTANT_BUFFER, graphics::PImpl<graphics::ConstantBuffer>> myConstantBuffers;
	std::unordered_map<GRAPHICS_REGISTRY, std::shared_ptr<GraphicsRegistry>> myGraphicsRegistries;
	std::unordered_map<LIGHT_REGISTRY, std::shared_ptr<CommonUtilities::Registry<LightEntryID, LightEntry>>> myLightRegistries;
	LightEntryID myNextLightEntryID;
	std::vector<std::shared_ptr<PipelineState>> myPipelineStateStack;
};