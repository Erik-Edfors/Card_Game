#pragma once
#include "GraphicsDefs.h"
#include "Result.h"
#include "RefObject.hpp"

namespace graphics
{
	class GraphicsObject;

	class Window;

	class ImmediateDeviceContext;
	class DeferredDeviceContext;

	class Data;
	struct DataDesc;

	class Texture2D;
	struct Texture2DDesc;
	class Texture2DArray;
	class ShaderResource;
	struct ShaderResourceDesc;
	class RenderTarget;
	struct RenderTargetDesc;
	class DepthStencil;
	struct DepthStencilDesc;
	class ConstantBuffer;
	struct ConstantBufferDesc;

	class VertexBuffer;
	struct VertexBufferDesc;
	class IndexBuffer;
	struct IndexBufferDesc;

	class VertexShader;
	struct VertexShaderDesc;
	class HullShader;
	struct HullShaderDesc;
	class DomainShader;
	struct DomainShaderDesc;
	class GeometryShader;
	struct GeometryShaderDesc;
	class PixelShader;
	struct PixelShaderDesc;
	class ComputeShader;
	struct ComputeShaderDesc;

	class RasterizerState;
	struct RasterizerStateDesc;
	class BlendState;
	struct RenderTargetBlendDesc;
	struct BlendStateDesc;
	class DepthStencilState;
	struct DepthStencilStateDesc;
	class InputLayout;
	struct InputLayoutDesc;
	class SamplerState;
	struct SamplerStateDesc;

	//class PipelineState;

	struct LayoutDefinition;

	class Device : public RefObject
	{
	public:
		virtual ~Device() = default;

		DeviceID GetID() const noexcept;
		bool IsDeviceObject(GraphicsObject* aGraphicsObject) const noexcept;
		GraphicsID CreateGraphicsID() noexcept;

		virtual Window* GetWindow() const = 0;

		//########
		// Objects
		//########

		virtual result::Code CreateData(
			Data** outData,														// A pointer to a Data interface that will recieve the created data.
			const DataDesc* aDesc												// A descriptor struct for a Data.
		) = 0;

		virtual result::Code CreateImmediateDeviceContext(
			ImmediateDeviceContext** outDeviceContext							// A pointer to an ImmediateDeviceContext interface that will recieve the created context.
		) = 0;

		virtual result::Code CreateDeferredDeviceContext(
			DeferredDeviceContext** outDeviceContext							// A pointer to an DeferredDeviceContext interface that will recieve the created context.
		) = 0;

		virtual result::Code CreateTexture2D(
			Texture2D** outTexture2D,											//A pointer to a Texture2D interface that will recieve the created texture.
			const Texture2DDesc* aDesc											//A descriptor struct for a Texture2D.
		) = 0;

		//Creates a shader resource on the device.
		virtual result::Code CreateShaderResource(
			ShaderResource** outShaderResource,									//A pointer to a ShaderResource interface that will recieve the created shader resource.
			const ShaderResourceDesc* aDesc
		) = 0;

		//Creates a render target to the given texture.
		virtual result::Code CreateRenderTarget(
			RenderTarget** outRenderTarget,										//A pointer to a RenderTarget interface that will recieve the created render target.
			const RenderTargetDesc* aDesc
		) = 0;

		virtual result::Code CreateDepthStencil(
			DepthStencil** outDepthStencil,										//A pointer to a DepthStencil interface that will recieve the created depth stencil.
			const DepthStencilDesc* aDesc
		) = 0;

		////Creates a constant buffer on the device.
		//virtual result::Code CreateConstantBuffer(
		//	ConstantBuffer** outConstantBuffer,									//A pointer to a ConstantBuffer interface that will recieve the created constant buffer.
		//	const char* aName,													//A name to assign the object.
		//	const void* aData,													//A pointer to data to fill the constant buffer with. If nullptr, will only allocate space. Can only be nullptr if aUsage is not GRAPHICS_USAGE_IMMUTABLE.
		//	size_t aSize,														//The size of constant buffer in bytes.
		//	GRAPHICS_USAGE aUsage,												//The usage of the constant buffer.
		//	GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags							//CPU access flags. Non-immutable buffers should be writable by the cpu.
		//) = 0;

		//Creates a constant buffer on the device.
		virtual result::Code CreateConstantBuffer(
			ConstantBuffer** outConstantBuffer,									//A pointer to a ConstantBuffer interface that will recieve the created constant buffer.
			const ConstantBufferDesc* aDesc
		) = 0;

		//Creates a vertex buffer on the device.
		virtual result::Code CreateVertexBuffer(
			VertexBuffer** outVertexBuffer,										//A pointer to a VertexBuffer interface that will recieve the created vertex buffer.
			const VertexBufferDesc* aDesc
		) = 0;

		//Creates a index buffer on the device.
		virtual result::Code CreateIndexBuffer(
			IndexBuffer** outIndexBuffer,										//A pointer to a IndexBuffer interface that will recieve the created index buffer.
			const char* aName,													//A name to assign the object.
			const void* aData,													//A pointer to an array of unsigned int data to fill the index buffer with. If nullptr, will only allocate space. Can only be nullptr if aUsage is not GRAPHICS_USAGE_IMMUTABLE.
			size_t aSize,														//The size of the index buffer in bytes.
			GRAPHICS_USAGE aUsage,												//The usage of the index buffer.
			GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags							//CPU Access flags. Non-immutable buffers should be writable by the cpu.
		) = 0;

