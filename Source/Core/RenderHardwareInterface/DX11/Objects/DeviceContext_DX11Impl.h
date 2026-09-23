#pragma once
#include "../../Objects/DeviceContext.h"
#include <wrl.h>

struct ID3D11DeviceContext;
struct ID3DUserDefinedAnnotation;

namespace graphics
{
	class DeviceContext_DX11Impl : public virtual DeviceContext
	{
	public:
		DeviceContext_DX11Impl(GraphicsID aID, ID3D11DeviceContext* aDeviceContext);
		~DeviceContext_DX11Impl();

		ID3D11DeviceContext* GetDeviceContext() const noexcept;
		ID3DUserDefinedAnnotation* GetUserDefinedAnnotation() const noexcept;

		void SetName(const char* aName) override;

		result::Code ResetState(
		) override;

		//########
		// Buffers
		//########

		result::Code UpdateConstantBuffer(
			ConstantBuffer* aConstantBuffer,
			const void* aData,
			size_t aDataSize
		) override;

		result::Code UpdateVertexBuffer(
			VertexBuffer* aVertexBuffer,
			const void* aData,
			size_t aDataSize
		) override;

		result::Code UpdateIndexBuffer(
			IndexBuffer* aIndexBuffer,
			const void* aData,
			size_t aDataSize
		) override;

		//########
		// Bind Resources
		//########

		result::Code BindVertexShader(
			VertexShader* aShader
		) override;
		result::Code BindHullShader(
			HullShader* aShader
		) override;
		result::Code BindDomainShader(
			DomainShader* aShader
		) override;
		result::Code BindGeometryShader(
			GeometryShader* aShader
		) override;
		result::Code BindPixelShader(
			PixelShader* aShader
		) override;
		result::Code BindComputeShader(
			ComputeShader* aShader
		) override;

		result::Code BindConstantBuffers(
			ConstantBuffer* const* aConstantBuffers,
			size_t aNumBuffers,
			unsigned int aStartSlot,
			GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags
		) override;

		result::Code BindVertexBuffers(
			VertexBuffer* const* aVertexBuffers,
			size_t aNumBuffers,
			unsigned int aStartSlot,
			const size_t* aStrides,
			const size_t* aOffsets
		) override;

		result::Code BindIndexBuffer(
			IndexBuffer* aIndexBuffer
		) override;

		result::Code BindShaderResources(
			ShaderResource* const* aShaderResources,
			size_t aNumShaderResources,
			unsigned int aStartSlot,
			GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags
		) override;

		result::Code BindTargets(
			RenderTarget* const* aRenderTargets,
			size_t aNumRenderTargets,
			DepthStencil* aDepthStencil
		) override;

		result::Code BindSamplerStates(
			SamplerState* const* aSamplerStates,
			size_t aNumSamplerStates,
			unsigned int aStartSlot,
			GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags
		) override;

		//########
		// Resource Management
		//########

		result::Code CopyTexture(
			Texture2D* aSource,
			Texture2D* aDestination
		) override;

		//result::Code ResizeTexture(
		//	Texture2D* aTexture,
		//	unsigned int aWidth,
		//	unsigned int aHeight
		//) override;

		//########
		// Input/Output
		//########

		result::Code ClearState() override;

		//Retreieves the backbuffer texture from the associated window with this device. WARNING: Make sure to remove all references to the backbuffer before calling UpdateBackbufferSize!
		//result::Code GetBuffer(
		//	Texture2D** outBuffer
		//) override;

		result::Code SetTopology(
			GRAPHICS_TOPOLOGY aTopology
		) override;

		result::Code SetRasterizerState(
			RasterizerState* aRasterizerState
		) override;

		result::Code SetDepthStencilState(
			DepthStencilState* aDepthStencilState,
			unsigned int aStencilReference
		) override;

		result::Code SetInputLayout(
			InputLayout* aInputLayout
		) override;

		result::Code SetBlendState(
			BlendState* aBlendState,
			float aBlendFactors[4] = nullptr,
			unsigned int aSampleMask = 0xffffffff
		) override;

		result::Code Draw(
			unsigned int aVertexCount,
			unsigned int aWhere
		) override;
		result::Code DrawIndexed(
			unsigned int aCount,
			unsigned int aWhere,
			unsigned int aVertexWhere
		) override;

		//result::Code Present(
		//	GRAPHICS_PRESENT_FLAGS aFlags
		//) override;

		result::Code ClearRenderTarget(
			RenderTarget* aRenderTarget,
			const float aColor[4]
		) override;

		result::Code ClearDepthStencil(
			DepthStencil* aDepthStencil,
			float aDepth = 1.f,
			unsigned char aStencil = 0u
		) override;

		result::Code SetViewports(
			const Viewport* aViewports,
			size_t aNumViewports
		) override;

		//result::Code UpdateBackbufferSize(
		//	unsigned int aWidth,
		//	unsigned int aHeight
		//) override;

		//########
		// User Anotations
		//########

		result::Code PushEvent(
			const char* aName,
			int* outNumEvents = nullptr
		) override;

		result::Code PopEvent(
			int* outNumEvents = nullptr
		) override;

	private:
		bool IsDeviceObject(const GraphicsObject* aGraphicsObject) const noexcept;

		result::Code _updateBuffer(
			ID3D11Buffer* aBuffer,
			const void* aData,
			size_t aDataSize
		);
		void _setName(
			const char* aName,
			GraphicsObject* aGraphicsObject
		);

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> myDeviceContext;
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> myUserDefinedAnnotation;
	};
}