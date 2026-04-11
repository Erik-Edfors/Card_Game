#include "RenderHardwareInterface.pch.h"
#include "DeviceContext_DX11Impl.h"

#include "DX11/Common_DX11.h"
#include "DX11/GAssert_DX11.h"

#include "Buffers/ConstantBuffer_DX11Impl.h"
#include "Buffers/VertexBuffer_DX11Impl.h"
#include "Buffers/IndexBuffer_DX11Impl.h"

#include "Shaders/VertexShader_DX11Impl.h"
#include "Shaders/HullShader_DX11Impl.h"
#include "Shaders/DomainShader_DX11Impl.h"
#include "Shaders/GeometryShader_DX11Impl.h"
#include "Shaders/PixelShader_DX11Impl.h"
#include "Shaders/ComputeShader_DX11Impl.h"

#include "Texture2D_DX11Impl.h"
#include "ShaderResource_DX11Impl.h"
#include "RenderTarget_DX11Impl.h"
#include "DepthStencil_DX11Impl.h"

#include "SamplerState_DX11Impl.h"
#include "DepthStencilState_DX11Impl.h"
#include "BlendState_DX11Impl.h"
#include "RasterizerState_DX11Impl.h"

#include "InputLayout_DX11Impl.h"

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "DeviceContext_DX11Impl.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->DeviceContext_DX11Impl"