		//Creates a vertex shader from shader byte code.
		virtual result::Code CreateShader(
			VertexShader** outShader,
			const char* aName,													//A name to assign the object.
			void* aShaderByteCode,
			size_t aNumBytes
		) = 0;
		//Creates a hull shader from shader byte code.
		virtual result::Code CreateShader(
			HullShader** outShader,
			const char* aName,													//A name to assign the object.
			void* aShaderByteCode,
			size_t aNumBytes
		) = 0;
		//Creates a domain shader from shader byte code.
		virtual result::Code CreateShader(
			DomainShader** outShader,
			const char* aName,													//A name to assign the object.
			void* aShaderByteCode,
			size_t aNumBytes
		) = 0;
		//Creates a geometry shader from shader byte code.
		virtual result::Code CreateShader(
			GeometryShader** outShader,
			const char* aName,													//A name to assign the object.
			void* aShaderByteCode,
			size_t aNumBytes
		) = 0;
		//Creates a pixel shader from shader byte code.
		virtual result::Code CreateShader(
			PixelShader** outShader,
			const char* aName,													//A name to assign the object.
			void* aShaderByteCode,
			size_t aNumBytes
		) = 0;
		//Creates a compute shader from shader byte code.
		virtual result::Code CreateShader(
			ComputeShader** outShader,
			const char* aName,													//A name to assign the object.
			void* aShaderByteCode,
			size_t aNumBytes
		) = 0;

		//virtual result::Code CreateInputLayout(
		//	InputLayout** outInputLayout,
		//	const char* aName,													//A name to assign the object.
		//	const LayoutDefinition& aLayoutDefinition,
		//	void* aShaderByteCode,
		//	size_t aNumBytes
		//) = 0;

		virtual result::Code CreateInputLayout(
			InputLayout** outInputLayout,
			const InputLayoutDesc* aDesc
		) = 0;

		//virtual result::Code CreateSamplerState(
		//	SamplerState** outSamplerState,
		//	const char* aName,													//A name to assign the object.
		//	GRAPHICS_SAMPLER_FILTER aFilter,
		//	GRAPHICS_TEXTURE_ADDRESS_MODE aAddressU,
		//	GRAPHICS_TEXTURE_ADDRESS_MODE aAddressV,
		//	GRAPHICS_TEXTURE_ADDRESS_MODE aAddressW,
		//	float aMipLODBias,
		//	unsigned int aMaxAnistropy,
		//	GRAPHICS_COMPARISON aComparison,
		//	const float aBorderColor[4],
		//	float aMinLOD,
		//	float aMaxLOD
		//) = 0;

		virtual result::Code CreateSamplerState(
			SamplerState** outSamplerState,
			const SamplerStateDesc* aDesc
		) = 0;

		//virtual result::Code CreateBlendState(
		//	BlendState** outBlendState,
		//	const char* aName,													//A name to assign the object.
		//	bool aEnableAlphaToCoverage,
		//	bool aEnableIndependentBlend,
		//	const RenderTargetBlendDesc aRenderTargetBlendDescriptions[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS]
		//) = 0;

		virtual result::Code CreateBlendState(
			BlendState** outBlendState,
			const BlendStateDesc* aDesc
		) = 0;

		virtual result::Code CreateRasterizerState(
			RasterizerState** outRasterizerState,
			const RasterizerStateDesc* aDesc
		) = 0;

		virtual result::Code CreateDepthStencilState(
			DepthStencilState** outDepthStencilState,
			const DepthStencilStateDesc* aDesc
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

		virtual result::Code Map(
			Texture2D* aTexture,
			unsigned int aSubResource,
			GRAPHICS_MAP aMapType,
			GRAPHICS_MAP_FLAGS aMapFlags,
			void** outData,
			unsigned int* outRowPitch,
			unsigned int* outDepthPitch
		) = 0;

		virtual result::Code Unmap(
			Texture2D* aTexture,
			unsigned int aSubResource
		) = 0;

		virtual result::Code ResizeTexture(
			Texture2D* aTexture,
			unsigned int aWidth,
			unsigned int aHeight
		) = 0;

		//########
		// Input/Output
		//########

		virtual result::Code ClearState() = 0;

		//Retreieves the backbuffer texture from the associated window with this device. WARNING: Make sure to remove all references to the backbuffer before calling UpdateBackbufferSize!
		virtual result::Code GetBufferTexture(
			Texture2D** outTexture
		) = 0;

		virtual result::Code GetBufferRenderTarget(
			RenderTarget** outRenderTarget
		) = 0;

		virtual result::Code GetBufferDepthStencil(
			DepthStencil** outDepthStencil
		) = 0;

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

		virtual result::Code Present(
			GRAPHICS_PRESENT_FLAGS aFlags
		) = 0;

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
			Viewport* aViewports,
			size_t aNumViewports
		) = 0;

		virtual result::Code UpdateBackbufferSize(
			unsigned int aWidth,
			unsigned int aHeight
		) = 0;

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
		Device();

		static DeviceID _createDeviceID();
		ObjectID _createObjectID();
		GraphicsID _createGraphicsID();

	private:
		static DeviceID locDeviceIDCounter;
		ObjectID myObjectIDCounter;

		DeviceID myDeviceID;
	};
}