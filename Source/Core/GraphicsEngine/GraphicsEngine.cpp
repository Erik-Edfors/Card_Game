#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include "GraphicsRegistry.h"

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/imgui.h>
#include <Core/RenderHardwareInterface/Objects/ImmediateDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>

#include "Objects/LightEntry.h"
#include "Objects/PipelineState.h"

#include "RenderLayers/RenderLayer.h"

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

#define FILE_CATEGORY "GraphicsEngine.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->GraphicsEngine"

GraphicsEngine::GraphicsEngine() :
	myDevice(nullptr),
	myNextLightEntryID(0),
	myVSyncEnabled(true)
	//myRenderPass(nullptr)
{
}
GraphicsEngine::~GraphicsEngine() = default;

bool GraphicsEngine::Initialize(graphics::Device* aDevice)
{
#define LOG_CATEGORY CLASS_CATEGORY"::Initialize"

	myDevice = aDevice;

	if (myDevice == nullptr)
	{
		LOG_ERROR(LOG_CATEGORY, "Failed to initialize GraphicsEngine! aDevice was nullptr!");
		return false;
	}

	GetDevice()->CreateImmediateDeviceContext(&myDeviceContext);

	graphics::ImGuiInitialize(myDevice.Get());

	//////////////////
	//// Constant Buffers
	//////////////////

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Frame Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(FrameBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_FRAME],
	//		&cbDesc
	//	);
	//}

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Object Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(ObjectBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_OBJECT],
	//		&cbDesc
	//	);
	//}

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Material Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(MaterialBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_MATERIAL],
	//		&cbDesc
	//	);
	//}

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Animation Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(AnimationBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_ANIMATION],
	//		&cbDesc
	//	);
	//}

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Light Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(LightBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_LIGHT],
	//		&cbDesc
	//	);
	//}

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Shadow Map Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(ShadowMapBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_SHADOW_MAP],
	//		&cbDesc
	//	);
	//}

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Bloom Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(BloomBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_BLOOM],
	//		&cbDesc
	//	);
	//}

	//{
	//	graphics::ConstantBufferDesc cbDesc = {};
	//	cbDesc.Name = "Blur Buffer";
	//	cbDesc.InitialData = nullptr;
	//	cbDesc.Size = sizeof(BlurBuffer);
	//	cbDesc.Usage = graphics::GRAPHICS_USAGE_DYNAMIC;
	//	cbDesc.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE;
	//	myDevice->CreateConstantBuffer(
	//		&myConstantBuffers[CONSTANT_BUFFER_BLUR],
	//		&cbDesc
	//	);
	//}

	return true;

#undef LOG_CATEGORY
}

bool GraphicsEngine::Shutdown()
{
	graphics::ImGuiShutdown();

	myRenderLayers.clear();
	myConstantBuffers.clear();
	myGraphicsRegistries.clear();
	myLightRegistries.clear();
	myNextLightEntryID = 0;
	myDeviceContext.Reset();
	myDevice.Reset();
	return true;
}

void GraphicsEngine::SetVSyncEnabled(bool aEnabled) noexcept
{
	myVSyncEnabled = aEnabled;
}

bool GraphicsEngine::IsVSyncEnabled() const noexcept
{
	return myVSyncEnabled;
}

void GraphicsEngine::BeginFrame()
{
	graphics::ImGuiNewFrame();
}

void GraphicsEngine::EndFrame()
{
	const float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	graphics::PImpl<graphics::RenderTarget> backbufferRenderTarget;
	GetDevice()->GetBufferRenderTarget(&backbufferRenderTarget);
	GetDevice()->ClearRenderTarget(backbufferRenderTarget.Get(), clearColor);

	graphics::PImpl<graphics::DepthStencil> backbufferDepthStencil;
	GetDevice()->GetBufferDepthStencil(&backbufferDepthStencil);
	GetDevice()->ClearDepthStencil(backbufferDepthStencil.Get());

	const graphics::Texture2DDesc& backbufferTextureDesc = backbufferRenderTarget->GetDesc().Texture->GetDesc();

	for (std::unique_ptr<RenderLayer>& layer : myRenderLayers)
	{
		layer->PreRender();
	}

	//TODO: Run on seperate threads
	for (std::unique_ptr<RenderLayer>& layer : myRenderLayers)
	{
		layer->Render();
	}

	for (std::unique_ptr<RenderLayer>& layer : myRenderLayers)
	{
		GetDeviceContext()->ExecuteDeferredDeviceContext(layer->GetContext(), false);
	}

	for (std::unique_ptr<RenderLayer>& layer : myRenderLayers)
	{
		layer->PostRender();
	}

	graphics::GRAPHICS_PRESENT_FLAGS flags = graphics::GRAPHICS_PRESENT_NONE;

	if (not IsVSyncEnabled())
	{
		flags |= graphics::GRAPHICS_PRESENT_ALLOW_TEARING;
	}

	GetDeviceContext()->ClearState();

	GetDeviceContext()->BindTargets(&backbufferRenderTarget, 1, backbufferDepthStencil.Get());
	graphics::Viewport viewport = {
		.Left = 0.f,
		.Right = static_cast<float>(backbufferTextureDesc.Width),
		.Top = 0.f,
		.Bottom = static_cast<float>(backbufferTextureDesc.Height),
		.Front = 0.f,
		.Back = 1.f
	};
	GetDeviceContext()->SetViewports(&viewport, 1);

	graphics::ImGuiRender();

	myDevice->Present(flags);
}

graphics::Device* GraphicsEngine::GetDevice() const noexcept
{
	return myDevice.Get();
}

graphics::ImmediateDeviceContext* GraphicsEngine::GetDeviceContext() const noexcept
{
	return myDeviceContext.Get();
}

graphics::Window* GraphicsEngine::GetWindow() const
{
	return myDevice->GetWindow();
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

#undef FILE_CATEGORY 
#undef CLASS_CATEGORY