namespace graphics
{
	DeviceContext_DX11Impl::DeviceContext_DX11Impl(GraphicsID aID, ID3D11DeviceContext* aDeviceContext) : 
		myDeviceContext(aDeviceContext),
		DeviceContext(aID)
	{
		myDeviceContext.As(&myUserDefinedAnnotation);
	}
	DeviceContext_DX11Impl::~DeviceContext_DX11Impl()
	{
	}
	ID3D11DeviceContext* DeviceContext_DX11Impl::GetDeviceContext() const noexcept 
	{
		return myDeviceContext.Get();
	}
	ID3DUserDefinedAnnotation* DeviceContext_DX11Impl::GetUserDefinedAnnotation() const noexcept
	{
		return myUserDefinedAnnotation.Get();
	}
	void DeviceContext_DX11Impl::SetName(const char* aName)
	{
		SetLocalName(aName);
		myDeviceContext->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(aName)), aName);
	}
	result::Code DeviceContext_DX11Impl::ResetState()
	{
		myDeviceContext->ClearState();
		return result::OK;
	}
	result::Code DeviceContext_DX11Impl::UpdateConstantBuffer(ConstantBuffer* aConstantBuffer, const void* aData, size_t aDataSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateConstantBuffer"

		GRAPHICS_ASSERT(aConstantBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aConstantBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aConstantBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aConstantBuffer) " (%u) was not from the same device (%u)!", GetID(), aConstantBuffer->GetID().Device);
		GRAPHICS_ASSERT(aData != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aData) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDataSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDataSize) " must be greater than 0!");
		GRAPHICS_ASSERT(aDataSize <= aConstantBuffer->GetSize(), LOG_CATEGORY, result::OUT_OF_RANGE, STRINGIFY(aDataSize) " was larger than the size of " STRINGIFY(aConstantBuffer) "!");

		ConstantBuffer_DX11Impl* buffer = dynamic_cast<ConstantBuffer_DX11Impl*>(aConstantBuffer);
		GRAPHICS_ASSERT(buffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aConstantBuffer) " could not be cast to " STRINGIFY(ConstantBuffer_DX11Impl*) "!");

		result::Code result = _updateBuffer(buffer->GetBuffer(), aData, aDataSize);
		if (GRAPHICS_FAILED(result))
		{
			return result;
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::UpdateVertexBuffer(VertexBuffer* aVertexBuffer, const void* aData, size_t aDataSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateVertexBuffer"

		GRAPHICS_ASSERT(aVertexBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aVertexBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aVertexBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aVertexBuffer) " (%u) was not from the same device (%u)!", GetID(), aVertexBuffer->GetID().Device);
		GRAPHICS_ASSERT(aData != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aData) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDataSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDataSize) " must be greater than 0!");
		GRAPHICS_ASSERT(aDataSize <= aVertexBuffer->GetSize(), LOG_CATEGORY, result::OUT_OF_RANGE, STRINGIFY(aDataSize) " was larger than the size of " STRINGIFY(aVertexBuffer) "!");

		VertexBuffer_DX11Impl* buffer = dynamic_cast<VertexBuffer_DX11Impl*>(aVertexBuffer);
		GRAPHICS_ASSERT(buffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aVertexBuffer) " could not be cast to " STRINGIFY(VertexBuffer_DX11Impl*) "!");

		result::Code result = _updateBuffer(buffer->GetBuffer(), aData, aDataSize);
		if (GRAPHICS_FAILED(result))
		{
			return result;
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::UpdateIndexBuffer(IndexBuffer* aIndexBuffer, const void* aData, size_t aDataSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateIndexBuffer"

		GRAPHICS_ASSERT(aIndexBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aIndexBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aIndexBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aIndexBuffer) " (%u) was not from the same device (%u)!", GetID(), aIndexBuffer->GetID().Device);
		GRAPHICS_ASSERT(aData != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aData) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDataSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDataSize) " must be greater than 0!");
		GRAPHICS_ASSERT(aDataSize <= aIndexBuffer->GetSize(), LOG_CATEGORY, result::OUT_OF_RANGE, STRINGIFY(aDataSize) " was larger than the capacity of " STRINGIFY(aIndexBuffer) "!");

		IndexBuffer_DX11Impl* buffer = dynamic_cast<IndexBuffer_DX11Impl*>(aIndexBuffer);
		GRAPHICS_ASSERT(buffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aIndexBuffer) " could not be cast to " STRINGIFY(IndexBuffer_DX11Impl*) "!");

		result::Code result = _updateBuffer(buffer->GetBuffer(), aData, aDataSize);
		if (GRAPHICS_FAILED(result))
		{
			return result;
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindVertexShader(VertexShader* aShader)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindVertexShader"
		if (aShader == nullptr)
		{
			myDeviceContext->VSSetShader(NULL, NULL, 0);
			return result::OK;
		}

		GRAPHICS_ASSERT(IsDeviceObject(aShader), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShader) " (%u) was not from the same device (%u)!", GetID(), aShader->GetID().Device);

		VertexShader_DX11Impl* shader = dynamic_cast<VertexShader_DX11Impl*>(aShader);
		GRAPHICS_ASSERT(shader != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aShader) " could not be cast to " STRINGIFY(VertexShader_DX11Impl*) "!");

		myDeviceContext->VSSetShader(shader->GetVertexShader(), NULL, 0);
		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindHullShader(HullShader* aShader)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindHullShader"
		if (aShader == nullptr)
		{
			myDeviceContext->HSSetShader(NULL, NULL, 0);
			return result::OK;
		}

		GRAPHICS_ASSERT(IsDeviceObject(aShader), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShader) " (%u) was not from the same device (%u)!", GetID(), aShader->GetID().Device);

		HullShader_DX11Impl* shader = dynamic_cast<HullShader_DX11Impl*>(aShader);
		GRAPHICS_ASSERT(shader != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aShader) " could not be cast to " STRINGIFY(HullShader_DX11Impl*) "!");

		myDeviceContext->HSSetShader(shader->GetHullShader(), NULL, 0);
		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindDomainShader(DomainShader* aShader)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindDomainShader"
		if (aShader == nullptr)
		{
			myDeviceContext->DSSetShader(NULL, NULL, 0);
			return result::OK;
		}

		GRAPHICS_ASSERT(IsDeviceObject(aShader), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShader) " (%u) was not from the same device (%u)!", GetID(), aShader->GetID().Device);

		DomainShader_DX11Impl* shader = dynamic_cast<DomainShader_DX11Impl*>(aShader);
		GRAPHICS_ASSERT(shader != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aShader) " could not be cast to " STRINGIFY(DomainShader_DX11Impl*) "!");

		myDeviceContext->DSSetShader(shader->GetDomainShader(), NULL, 0);
		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindGeometryShader(GeometryShader* aShader)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindGeometryShader"
		if (aShader == nullptr)
		{
			myDeviceContext->GSSetShader(NULL, NULL, 0);
			return result::OK;
		}

		GRAPHICS_ASSERT(IsDeviceObject(aShader), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShader) " (%u) was not from the same device (%u)!", GetID(), aShader->GetID().Device);

		GeometryShader_DX11Impl* shader = dynamic_cast<GeometryShader_DX11Impl*>(aShader);
		GRAPHICS_ASSERT(shader != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aShader) " could not be cast to " STRINGIFY(GeometryShader_DX11Impl*) "!");

		myDeviceContext->GSSetShader(shader->GetGeometryShader(), NULL, 0);
		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindPixelShader(PixelShader* aShader)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindPixelShader"
		if (aShader == nullptr)
		{
			myDeviceContext->PSSetShader(NULL, NULL, 0);
			return result::OK;
		}

		GRAPHICS_ASSERT(IsDeviceObject(aShader), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShader) " (%u) was not from the same device (%u)!", GetID(), aShader->GetID().Device);

		PixelShader_DX11Impl* shader = dynamic_cast<PixelShader_DX11Impl*>(aShader);
		GRAPHICS_ASSERT(shader != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aShader) " could not be cast to " STRINGIFY(PixelShader_DX11Impl*) "!");

		myDeviceContext->PSSetShader(shader->GetPixelShader(), NULL, 0);
		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindComputeShader(ComputeShader* aShader)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindComputeShader"
		if (aShader == nullptr)
		{
			myDeviceContext->CSSetShader(NULL, NULL, 0);
			return result::OK;
		}

		GRAPHICS_ASSERT(IsDeviceObject(aShader), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShader) " (%u) was not from the same device (%u)!", GetID(), aShader->GetID().Device);

		ComputeShader_DX11Impl* shader = dynamic_cast<ComputeShader_DX11Impl*>(aShader);
		GRAPHICS_ASSERT(shader != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aShader) " could not be cast to " STRINGIFY(ComputeShader_DX11Impl*) "!");

		myDeviceContext->CSSetShader(shader->GetComputeShader(), NULL, 0);
		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindConstantBuffers(ConstantBuffer* const* aConstantBuffers, size_t aNumBuffers, unsigned int aStartSlot, GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindConstantBuffers"

		//GRAPHICS_ASSERT(aConstantBuffers != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aConstantBuffers) " cannot be nullptr!");
		GRAPHICS_ASSERT(aStartSlot + aNumBuffers <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aStartSlot) " + " STRINGIFY(aNumBuffers) " must be lower than '" STRINGIFY(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) "'!");
		GRAPHICS_ASSERT(aNumBuffers > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBuffers) " must be greater than 0!");

		ID3D11Buffer** buffers = new ID3D11Buffer * [aNumBuffers];
		for (size_t i = 0; i < aNumBuffers; i++)
		{
			if (aConstantBuffers != nullptr && aConstantBuffers[i] != nullptr)
			{
				ConstantBuffer_DX11Impl* constantBuffer = dynamic_cast<ConstantBuffer_DX11Impl*>(aConstantBuffers[i]);
				GRAPHICS_ASSERT(constantBuffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, "A " STRINGIFY(ConstantBuffer) " at " STRINGIFY(aConstantBuffers) "[%u] failed to cast to " STRINGIFY(ConstantBuffer_DX11Impl*) "!", i);
				GRAPHICS_ASSERT(IsDeviceObject(constantBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, "A " STRINGIFY(ConstantBuffer) " (%u) at " STRINGIFY(aConstantBuffers) "[%u] was not from the same device (%u)!", GetID(), i, constantBuffer->GetID().Device);
				buffers[i] = constantBuffer->GetBuffer();
			}
			else
			{
				buffers[i] = NULL;
			}
		}

		if (aShaderFlags & GRAPHICS_SHADER_STAGE_VERTEX)
		{
			myDeviceContext->VSSetConstantBuffers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumBuffers), buffers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_HULL)
		{
			myDeviceContext->HSSetConstantBuffers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumBuffers), buffers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_DOMAIN)
		{
			myDeviceContext->DSSetConstantBuffers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumBuffers), buffers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_GEOMETRY)
		{
			myDeviceContext->GSSetConstantBuffers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumBuffers), buffers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_PIXEL)
		{
			myDeviceContext->PSSetConstantBuffers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumBuffers), buffers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_COMPUTE)
		{
			myDeviceContext->CSSetConstantBuffers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumBuffers), buffers);
		}

		delete[] buffers;

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindVertexBuffers(VertexBuffer* const* aVertexBuffers, size_t aNumBuffers, unsigned int aStartSlot, const size_t* aStrides, const size_t* aOffsets)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindVertexBuffer"

		GRAPHICS_ASSERT(aVertexBuffers != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aVertexBuffers) " cannot be nullptr!");
		GRAPHICS_ASSERT(aNumBuffers > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBuffers) " must be greater than 0!");

		UINT* strides = new UINT[aNumBuffers];
		UINT* offsets = new UINT[aNumBuffers];
		ID3D11Buffer** buffers = new ID3D11Buffer * [aNumBuffers];
		for (size_t i = 0; i < aNumBuffers; i++)
		{
			if (aStrides)
			{
				strides[i] = static_cast<UINT>(aStrides[i]);
			}
			else
			{
				strides[i] = 0;
			}

			if (aOffsets)
			{
				offsets[i] = static_cast<UINT>(aOffsets[i]);
			}
			else
			{
				offsets[i] = 0;
			}

			if (aVertexBuffers[i])
			{
				VertexBuffer_DX11Impl* vertexBuffer = dynamic_cast<VertexBuffer_DX11Impl*>(aVertexBuffers[i]);
				GRAPHICS_ASSERT(vertexBuffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, "A " STRINGIFY(VertexBuffer) " at " STRINGIFY(aVertexBuffers) "[%u] failed to cast to " STRINGIFY(VertexBuffer_DX11Impl*) "!", i);
				GRAPHICS_ASSERT(IsDeviceObject(vertexBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, "A " STRINGIFY(VertexBuffer) " (%u) at " STRINGIFY(aVertexBuffers) "[%u] was not from the same device (%u)!", GetID(), i, vertexBuffer->GetID().Device);
				buffers[i] = vertexBuffer->GetBuffer();
			}
			else
			{
				buffers[i] = NULL;
			}
		}

		myDeviceContext->IASetVertexBuffers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumBuffers), buffers, strides, offsets);

		delete[] strides;
		delete[] offsets;
		delete[] buffers;

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindIndexBuffer(IndexBuffer* aIndexBuffer)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindIndexBuffer"

		ID3D11Buffer* buffer = nullptr;
		if (aIndexBuffer)
		{
			GRAPHICS_ASSERT(IsDeviceObject(aIndexBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aIndexBuffer) " (%u) was not from the same device (%u)!", GetID(), aIndexBuffer->GetID().Device);

			IndexBuffer_DX11Impl* indexBuffer = dynamic_cast<IndexBuffer_DX11Impl*>(aIndexBuffer);
			GRAPHICS_ASSERT(indexBuffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aIndexBuffer) " could not cast to " STRINGIFY(IndexBuffer_DX11Impl*) "!");
			buffer = indexBuffer->GetBuffer();
		}

		myDeviceContext->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindShaderResources(ShaderResource* const* aShaderResources, size_t aNumShaderResources, unsigned int aStartSlot, GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateConstantBuffer"

		//GRAPHICS_ASSERT(aShaderResources != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderResources) " cannot be nullptr!");
		GRAPHICS_ASSERT(aStartSlot + aNumShaderResources <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aStartSlot) " + " STRINGIFY(aNumShaderResources) " must be lower than '" STRINGIFY(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) "'!");
		GRAPHICS_ASSERT(aNumShaderResources > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumShaderResources) " must be greater than 0!");

		ID3D11ShaderResourceView** srvs = new ID3D11ShaderResourceView * [aNumShaderResources];
		for (size_t i = 0; i < aNumShaderResources; i++)
		{
			if (aShaderResources != nullptr && aShaderResources[i] != nullptr)
			{
				ShaderResource_DX11Impl* shaderResource = dynamic_cast<ShaderResource_DX11Impl*>(aShaderResources[i]);
				GRAPHICS_ASSERT(shaderResource != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, "A " STRINGIFY(ShaderResource) " at " STRINGIFY(aShaderResources) "[%u] failed to cast to " STRINGIFY(ShaderResource_DX11Impl*) "!", i);
				GRAPHICS_ASSERT(IsDeviceObject(shaderResource), LOG_CATEGORY, result::INVALID_ARGUMENT, "A " STRINGIFY(ConstantBuffer) " (%u) at " STRINGIFY(aShaderResources) "[%u] was not from the same device (%u)!", GetID(), i, shaderResource->GetID().Device);
				srvs[i] = shaderResource->GetShaderResourceView();
			}
			else
			{
				srvs[i] = NULL;
			}
		}

		if (aShaderFlags & GRAPHICS_SHADER_STAGE_VERTEX)
		{
			myDeviceContext->VSSetShaderResources(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumShaderResources), srvs);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_HULL)
		{
			myDeviceContext->HSSetShaderResources(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumShaderResources), srvs);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_DOMAIN)
		{
			myDeviceContext->DSSetShaderResources(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumShaderResources), srvs);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_GEOMETRY)
		{
			myDeviceContext->GSSetShaderResources(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumShaderResources), srvs);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_PIXEL)
		{
			myDeviceContext->PSSetShaderResources(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumShaderResources), srvs);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_COMPUTE)
		{
			myDeviceContext->CSSetShaderResources(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumShaderResources), srvs);
		}

		delete[] srvs;

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindTargets(RenderTarget* const* aRenderTargets, size_t aNumRenderTargets, DepthStencil* aDepthStencil)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindRenderTargets"

		GRAPHICS_ASSERT(aNumRenderTargets <= GRAPHICS_MAX_BINDABLE_RENDER_TARGETS, LOG_CATEGORY, result::INVALID_ARGUMENT, "aNumRenderTargets must be lower than '" STRINGIFY(GRAPHICS_MAX_BINDABLE_RENDER_TARGETS) "'");
		GRAPHICS_ASSERT(aRenderTargets != nullptr || aNumRenderTargets == 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumRenderTargets) " must be 0 if " STRINGIFY(aRenderTargets) " is nullptr!");

		ID3D11RenderTargetView* rtvs[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS] = {};
		ID3D11RenderTargetView** pRtvs = (aRenderTargets == nullptr) ? NULL : rtvs;
		for (size_t i = 0; i < aNumRenderTargets; i++)
		{
			if (aRenderTargets[i])
			{
				RenderTarget_DX11Impl* renderTarget = dynamic_cast<RenderTarget_DX11Impl*>(aRenderTargets[i]);
				GRAPHICS_ASSERT(renderTarget != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, "A " STRINGIFY(RenderTarget) " at " STRINGIFY(aRenderTargets) "[%u] failed to cast to " STRINGIFY(RenderTarget_DX11Impl*) "!", i);
				GRAPHICS_ASSERT(IsDeviceObject(renderTarget), LOG_CATEGORY, result::INVALID_ARGUMENT, "A " STRINGIFY(ConstantBuffer) " (%u) at " STRINGIFY(aRenderTargets) "[%u] was not from the same device (%u)!", GetID(), i, renderTarget->GetID().Device);
				rtvs[i] = renderTarget->GetRenderTargetView();
			}
			else
			{
				rtvs[i] = NULL;
			}
		}

		ID3D11DepthStencilView* dsv = NULL;
		DepthStencil_DX11Impl* depthStencil = dynamic_cast<DepthStencil_DX11Impl*>(aDepthStencil);
		if (depthStencil)
		{
			dsv = depthStencil->GetDepthStencilView();
		}

		myDeviceContext->OMSetRenderTargets(static_cast<UINT>(aNumRenderTargets), pRtvs, dsv);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::BindSamplerStates(SamplerState* const* aSamplerStates, size_t aNumSamplerStates, unsigned int aStartSlot, GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::BindSamplerStates"

		//GRAPHICS_ASSERT(aSamplerStates != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSamplerStates) " cannot be nullptr!");
		GRAPHICS_ASSERT(aStartSlot + aNumSamplerStates <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aStartSlot) " + " STRINGIFY(aNumSamplerStates) " must be lower than '" STRINGIFY(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT) "'!");
		GRAPHICS_ASSERT(aNumSamplerStates > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumSamplerStates) " must be greater than 0!");

		ID3D11SamplerState** samplers = new ID3D11SamplerState * [aNumSamplerStates];
		for (size_t i = 0; i < aNumSamplerStates; i++)
		{
			if (aSamplerStates != nullptr && aSamplerStates[i] != nullptr)
			{
				SamplerState_DX11Impl* samplerState = dynamic_cast<SamplerState_DX11Impl*>(aSamplerStates[i]);
				GRAPHICS_ASSERT(samplerState != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, "A " STRINGIFY(SamplerState) " at " STRINGIFY(aSamplerStates) "[%u] failed to cast to " STRINGIFY(SamplerState_DX11Impl*) "!", i);
				GRAPHICS_ASSERT(IsDeviceObject(samplerState), LOG_CATEGORY, result::INVALID_ARGUMENT, "A " STRINGIFY(ConstantBuffer) " (%u) at " STRINGIFY(aSamplerStates) "[%u] was not from the same device (%u)!", GetID(), i, samplerState->GetID().Device);
				samplers[i] = samplerState->GetSamplerState();
			}
			else
			{
				samplers[i] = NULL;
			}
		}

		if (aShaderFlags & GRAPHICS_SHADER_STAGE_VERTEX)
		{
			myDeviceContext->VSSetSamplers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumSamplerStates), samplers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_HULL)
		{
			myDeviceContext->HSSetSamplers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumSamplerStates), samplers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_DOMAIN)
		{
			myDeviceContext->DSSetSamplers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumSamplerStates), samplers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_GEOMETRY)
		{
			myDeviceContext->GSSetSamplers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumSamplerStates), samplers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_PIXEL)
		{
			myDeviceContext->PSSetSamplers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumSamplerStates), samplers);
		}
		if (aShaderFlags & GRAPHICS_SHADER_STAGE_COMPUTE)
		{
			myDeviceContext->CSSetSamplers(static_cast<UINT>(aStartSlot), static_cast<UINT>(aNumSamplerStates), samplers);
		}

		delete[] samplers;

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::CopyTexture(Texture2D* aSource, Texture2D* aDestination)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CopyTexture"

		Texture2D_DX11Impl* source = dynamic_cast<Texture2D_DX11Impl*>(aSource);
		GRAPHICS_ASSERT(source != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSource) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");
		Texture2D_DX11Impl* destination = dynamic_cast<Texture2D_DX11Impl*>(aDestination);
		GRAPHICS_ASSERT(destination != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDestination) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");

		ID3D11Texture2D* dxSource = source->GetTexture();
		GRAPHICS_ASSERT(dxSource != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSource) " has no valid texture assigned.");
		ID3D11Texture2D* dxDestination = destination->GetTexture();
		GRAPHICS_ASSERT(dxDestination != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDestination) " has no valid texture assigned.");

		myDeviceContext->CopyResource(dxDestination, dxSource);

		return result::OK;
