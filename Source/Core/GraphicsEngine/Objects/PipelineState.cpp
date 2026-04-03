#include "GraphicsEngine.pch.h"
#include "PipelineState.h"
#include "../Common.h"
#include "../GraphicsEnumsFromString.h"

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/InputLayout.h>
#include <Core/RenderHardwareInterface/Objects/RasterizerState.h>
#include <Core/RenderHardwareInterface/Objects/BlendState.h>

#undef FILE_CATEGORY
#define FILE_CATEGORY "PipelineState.cpp"

PipelineStateDesc::PipelineStateDesc() :
	Topology(graphics::GRAPHICS_TOPOLOGY_UNDEFINED)
{
}
PipelineStateDesc::~PipelineStateDesc() = default;

#undef CLASS_CATEGORY
#define CLASS_CATEGORY FILE_CATEGORY"->PipelineState"

PipelineState::PipelineState() :
	myDesc({}),
	myPushedEvent(false)
{

}

PipelineState::PipelineState(const PipelineStateDesc& aDesc) :
	myDesc(aDesc),
	myPushedEvent(false)
{

}

//PipelineStateDesc PipelineStateDesc::Load(graphics::Device* aDevice, std::istream& aStream)
//{
//	PipelineStateDesc desc = {};
//
//	nlohmann::json json;
//	aStream >> json;
//
//	desc.Topology = GraphicsTopologyFromString(json.value("Topology", "GRAPHICS_TOPOLOGY_UNDEFINED").c_str());
//
//#define _PipelineState_TryLoadShader(shaderType) \
//{ \
//	void* shaderByteCode = nullptr; \
//	size_t shaderByteCodeSize = 0ull; \
//	std::filesystem::path shaderPath = json.value(#shaderType, ""); \
//	if (shaderPath.empty() == false) \
//	{ \
//		if (LoadFileIntoMemory(&shaderByteCode, &shaderByteCodeSize, shaderPath.string().c_str()) == true) \
//		{ \
//			aDevice->CreateShader(&desc.shaderType, shaderPath.filename().string().c_str(), shaderByteCode, shaderByteCodeSize); \
//			delete shaderByteCode; \
//			shaderByteCode = nullptr; \
//		} \
//	} \
//}
//
//	{
//		void* shaderByteCode = nullptr;
//		size_t shaderByteCodeSize = 0ull;
//		std::filesystem::path shaderPath = json.value("VertexShader", "");
//		if (shaderPath.empty() == false)
//		{
//			LoadFileIntoMemory(&shaderByteCode, &shaderByteCodeSize, shaderPath.string().c_str());
//			aDevice->CreateShader(&desc.VertexShader, shaderPath.filename().string().c_str(), shaderByteCode, shaderByteCodeSize);
//
//			std::filesystem::path inputLayoutPath = json.value("InputLayout", "");
//			if (inputLayoutPath.empty() == false)
//			{
//				std::filesystem::path layoutName = inputLayoutPath.filename();
//				layoutName.replace_extension("");
//
//				std::ifstream inputLayoutFile(inputLayoutPath);
//
//				graphics::LayoutDefinition definition = {};
//				//TODO: Add error handling.
//				bool result = LoadGraphicsLayoutDefinitionFromStream(&definition, inputLayoutFile);
//				if (result)
//				{
//					aDevice->CreateInputLayout(
//						&desc.InputLayout,
//						layoutName.string().c_str(),
//						definition,
//						shaderByteCode,
//						shaderByteCodeSize
//					);
//				}
//			}
//
//			delete shaderByteCode;
//			shaderByteCode = nullptr;
//		};
//	}
//	_PipelineState_TryLoadShader(HullShader);
//	_PipelineState_TryLoadShader(DomainShader);
//	_PipelineState_TryLoadShader(GeometryShader);
//	_PipelineState_TryLoadShader(PixelShader);
//	_PipelineState_TryLoadShader(ComputeShader);
//
//
//	return desc;
//#undef _PipelineState_TryLoadShader
//}

PipelineState::~PipelineState() = default;

const PipelineStateDesc& PipelineState::GetDesc() const
{
	return myDesc;
}

