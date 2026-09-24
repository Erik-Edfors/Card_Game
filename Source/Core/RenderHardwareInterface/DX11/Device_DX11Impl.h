#pragma once
#include "../Device.h"
#include "Window_DX11Impl.h"
#include <wrl.h>
#include "../PImpl.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3DUserDefinedAnnotation;

struct ID3D11Buffer;

namespace graphics
{
	class Texture2D;

	class Device_DX11Impl : public Device
	{
	public:
		Device_DX11Impl(Window_DX11Impl* aWindow, ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, IDXGISwapChain* aSwapChain, UINT aSwapChainFlags);
		~Device_DX11Impl();

		Window_DX11Impl* GetViewport();
		const Window_DX11Impl* GetViewport() const;

		ID3D11Device* GetDevice();
		const ID3D11Device* GetDevice() const;

		ID3D11DeviceContext* GetDeviceContext();
		const ID3D11DeviceContext* GetDeviceContext() const;

		IDXGISwapChain* GetSwapChain();
		const IDXGISwapChain* GetSwapChain() const;

		Window* GetWindow() const override;

		//########
		// Objects
		//########

		result::Code CreateData(
			Data** outData,
			const DataDesc* aDesc
		) override;

		result::Code CreateImmediateDeviceContext(
			ImmediateDeviceContext** outDeviceContext
		) override;

		result::Code CreateDeferredDeviceContext(
			DeferredDeviceContext** outDeviceContext
		) override;

		result::Code CreateTexture2D(
			Texture2D** outTexture2D,
			const Texture2DDesc* aDesc
		) override;

		result::Code CreateShaderResource(
			ShaderResource** outShaderResource,
			const ShaderResourceDesc* aDesc
		) override;

		result::Code CreateRenderTarget(
			RenderTarget** outRenderTarget,
			const RenderTargetDesc* aDesc
		) override;

		result::Code CreateDepthStencil(
			DepthStencil** outDepthStencil,
			const DepthStencilDesc* aDesc
		) override;

		//result::Code CreateConstantBuffer(
		//	ConstantBuffer** outConstantBuffer,
		//	const char* aName,
		//	const void* aData,
		//	size_t aSize,
		//	GRAPHICS_USAGE aUsage,
		//	GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags
		//) override;

		result::Code CreateConstantBuffer(
			ConstantBuffer** outConstantBuffer,
			const ConstantBufferDesc* aDesc
		) override;

		result::Code CreateVertexBuffer(
			VertexBuffer** outVertexBuffer,
			const VertexBufferDesc* aDesc
		) override;

		result::Code CreateIndexBuffer(
			IndexBuffer** outIndexBuffer,
			const char* aName,
			const void* aData,
			size_t aSize,
			GRAPHICS_USAGE aUsage,
			GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags
		) override;

		result::Code CreateShader(
			VertexShader** outShader,
			const char* aName,
			void* aShaderByteCode,
			size_t aNumBytes
		) override;
		result::Code CreateShader(
			HullShader** outShader,
			const char* aName,
			void* aShaderByteCode,
			size_t aNumBytes
		) override;
		result::Code CreateShader(
			DomainShader** outShader,
			const char* aName,
			void* aShaderByteCode,
			size_t aNumBytes
		) override;
		result::Code CreateShader(
			GeometryShader** outShader,
			const char* aName,
			void* aShaderByteCode,
			size_t aNumBytes
		) override;
		result::Code CreateShader(
			PixelShader** outShader,
			const char* aName,
			void* aShaderByteCode,
			size_t aDataSize
		) override;
		result::Code CreateShader(
			ComputeShader** outShader,
			const char* aName,
			void* aShaderByteCode,
			size_t aNumBytes
		) override;

		//result::Code CreateInputLayout(
		//	InputLayout** outInputLayout,
		//	const char* aName,
		//	const LayoutDefinition& aLayoutDefinition,
		//	void* aShaderByteCode,
		//	size_t aNumBytes
		//) override;

		result::Code CreateInputLayout(
			InputLayout** outInputLayout,
			const InputLayoutDesc* aDesc
		) override;

		result::Code CreateSamplerState(
			SamplerState** outSamplerState,
			const SamplerStateDesc* aDesc
		) override;