#undef LOG_CATEGORY
	}
//	result::Code DeviceContext_DX11Impl::Map(Texture2D* aTexture, unsigned int aSubResource, GRAPHICS_MAP aMapType, GRAPHICS_MAP_FLAGS aMapFlags, void** outData, unsigned int* outRowPitch, unsigned int* outDepthPitch)
//	{
//#define LOG_CATEGORY CLASS_CATEGORY"::Map"
//
//		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
//		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");
//
//		ID3D11Texture2D* dxTexture = texture->GetTexture();
//		GRAPHICS_ASSERT(dxTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " has no valid texture assigned.");
//
//		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
//
//		HRESULT result = myDeviceContext->Map(
//			dxTexture,
//			aSubResource,
//			dx11::GetD3D11Map(aMapType),
//			dx11::GetD3D11MapFlag(aMapFlags),
//			&mappedSubresource
//		);
//
//		if (outData != nullptr)
//		{
//			*outData = mappedSubresource.pData;
//		}
//		if (outRowPitch != nullptr)
//		{
//			*outRowPitch = mappedSubresource.RowPitch;
//		}
//		if (outDepthPitch != nullptr)
//		{
//			*outDepthPitch = mappedSubresource.DepthPitch;
//		}
//
//		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11DeviceContext::Map, LOG_CATEGORY);
//		result;
//
//		return result::OK;
//#undef LOG_CATEGORY
//	}
//	result::Code DeviceContext_DX11Impl::Unmap(Texture2D* aTexture, unsigned int aSubResource)
//	{
//#define LOG_CATEGORY CLASS_CATEGORY"::Unmap"
//
//		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
//		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");
//
//		ID3D11Texture2D* dxTexture = texture->GetTexture();
//		GRAPHICS_ASSERT(dxTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " has no valid texture assigned.");
//
//		myDeviceContext->Unmap(
//			dxTexture,
//			aSubResource
//		);
//
//		return result::OK;
//#undef LOG_CATEGORY	
//	}
//	result::Code DeviceContext_DX11Impl::ResizeTexture(Texture2D* aTexture, unsigned int aWidth, unsigned int aHeight)
//	{
//#define LOG_CATEGORY CLASS_CATEGORY"::ResizeTexture"
//
//		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
//		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");
//
//		Texture2DDesc desc = texture->GetDesc();
//		if (desc.Width == aWidth && desc.Height == aHeight)
//		{
//			return result::OK;	// No need to resize if the texture already is that size.
//		}
//		desc.Width = aWidth;
//		desc.Height = aHeight;
//
//		graphics::PImpl<Texture2D> newTexture;
//		result::Code result = CreateTexture2D(
//			&newTexture,
//			&desc
//		);
//		GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to resize Texture2D!");
//
//		Texture2D_DX11Impl* newTextureImpl = dynamic_cast<Texture2D_DX11Impl*>(newTexture.Get());
//
//		texture->SetTexture(newTextureImpl->GetTexture());
//
//		return result::OK;
//#undef LOG_CATEGORY	
//	}
	result::Code DeviceContext_DX11Impl::ClearState()
	{
		myDeviceContext->ClearState();
		return result::OK;
	}
