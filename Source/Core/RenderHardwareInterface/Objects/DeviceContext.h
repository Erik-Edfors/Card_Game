#pragma once
#include "GraphicsObject.h"
#include "../GraphicsDefs.h"
#include "../Result.h"

namespace graphics
{
	class ConstantBuffer;
	class VertexBuffer;
	class IndexBuffer;

	class VertexShader;
	class HullShader;
	class DomainShader;
	class GeometryShader;
	class PixelShader;
	class ComputeShader;

	class InputLayout;

	class Texture2D;
	class ShaderResource;
	class RenderTarget;
	class DepthStencil;

	class SamplerState;
	class RasterizerState;
	class DepthStencilState;
	class BlendState;

	class DeviceContext : public GraphicsObject
	{
	public:
		virtual ~DeviceContext();

		virtual result::Code ResetState(
		) = 0;

		//########
		// Buffers
		//########

		virtual result::Code UpdateConstantBuffer(
			ConstantBuffer* aConstantBuffer,
			const void* aData,
			size_t aDataSize
		) = 0;

		virtual result::Code UpdateVertexBuffer(
			VertexBuffer* aVertexBuffer,
			const void* aData,
			size_t aDataSize
		) = 0;

		virtual result::Code UpdateIndexBuffer(
			IndexBuffer* aIndexBuffer,
			const void* aData,
			size_t aDataSize
		) = 0;

		//########
		// Bind Resources
		//########

		virtual result::Code BindVertexShader(
			VertexShader* aShader
		) = 0;
		virtual result::Code BindHullShader(
			HullShader* aShader
		) = 0;
		virtual result::Code BindDomainShader(
			DomainShader* aShader
		) = 0;
		virtual result::Code BindGeometryShader(
			GeometryShader* aShader
		) = 0;
		virtual result::Code BindPixelShader(
			PixelShader* aShader
		) = 0;
		virtual result::Code BindComputeShader(
			ComputeShader* aShader
		) = 0;

		virtual result::Code BindConstantBuffers(
			ConstantBuffer* const* aConstantBuffers,
			size_t aNumBuffers,
			unsigned int aStartSlot,
			GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags
		) = 0;

		virtual result::Code BindVertexBuffers(
			VertexBuffer* const* aVertexBuffers,
			size_t aNumBuffers,
			unsigned int aStartSlot,
			const size_t* aStrides, 
			const size_t* aOffsets
		) = 0;

		virtual result::Code BindIndexBuffer(
			IndexBuffer* aIndexBuffer
		) = 0;

		virtual result::Code BindShaderResources(
			ShaderResource* const* aShaderResources,
			size_t aNumShaderResources,
			unsigned int aStartSlot,
			GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags
		) = 0;

		virtual result::Code BindTargets(
			RenderTarget* const* aRenderTargets,
			size_t aNumRenderTargets,
			DepthStencil* aDepthStencil
		) = 0;

		virtual result::Code BindSamplerStates(
			SamplerState* const* aSamplerStates,
			size_t aNumSamplerStates,
			unsigned int aStartSlot,
			GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags
		) = 0;

		//########
		// Resource Management
		//########

		virtual result::Code CopyTexture(
			Texture2D* aSource,
			Texture2D* aDestination
		) = 0;

		//virtual result::Code ResizeTexture(
		//	Texture2D* aTexture,
		//	unsigned int aWidth,
		//	unsigned int aHeight
		//) = 0;

		//########
		// Input/Output
		//########

		virtual result::Code ClearState() = 0;

		//Retreieves the backbuffer texture from the associated window with this device. WARNING: Make sure to remove all references to the backbuffer before calling UpdateBackbufferSize!
		//virtual result::Code GetBuffer(
		//	Texture2D** outBuffer
		//) = 0;

		virtual result::Code SetTopology(
			GRAPHICS_TOPOLOGY aTopology
		) = 0;

		virtual result::Code SetRasterizerState(
			RasterizerState* aRasterizerState
		) = 0;

		virtual result::Code SetDepthStencilState(
			DepthStencilState* aDepthStencilState,
			unsigned int aStencilReference
		) = 0;

		virtual result::Code SetInputLayout(
			InputLayout* aInputLayout
		) = 0;

		virtual result::Code SetBlendState(
			BlendState* aBlendState,
			float aBlendFactors[4] = nullptr,
			unsigned int aSampleMask = 0xffffffff
		) = 0;

		virtual result::Code Draw(
			unsigned int aVertexCount,
			unsigned int aWhere
		) = 0;
		virtual result::Code DrawIndexed(
			unsigned int aCount,
			unsigned int aWhere,
			unsigned int aVertexWhere
		) = 0;

		//virtual result::Code Present(
		//	GRAPHICS_PRESENT_FLAGS aFlags
		//) = 0;

		virtual result::Code ClearRenderTarget(
			RenderTarget* aRenderTarget,
			const float aColor[4]
		) = 0;

		virtual result::Code ClearDepthStencil(
			DepthStencil* aDepthStencil,
			float aDepth = 1.f,
			unsigned char aStencil = 0u
		) = 0;

		virtual result::Code SetViewports(
			const Viewport* aViewports,
			size_t aNumViewports
		) = 0;

		//virtual result::Code UpdateBackbufferSize(
		//	unsigned int aWidth,
		//	unsigned int aHeight
		//) = 0;

		//########
		// User Anotations
		//########

		virtual result::Code PushEvent(
			const char* aName,
			int* outNumEvents = nullptr
		) = 0;

		virtual result::Code PopEvent(
			int* outNumEvents = nullptr
		) = 0;

	protected:
		DeviceContext(GraphicsID aID);

	private:
	};
}