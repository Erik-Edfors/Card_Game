#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include "GraphicsRegistry.h"

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>

#include "Objects/LightEntry.h"
#include "Objects/PipelineState.h"

#include "ConstantBuffers/FrameBuffer.h"
#include "ConstantBuffers/ObjectBuffer.h"
#include "ConstantBuffers/MaterialBuffer.h"
#include "ConstantBuffers/AnimationBuffer.h"
#include "ConstantBuffers/LightBuffer.h"
#include "ConstantBuffers/ShadowMapBuffer.h"
#include "ConstantBuffers/BloomBuffer.h"
#include "ConstantBuffers/BlurBuffer.h"

#include "Objects/Vertex.h"

#include "Common.h"

GraphicsEngine::GraphicsEngine(graphics::Device* aDevice) :
	myDevice(aDevice),
	myNextLightEntryID(0)
	//myRenderPass(nullptr)
{
}
GraphicsEngine::~GraphicsEngine() = default;

void GraphicsEngine::Initialize()
{
	//////////////////
	//// Constant Buffers
	//////////////////

	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Frame Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(FrameBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_FRAME],
			&cbDesc
		);
	}

	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Object Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(ObjectBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_OBJECT],
			&cbDesc
		);
	}

	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Material Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(MaterialBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_MATERIAL],
			&cbDesc
		);
	}

	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Animation Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(AnimationBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_ANIMATION],
			&cbDesc
		);
	}

	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Light Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(LightBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_LIGHT],
			&cbDesc
		);
	}

	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Shadow Map Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(ShadowMapBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_SHADOW_MAP],
			&cbDesc
		);
	}
	
	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Bloom Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(BloomBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_BLOOM],
			&cbDesc
		);
	}

	{
		graphics::ConstantBufferDesc cbDesc = {};
		cbDesc.Name = "Blur Buffer";
		cbDesc.InitialData = nullptr;
		cbDesc.Size = sizeof(BlurBuffer);
		cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
		myDevice->CreateConstantBuffer(
			&myConstantBuffers[CONSTANT_BUFFER_BLUR],
			&cbDesc
		);
	}
}

graphics::Device* GraphicsEngine::GetDevice()
{
	return myDevice.Get();
}

bool GraphicsEngine::UpdateConstantBuffer(CONSTANT_BUFFER aBuffer, void* aData, size_t aSize)
{
	auto buffer = myConstantBuffers.find(aBuffer);
	if (buffer != myConstantBuffers.end())
	{
		graphics::result::Code result = myDevice->UpdateConstantBuffer(buffer->second.Get(), aData, aSize);
		if (GRAPHICS_SUCCEEDED(result))
		{
			return true;
		}
	}
	return false;
}

graphics::ConstantBuffer* GraphicsEngine::GetConstantBuffer(CONSTANT_BUFFER aBuffer)
{
	auto entry = myConstantBuffers.find(aBuffer);
	if (entry != myConstantBuffers.end())
	{
		return entry->second.Get();
	}
	else return nullptr;
}

GraphicsRegistry* GraphicsEngine::GetGraphicsRegistry(GRAPHICS_REGISTRY aGraphicsRegistry)
{
	if (aGraphicsRegistry == GRAPHICS_REGISTRY_INVALID)
	{
		return nullptr;
	}

	auto entry = myGraphicsRegistries.find(aGraphicsRegistry);
	if (entry == myGraphicsRegistries.end())
	{
		std::shared_ptr<GraphicsRegistry> graphicsRegistry = std::make_shared<GraphicsRegistry>();
		myGraphicsRegistries.emplace(aGraphicsRegistry, graphicsRegistry);
		return graphicsRegistry.get();
	}
	else
	{
		return entry->second.get();
	}
}

CommonUtilities::Registry<LightEntryID, LightEntry>* GraphicsEngine::GetLightRegistry(LIGHT_REGISTRY aLightRegistry)
{
	if (aLightRegistry == LIGHT_REGISTRY_INVALID)
	{
		return nullptr;
	}

	auto entry = myLightRegistries.find(aLightRegistry);
	if (entry == myLightRegistries.end())
	{
		std::shared_ptr<CommonUtilities::Registry<LightEntryID, LightEntry>> lightRegistry = std::make_shared<CommonUtilities::Registry<LightEntryID, LightEntry>>();
		myLightRegistries.emplace(aLightRegistry, lightRegistry);
		return lightRegistry.get();
	}
	else
	{
		return entry->second.get();
	}
}