//	result::Code DeviceContext_DX11Impl::GetBuffer(Texture2D** outBuffer)
//	{
//#define LOG_CATEGORY CLASS_CATEGORY"::SetTopology"
//
//		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
//		HRESULT result = mySwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), &texture);
//
//		GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::GetBuffer, LOG_CATEGORY);
//		result;
//
//		Texture2DDesc desc = {};
//
//		D3D11_TEXTURE2D_DESC dx11Desc = {};
//		texture->GetDesc(&dx11Desc);
//
//		desc.Name = "Backbuffer Texture";
//		desc.BindFlags = dx11::GetGraphicsTextureFlags(static_cast<D3D11_BIND_FLAG>(dx11Desc.BindFlags));
//		desc.ResourceFlags = dx11::GetGraphicsResourceFlags(static_cast<D3D11_RESOURCE_MISC_FLAG>(dx11Desc.MiscFlags));
//		desc.Width = dx11Desc.Width;
//		desc.Height = dx11Desc.Height;
//		desc.MipLevels = dx11Desc.MipLevels;
//		desc.NumTextures = dx11Desc.ArraySize;
//		desc.Data = nullptr;
//		desc.NumData = 0;
//		desc.Format = dx11::GetGraphicsFormat(dx11Desc.Format);
//		desc.Usage = dx11::GetGraphicsUsage(dx11Desc.Usage);
//		desc.CPUAccessFlags = dx11::GetGraphicsCPUAccessFlags(static_cast<D3D11_CPU_ACCESS_FLAG>(dx11Desc.CPUAccessFlags));
//
//		*outBuffer = new Texture2D_DX11Impl(_createGraphicsID(), texture.Get(), desc);
//		(**outBuffer).AddReference();
//
//		return result::OK;
//#undef LOG_CATEGORY
//	}
	result::Code DeviceContext_DX11Impl::SetTopology(GRAPHICS_TOPOLOGY aTopology)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetTopology"

		myDeviceContext->IASetPrimitiveTopology(dx11::GetD3D11PrimitiveTopology(aTopology));

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::SetRasterizerState(RasterizerState* aRasterizerState)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetTopology"

		//GRAPHICS_ASSERT(aRasterizerState != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aRasterizerState) " cannot be nullptr!");
		GRAPHICS_ASSERT(aRasterizerState == nullptr || IsDeviceObject(aRasterizerState), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aRasterizerState) " (%u) was not from the same device (%u)!", GetID(), aRasterizerState->GetID().Device);

		if (aRasterizerState == nullptr)
		{
			myDeviceContext->RSSetState(NULL);
		}
		else
		{
			RasterizerState_DX11Impl* rasterizerState = dynamic_cast<RasterizerState_DX11Impl*>(aRasterizerState);
			GRAPHICS_ASSERT(rasterizerState != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aRasterizerState) " could not be cast to " STRINGIFY(RasterizerState_DX11Impl*) "!");
			myDeviceContext->RSSetState(rasterizerState->GetRasterizerState());
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::SetDepthStencilState(DepthStencilState* aDepthStencilState, unsigned int aStencilReference)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetDepthStencilState"

		GRAPHICS_ASSERT(aDepthStencilState == nullptr || IsDeviceObject(aDepthStencilState), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDepthStencilState) " (%u) was not from the same device (%u)!", GetID(), aDepthStencilState->GetID().Device);

		if (aDepthStencilState == nullptr)
		{
			myDeviceContext->OMSetDepthStencilState(NULL, aStencilReference);
		}
		else
		{
			DepthStencilState_DX11Impl* depthStencilState = dynamic_cast<DepthStencilState_DX11Impl*>(aDepthStencilState);
			GRAPHICS_ASSERT(depthStencilState != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(depthStencilState) " could not be cast to " STRINGIFY(DepthStencilState_DX11Impl*) "!");
			myDeviceContext->OMSetDepthStencilState(depthStencilState->GetDepthStencilState(), aStencilReference);
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::SetInputLayout(InputLayout* aInputLayout)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetInputLayout"

		GRAPHICS_ASSERT(aInputLayout != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aInputLayout) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aInputLayout), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aInputLayout) " (%u) was not from the same device (%u)!", GetID(), aInputLayout->GetID().Device);

		InputLayout_DX11Impl* inputLayout = dynamic_cast<InputLayout_DX11Impl*>(aInputLayout);
		GRAPHICS_ASSERT(inputLayout != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aInputLayout) " could not be cast to " STRINGIFY(InputLayout_DX11Impl*) "!");

		myDeviceContext->IASetInputLayout(inputLayout->GetInputLayout());

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::SetBlendState(BlendState* aBlendState, float aBlendFactors[4], unsigned int aSampleMask)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetBlendState"

		ID3D11BlendState* dxBlendState = nullptr;
		if (aBlendState != nullptr)
		{
			GRAPHICS_ASSERT(IsDeviceObject(aBlendState), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aBlendState) " (%u) was not from the same device (%u)!", GetID(), aBlendState->GetID().Device);
			BlendState_DX11Impl* blendState = dynamic_cast<BlendState_DX11Impl*>(aBlendState);
			GRAPHICS_ASSERT(blendState != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aBlendState) " could not be cast to " STRINGIFY(BlendState_DX11Impl*) "!");
			dxBlendState = blendState->GetBlendState();
		}

		myDeviceContext->OMSetBlendState(dxBlendState, aBlendFactors, aSampleMask);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::Draw(unsigned int aVertexCount, unsigned int aWhere)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Draw"

		myDeviceContext->Draw(static_cast<UINT>(aVertexCount), static_cast<UINT>(aWhere));

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::DrawIndexed(unsigned int aCount, unsigned int aWhere, unsigned int aVertexWhere)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::DrawIndexed"

		myDeviceContext->DrawIndexed(static_cast<UINT>(aCount), static_cast<UINT>(aWhere), static_cast<UINT>(aVertexWhere));

		return result::OK;