void PipelineState::Bind(graphics::Device* aDevice)
{
#undef LOG_CATEGORY
#define LOG_CATEGORY CLASS_CATEGORY"::Bind"

	if (not myDesc.Name.empty())
	{
		aDevice->PushEvent(myDesc.Name.c_str(), nullptr);
		myPushedEvent = true;
	}

	aDevice->SetRasterizerState(myDesc.RasterizerState.Get());
	aDevice->SetDepthStencilState(myDesc.DepthStencilState.Get(), 0);	//TODO: Figure out where to put aStencilReference.
	aDevice->SetTopology(myDesc.Topology);
	aDevice->SetBlendState(myDesc.BlendState.Get(), NULL);

	for (const PipelineStateConstantBuffer& cb : myDesc.ConstantBuffers)
	{
		aDevice->BindConstantBuffers(&cb.ConstantBuffer, 1u, cb.Slot, cb.Stages);
	}

	aDevice->BindVertexShader(myDesc.VertexShader.Get());
	if (myDesc.InputLayout != nullptr)
	{
		aDevice->SetInputLayout(myDesc.InputLayout.Get());
	}

	aDevice->BindHullShader(myDesc.HullShader.Get());
	aDevice->BindDomainShader(myDesc.DomainShader.Get());
	aDevice->BindGeometryShader(myDesc.GeometryShader.Get());
	aDevice->BindPixelShader(myDesc.PixelShader.Get());
	aDevice->BindComputeShader(myDesc.ComputeShader.Get());

	aDevice->SetViewports(myDesc.Viewports.data(), myDesc.Viewports.size());

	graphics::RenderTarget* renderTargets[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS] = {};
	size_t numRenderTargets = myDesc.RenderTargets.size();
	for (size_t i = 0; i < numRenderTargets; i++)
	{
		renderTargets[i] = myDesc.RenderTargets[i].RenderTarget.Get();
		if (myDesc.RenderTargets[i].RenderTarget != nullptr && (myDesc.RenderTargets[i].ClearWhen & PIPELINE_STATE_BEFORE) != 0)
		{
			aDevice->ClearRenderTarget(myDesc.RenderTargets[i].RenderTarget.Get(), myDesc.RenderTargets[i].ClearColor);
		}
	}
	if (myDesc.DepthStencil.DepthStencil != nullptr && (myDesc.DepthStencil.ClearWhen & PIPELINE_STATE_BEFORE) != 0)
	{
		aDevice->ClearDepthStencil(myDesc.DepthStencil.DepthStencil.Get(), myDesc.DepthStencil.ClearDepth, myDesc.DepthStencil.ClearStencil);
	}
	aDevice->BindTargets(renderTargets, numRenderTargets, myDesc.DepthStencil.DepthStencil.Get());

	for (const auto& sr : myDesc.ShaderResources)
	{
		aDevice->BindShaderResources(&sr.ShaderResource, 1, sr.Slot, sr.Stages);
	}
	for (const auto& ss : myDesc.SamplerStates)
	{
		aDevice->BindSamplerStates(&ss.SamplerState, 1, ss.Slot, ss.Stages);
	}
}

void PipelineState::Unbind(graphics::Device* aDevice)
{
	//aDevice->ClearState();
	aDevice->SetRasterizerState(nullptr);
	aDevice->SetDepthStencilState(nullptr, 0);	//TODO: Figure out where to put aStencilReference.
	aDevice->SetTopology(graphics::GRAPHICS_TOPOLOGY_UNDEFINED);
	aDevice->SetBlendState(nullptr, NULL);

	for (const PipelineStateConstantBuffer& cb : myDesc.ConstantBuffers)
	{
		aDevice->BindConstantBuffers(nullptr, 1u, cb.Slot, cb.Stages);
	}

	aDevice->BindVertexShader(nullptr);
	aDevice->BindHullShader(nullptr);
	aDevice->BindDomainShader(nullptr);
	aDevice->BindGeometryShader(nullptr);
	aDevice->BindPixelShader(nullptr);
	aDevice->BindComputeShader(nullptr);

	aDevice->SetViewports(nullptr, 0);
	aDevice->BindTargets(nullptr, 0, nullptr);
	if (myDesc.DepthStencil.DepthStencil != nullptr && (myDesc.DepthStencil.ClearWhen & PIPELINE_STATE_AFTER) != 0)
	{
		aDevice->ClearDepthStencil(myDesc.DepthStencil.DepthStencil.Get(), myDesc.DepthStencil.ClearDepth, myDesc.DepthStencil.ClearStencil);
	}
	for (size_t i = 0; i < myDesc.RenderTargets.size(); i++)
	{
		if (myDesc.RenderTargets[i].RenderTarget != nullptr && (myDesc.RenderTargets[i].ClearWhen & PIPELINE_STATE_AFTER) != 0)
		{
			aDevice->ClearRenderTarget(myDesc.RenderTargets[i].RenderTarget.Get(), myDesc.RenderTargets[i].ClearColor);
		}
	}

	for (const auto& sr : myDesc.ShaderResources)
	{
		aDevice->BindShaderResources(nullptr, 1, sr.Slot, sr.Stages);
	}
	for (const auto& ss : myDesc.SamplerStates)
	{
		aDevice->BindSamplerStates(nullptr, 1, ss.Slot, ss.Stages);
	}

	if (myPushedEvent)
	{
		aDevice->PopEvent(nullptr);
		myPushedEvent = false;
	}
}

void PipelineState::SetTopology(graphics::GRAPHICS_TOPOLOGY aTopology)
{
	myDesc.Topology = aTopology;
}

void PipelineState::SetInputLayout(graphics::InputLayout* aInputLayout)
{
	myDesc.InputLayout = aInputLayout;
}

void PipelineState::SetShader(graphics::VertexShader* aVertexShader)
{
	myDesc.VertexShader = aVertexShader;
}

void PipelineState::SetShader(graphics::HullShader* aHullShader)
{
	myDesc.HullShader = aHullShader;
}

void PipelineState::SetShader(graphics::DomainShader* aDomainShader)
{
	myDesc.DomainShader = aDomainShader;
}

void PipelineState::SetShader(graphics::GeometryShader* aGeometryShader)
{
	myDesc.GeometryShader = aGeometryShader;
}

void PipelineState::SetShader(graphics::PixelShader* aPixelShader)
{
	myDesc.PixelShader = aPixelShader;
}

void PipelineState::SetShader(graphics::ComputeShader* aComputeShader)
{
	myDesc.ComputeShader = aComputeShader;
}

//void PipelineState::SetDepthStencil(graphics::DepthStencil* aDepthStencil)
//{
//	myDesc.DepthStencil = aDepthStencil;
//}