		//result::Code CreateBlendState(
		//	BlendState** outBlendState,
		//	const char* aName,
		//	bool aEnableAlphaToCoverage,
		//	bool aEnableIndependentBlend,
		//	const RenderTargetBlendDesc aRenderTargetBlendDescriptions[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS]
		//) override;

		result::Code CreateBlendState(
			BlendState** outBlendState,
			const BlendStateDesc* aDesc
		) override;

		result::Code CreateRasterizerState(
			RasterizerState** outRasterizerState,
			const RasterizerStateDesc* aDesc
		) override;

		result::Code CreateDepthStencilState(
			DepthStencilState** outDepthStencilState,
			const DepthStencilStateDesc* aDesc
		) override;

		//########
		// Buffers
		//########

		result::Code UpdateConstantBuffer(
			ConstantBuffer* aConstantBuffer,
			void* aData,
			size_t aDataSize
		) override;

		result::Code UpdateVertexBuffer(
			VertexBuffer* aVertexBuffer,
			void* aData,
			size_t aDataSize
		) override;

		result::Code UpdateIndexBuffer(
			IndexBuffer* aIndexBuffer,
			void* aData,
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

		result::Code Map(
			Texture2D* aTexture,
			unsigned int aSubResource,
			GRAPHICS_MAP aMapType,
			GRAPHICS_MAP_FLAGS aMapFlags,
			void** outData,
			unsigned int* outRowPitch,
			unsigned int* outDepthPitch
		) override;

		result::Code Unmap(
			Texture2D* aTexture,
			unsigned int aSubResource
		) override;

		result::Code ResizeTexture(
			Texture2D* aTexture,
			unsigned int aWidth,
			unsigned int aHeight
		) override;

		//########
		// Input/Output
		//########

		result::Code ClearState() override;

		result::Code GetBufferTexture(
			Texture2D** outTexture
		) override;

		result::Code GetBufferRenderTarget(
			RenderTarget** outRenderTarget
		) override;

		result::Code GetBufferDepthStencil(
			DepthStencil** outDepthStencil
		) override;

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
		);

		result::Code SetBlendState(
			BlendState* aBlendState,
			float aBlendFactors[4],
			unsigned int aSampleMask
		);

		result::Code Draw(
			unsigned int aVertexCount,
			unsigned int aWhere
		) override;

		result::Code DrawIndexed(
			unsigned int aCount,
			unsigned int aWhere,
			unsigned int aVertexWhere
		) override;

		result::Code Present(
			GRAPHICS_PRESENT_FLAGS aFlags
		) override;

		result::Code ClearRenderTarget(
			RenderTarget* aRenderTarget,
			const float aColor[4]
		) override;

		result::Code ClearDepthStencil(
			DepthStencil* aDepthStencil,
			float aDepth,
			unsigned char aStencil
		) override;

		result::Code SetViewports(
			Viewport* aViewports,
			size_t aNumViewports
		) override;

		result::Code UpdateBackbufferSize(
			unsigned int aWidth,
			unsigned int aHeight
		) override;

		//########
		// User
		//########

		result::Code PushEvent(
			const char* aName,
			int* outNumEvents
		) override;

		result::Code PopEvent(
			int* outNumEvents
		) override;

	private:
		result::Code _createBackbufferTexture(
			Texture2D** outTexture
		);

		result::Code _createBackbufferDepthTexture(
			Texture2D** outTexture
		);

		result::Code _createBackbufferRenderTarget(
			RenderTarget** outRenderTarget
		);

		result::Code _createBackbufferDepthStencil(
			DepthStencil** outDepthStencil
		);

		result::Code _createBuffer(
			ID3D11Buffer** outBuffer,
			UINT aBindFlags,
			const void* aData,
			size_t aSize,
			GRAPHICS_USAGE aUsage,
			GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags
		);
		result::Code _updateBuffer(
			ID3D11Buffer* aBuffer,
			const void* aData,
			size_t aDataSize
		);
		void _setName(
			const char* aName,
			GraphicsObject* aGraphicsObject
		);

		PImpl<Window_DX11Impl> myWindow;

		PImpl<Texture2D> myBackbufferTexture;
		PImpl<Texture2D> myBackbufferDepthTexture;
		PImpl<RenderTarget> myBackbufferRenderTarget;
		PImpl<DepthStencil> myBackbufferDepthStencil;

		Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> myDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapChain;
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> myUserDefinedAnnotation;

		UINT mySwapChainFlags;
	};
}