#undef LOG_CATEGORY
	}
//	result::Code DeviceContext_DX11Impl::Present(GRAPHICS_PRESENT_FLAGS aFlags)
//	{
//#define LOG_CATEGORY CLASS_CATEGORY"::Present"
//
//		UINT presentationFlags = NULL;
//
//		if (aFlags & GRAPHICS_PRESENT_ALLOW_TEARING)
//		{
//			GRAPHICS_ASSERT((mySwapChainFlags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING) != 0, LOG_CATEGORY, result::INVALID_STATE, STRINGIFY(aFlags) " contained " STRINGIFY(GRAPHICS_PRESENT_ALLOW_TEARING) ", but " STRINGIFY(mySwapChainFlags) " does not have flag " STRINGIFY(DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING) ".");
//			presentationFlags |= DXGI_PRESENT_ALLOW_TEARING;
//		}
//
//		HRESULT result = mySwapChain->Present(0, presentationFlags);
//
//		if (result == DXGI_ERROR_DEVICE_REMOVED)
//		{
//			result = myDevice->GetDeviceRemovedReason();
//		}
//
//		if (result != DXGI_STATUS_OCCLUDED)
//		{
//			GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::Present, LOG_CATEGORY);
//		}
//
//		return result::OK;
//#undef LOG_CATEGORY
//	}
	result::Code DeviceContext_DX11Impl::ClearRenderTarget(RenderTarget* aRenderTarget, const float aColor[4])
	{
#define LOG_CATEGORY CLASS_CATEGORY"::ClearRenderTarget"

		GRAPHICS_ASSERT(aRenderTarget != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aRenderTarget) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aRenderTarget), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aRenderTarget) " (%u) was not from the same device (%u)!", GetID(), aRenderTarget->GetID().Device);

		RenderTarget_DX11Impl* renderTarget = dynamic_cast<RenderTarget_DX11Impl*>(aRenderTarget);
		GRAPHICS_ASSERT(renderTarget != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aRenderTarget) " could not be cast to " STRINGIFY(RenderTarget_DX11Impl*) "!");

		myDeviceContext->ClearRenderTargetView(renderTarget->GetRenderTargetView(), aColor);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::ClearDepthStencil(DepthStencil* aDepthStencil, float aDepth, unsigned char aStencil)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::ClearDepthStencil"

		GRAPHICS_ASSERT(aDepthStencil != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDepthStencil) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aDepthStencil), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDepthStencil) " (%u) was not from the same device (%u)!", GetID(), aDepthStencil->GetID().Device);

		DepthStencil_DX11Impl* depthStencil = dynamic_cast<DepthStencil_DX11Impl*>(aDepthStencil);
		GRAPHICS_ASSERT(depthStencil != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aDepthStencil) " could not be cast to " STRINGIFY(DepthStencil_DX11Impl*) "!");

		myDeviceContext->ClearDepthStencilView(
			depthStencil->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			static_cast<FLOAT>(aDepth),
			static_cast<UINT8>(aStencil)
		);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::SetViewports(Viewport* aViewports, size_t aNumViewports)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetViewports"

		GRAPHICS_ASSERT(aViewports != nullptr || aNumViewports == 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aViewport) " cannot be nullptr when " STRINGIFY(aNumViewports) " is not 0!");
		GRAPHICS_ASSERT(aNumViewports <= GRAPHICS_MAX_BINDABLE_RENDER_TARGETS, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumViewports) " cannot be larger than '" STRINGIFY(GRAPHICS_MAX_BINDABLE_RENDER_TARGETS) "'!");

		D3D11_VIEWPORT viewports[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS] = {};

		for (size_t i = 0; i < aNumViewports; i++)
		{
			viewports[i].TopLeftX = aViewports[i].Left;
			viewports[i].Width = aViewports[i].Right - aViewports[i].Left;
			viewports[i].TopLeftY = aViewports[i].Top;
			viewports[i].Height = aViewports[i].Bottom - aViewports[i].Top;
			viewports[i].MinDepth = aViewports[i].Front;
			viewports[i].MaxDepth = aViewports[i].Back;
		}

		myDeviceContext->RSSetViewports(static_cast<UINT>(aNumViewports), viewports);

		return result::OK;
