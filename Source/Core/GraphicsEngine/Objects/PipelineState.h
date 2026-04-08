#pragma once
#include <Core/RenderHardwareInterface/GraphicsDefs.h>
#include <Core/RenderHardwareInterface/PImpl.hpp>

#include <Core/RenderHardwareInterface/Objects/RasterizerState.h>
#include <Core/RenderHardwareInterface/Objects/DepthStencilState.h>
#include <Core/RenderHardwareInterface/Objects/BlendState.h>
#include <Core/RenderHardwareInterface/Objects/InputLayout.h>

#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>

#include <Core/RenderHardwareInterface/Objects/Shaders/VertexShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/HullShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/DomainShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/GeometryShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/PixelShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/ComputeShader.h>

#include <Core/RenderHardwareInterface/Objects/RenderTarget.h>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>
#include <Core/RenderHardwareInterface/Objects/SamplerState.h>
#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>

typedef unsigned int PIPELINE_STATE_WHEN;
enum
{
	PIPELINE_STATE_NEVER = 0,
	PIPELINE_STATE_BEFORE = 1,
	PIPELINE_STATE_AFTER = 2,
};

namespace graphics
{
	class Device;
}

struct PipelineStateShaderResource
{
	graphics::PImpl<graphics::ShaderResource> ShaderResource;
	unsigned int Slot;
	graphics::GRAPHICS_SHADER_STAGE_FLAGS Stages;
};

struct PipelineStateSamplerState
{
	graphics::PImpl<graphics::SamplerState> SamplerState;
	unsigned int Slot;
	graphics::GRAPHICS_SHADER_STAGE_FLAGS Stages;
};

struct PipelineStateDepthStencil
{
	graphics::PImpl<graphics::DepthStencil> DepthStencil;
	PIPELINE_STATE_WHEN ClearWhen;
	float ClearDepth;
	unsigned char ClearStencil;
};

struct PipelineStateRenderTarget
{
	graphics::PImpl<graphics::RenderTarget> RenderTarget;
	PIPELINE_STATE_WHEN ClearWhen;
	float ClearColor[4];
};

struct PipelineStateConstantBuffer
{
	graphics::PImpl<graphics::ConstantBuffer> ConstantBuffer;
	unsigned int Slot;
	graphics::GRAPHICS_SHADER_STAGE_FLAGS Stages;
};

struct PipelineStateDesc
{
public:
	//PipelineStateDesc();
	//~PipelineStateDesc();

	std::string Name;

	graphics::GRAPHICS_TOPOLOGY Topology;
	graphics::PImpl<graphics::RasterizerState> RasterizerState;
	graphics::PImpl<graphics::DepthStencilState> DepthStencilState;
	graphics::PImpl<graphics::BlendState> BlendState;
	graphics::PImpl<graphics::InputLayout> InputLayout;

	std::vector<PipelineStateConstantBuffer> ConstantBuffers;

	graphics::PImpl<graphics::VertexShader> VertexShader;
	graphics::PImpl<graphics::HullShader> HullShader;
	graphics::PImpl<graphics::DomainShader> DomainShader;
	graphics::PImpl<graphics::GeometryShader> GeometryShader;
	graphics::PImpl<graphics::PixelShader> PixelShader;
	graphics::PImpl<graphics::ComputeShader> ComputeShader;

	std::vector<PipelineStateShaderResource> ShaderResources;
	std::vector<PipelineStateSamplerState> SamplerStates;
	std::vector<PipelineStateRenderTarget> RenderTargets;
	std::vector<graphics::Viewport> Viewports;
	PipelineStateDepthStencil DepthStencil;

	//static PipelineStateDesc Load(graphics::Device* aDevice, std::istream& aStream);
};

class PipelineState
{
public:
	PipelineState();
	PipelineState(const PipelineStateDesc& aDesc);
	~PipelineState();

	const PipelineStateDesc& GetDesc() const;

	void Bind(graphics::Device* aDevice);

	void Unbind(graphics::Device* aDevice);

	void SetTopology(graphics::GRAPHICS_TOPOLOGY aTopology);

	void SetInputLayout(graphics::InputLayout* aInputLayout);

	void SetShader(graphics::VertexShader* aVertexShader);
	void SetShader(graphics::HullShader* aHullShader);
	void SetShader(graphics::DomainShader* aDomainShader);
	void SetShader(graphics::GeometryShader* aGeometryShader);
	void SetShader(graphics::PixelShader* aPixelShader);
	void SetShader(graphics::ComputeShader* aComputeShader);

	//void SetDepthStencil(graphics::DepthStencil* aDepthStencil);

	//template<typename _iteratorTy>
	//void SetRenderTargets(_iteratorTy aBegin, _iteratorTy aEnd);

	template<typename _iteratorTy>
	void SetShaderResources(_iteratorTy aBegin, _iteratorTy aEnd);

private:
	PipelineStateDesc myDesc;
	bool myPushedEvent;
};

//template<typename _iteratorTy>
//inline void PipelineState::SetRenderTargets(_iteratorTy aBegin, _iteratorTy aEnd)
//{
//	myDesc.RenderTargets.clear();
//	auto it = aBegin;
//	while (it != aEnd)
//	{
//		myDesc.RenderTargets.push_back(*it);
//		++it;
//	}
//}

template<typename _iteratorTy>
inline void PipelineState::SetShaderResources(_iteratorTy aBegin, _iteratorTy aEnd)
{
	myDesc.ShaderResources.clear();
	auto it = aBegin;
	while (it != aEnd)
	{
		myDesc.ShaderResources.push_back(*it);
		++it;
	}
}