LightEntryID GraphicsEngine::CreateLightEntryID()
{
	return myNextLightEntryID++;
}

//RenderPass* GraphicsEngine::GetRenderPass() const
//{
//	return myRenderPass;
//}
//
//void GraphicsEngine::SetRenderPass(RenderPass* aPipelineState)
//{
//	if (myRenderPass != nullptr)
//	{
//		myRenderPass->Unbind(this);
//	}
//	myRenderPass = aPipelineState;
//	if (myRenderPass != nullptr)
//	{
//		myRenderPass->Bind(this);
//	}
//}

//bool GraphicsEngine::BeginRender()
//{
//	if (myRenderPass == nullptr)
//	{
//		return false;
//	}
//
//	//TODO: Find better way to implement constant buffers
//	for (auto& entry : myConstantBuffers)
//	{
//		myDevice->BindConstantBuffers(&entry.second, 1, entry.first, graphics::GRAPHICS_SHADER_STAGE_VERTEX | graphics::GRAPHICS_SHADER_STAGE_PIXEL);
//	}
//
//	myRenderPass->BeginRender(this);
//	return true;
//}
//
//bool GraphicsEngine::EndRender()
//{
//	if (myRenderPass == nullptr)
//	{
//		return false;
//	}
//
//	myRenderPass->EndRender(this);
//
//	graphics::ConstantBuffer* dummy = nullptr;
//
//	//TODO: Find better way to implement constant buffers
//	for (auto& entry : myConstantBuffers)
//	{
//		myDevice->BindConstantBuffers(&dummy, 1, entry.first, graphics::GRAPHICS_SHADER_STAGE_VERTEX | graphics::GRAPHICS_SHADER_STAGE_PIXEL);
//	}
//	return true;
//}

//bool GraphicsEngine::RenderPipelineState()
//{
//	if (myPipelineState == nullptr)
//	{
//		return false;
//	}
//
//	myPipelineState->Render(myDevice.Get());
//
//	return true;
//}

//void GraphicsEngine::ResetPipelineState()
//{
//	if (myRenderPass != nullptr)
//	{
//		myRenderPass->Unbind(this);
//		myRenderPass = nullptr;
//	}
//}

PipelineState* GraphicsEngine::GetState()
{
	return myPipelineStateStack.empty() ? nullptr : myPipelineStateStack.back().get();
}

PipelineState* GraphicsEngine::PushState(const PipelineStateDesc& aDesc)
{
	if (not myPipelineStateStack.empty())
	{
		myPipelineStateStack.back()->Unbind(myDevice.Get());
	}
	myPipelineStateStack.push_back(std::move(std::make_shared<PipelineState>(aDesc)));
	myPipelineStateStack.back()->Bind(myDevice.Get());
	return myPipelineStateStack.back().get();
}

PipelineState* GraphicsEngine::PopState()
{
	if (myPipelineStateStack.empty())
	{
		return nullptr;
	}
	myPipelineStateStack.back()->Unbind(myDevice.Get());
	myPipelineStateStack.pop_back();
	return myPipelineStateStack.empty() ? nullptr : myPipelineStateStack.back().get();
}

//graphics::PipelineState* GraphicsEngine::GetPipelineState(PIPELINE_STATE aPipelineState)
//{
//	auto entry = myPipelineStates.find(aPipelineState);
//	if (entry != myPipelineStates.end())
//	{
//		return entry->second.Get();
//	}
//	else return nullptr;
//}
//
//bool GraphicsEngine::SetState(PIPELINE_STATE aPipelineState)
//{
//	ResetState();
//	if (aPipelineState == PIPELINE_STATE_NONE)
//	{
//		return true;
//	}
//
//	auto entry = myPipelineStates.find(aPipelineState);
//	if (entry != myPipelineStates.end())
//	{
//		myBoundState = aPipelineState;
//		graphics::result::Code result = entry->second->Bind(myDevice.Get());
//		return GRAPHICS_SUCCEEDED(result);
//	}
//	return false;
//}
//
//PIPELINE_STATE GraphicsEngine::GetState() const
//{
//	return myBoundState;
//}
//
//bool GraphicsEngine::ResetState()
//{
//	if (myBoundState != PIPELINE_STATE_NONE)
//	{
//		graphics::result::Code result = myPipelineStates[myBoundState]->Unbind(myDevice.Get());
//		myBoundState = PIPELINE_STATE_NONE;
//		return GRAPHICS_SUCCEEDED(result);
//	}
//	return false;
//}