#undef LOG_CATEGORY
	}
//	result::Code DeviceContext_DX11Impl::UpdateBackbufferSize(unsigned int aWidth, unsigned int aHeight)
//	{
//#define LOG_CATEGORY CLASS_CATEGORY"::UpdateBackbufferSize"
//
//		myDeviceContext->ClearState();
//		HRESULT result = mySwapChain->ResizeBuffers(0, static_cast<UINT>(aWidth), static_cast<UINT>(aHeight), DXGI_FORMAT_UNKNOWN, mySwapChainFlags);
//
//		GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::ResizeBuffers, LOG_CATEGORY);
//		result;
//
//		return result::OK;
//#undef LOG_CATEGORY
//	}
	result::Code DeviceContext_DX11Impl::PushEvent(const char* aName, int* outNumEvents)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::PushEvent"

		int size = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, aName, -1, NULL, 0);

		LPWSTR name = new WCHAR[size];

		int result = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, aName, -1, name, size);

		if (result == 0)
		{
			DWORD error = GetLastError();
			GRAPHICS_LOG_FAILED_WIN32_API_CALL(error, MultiByteToWideChar, LOG_CATEGORY);
			error;
			return result::SYSTEM_ERROR;
		}

		INT numEvents = myUserDefinedAnnotation->BeginEvent(name);
		delete[] name;

		if (outNumEvents)
		{
			*outNumEvents = static_cast<int>(numEvents);
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code DeviceContext_DX11Impl::PopEvent(int* outNumEvents)
	{
		INT numEvents = myUserDefinedAnnotation->EndEvent();

		if (outNumEvents)
		{
			*outNumEvents = static_cast<int>(numEvents);
		}
		return result::OK;
	}
	bool DeviceContext_DX11Impl::IsDeviceObject(const GraphicsObject* aGraphicsObject) const noexcept
	{
		return aGraphicsObject != nullptr && GetID().Device == aGraphicsObject->GetID().Device;
	}
	result::Code DeviceContext_DX11Impl::_updateBuffer(ID3D11Buffer* aBuffer, const void* aData, size_t aDataSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::_updateBuffer"

		GRAPHICS_ASSERT(aBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(aData != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aData) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDataSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDataSize) " must be greater than 0!");

		D3D11_MAPPED_SUBRESOURCE map = {};

		HRESULT result = myDeviceContext->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &map);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11DeviceContext::Map, LOG_CATEGORY);
		result;

		memcpy(map.pData, aData, aDataSize);

		myDeviceContext->Unmap(aBuffer, 0);

		return result::OK;
#undef LOG_CATEGORY
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY