#include "RenderHardwareInterface.pch.h"
#include "Device_DX11Impl.h"
#include "Common_DX11.h"
#include "GAssert_DX11.h"

#include "Objects/ImmediateDeviceContext_DX11Impl.h"
#include "Objects/DeferredDeviceContext_DX11Impl.h"

#include "Objects/Data_DX11Impl.h"

#include "Objects/Texture2D_DX11Impl.h"
#include "Objects/Texture2DArray_DX11Impl.h"
#include "Objects/ShaderResource_DX11Impl.h"
#include "Objects/RenderTarget_DX11Impl.h"
#include "Objects/DepthStencil_DX11Impl.h"
#include "Objects/InputLayout_DX11Impl.h"
#include "Objects/SamplerState_DX11Impl.h"
#include "Objects/BlendState_DX11Impl.h"
#include "Objects/RasterizerState_DX11Impl.h"
#include "Objects/DepthStencilState_DX11Impl.h"

#include "Objects/Buffers/ConstantBuffer_DX11Impl.h"
#include "Objects/Buffers/VertexBuffer_DX11Impl.h"
#include "Objects/Buffers/IndexBuffer_DX11Impl.h"

#include "Objects/Shaders/VertexShader_DX11Impl.h"
#include "Objects/Shaders/HullShader_DX11Impl.h"
#include "Objects/Shaders/DomainShader_DX11Impl.h"
#include "Objects/Shaders/GeometryShader_DX11Impl.h"
#include "Objects/Shaders/PixelShader_DX11Impl.h"
#include "Objects/Shaders/ComputeShader_DX11Impl.h"

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "Device_DX11Impl.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Device_DX11Impl"

namespace graphics
{
	Device_DX11Impl::Device_DX11Impl(Window_DX11Impl* aWindow, ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, IDXGISwapChain* aSwapChain, UINT aSwapChainFlags) :
		myWindow(aWindow),
		myDevice(aDevice),
		myDeviceContext(aDeviceContext),
		mySwapChain(aSwapChain),
		mySwapChainFlags(aSwapChainFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Device_DX11Impl"

		if (myDeviceContext)
		{
			myDeviceContext.As(&myUserDefinedAnnotation);
		}
		else
		{
			LOG_WARNING(LOG_CATEGORY, "Failed to retrieve D3DUserDefinedAnnotation from " STRINGIFY(aDeviceContext) "!");
		}

		result::Code result = _createBackbufferTexture(&myBackbufferTexture);
		result = _createBackbufferRenderTarget(&myBackbufferRenderTarget);
		result = _createBackbufferDepthTexture(&myBackbufferDepthTexture);
		result = _createBackbufferDepthStencil(&myBackbufferDepthStencil);

#undef LOG_CATEGORY
	}
	Device_DX11Impl::~Device_DX11Impl() = default;
	Window_DX11Impl* Device_DX11Impl::GetViewport()
	{
		return myWindow.Get();
	}
	const Window_DX11Impl* Device_DX11Impl::GetViewport() const
	{
		return myWindow.Get();
	}
	ID3D11Device* Device_DX11Impl::GetDevice()
	{
		return myDevice.Get();
	}
	const ID3D11Device* Device_DX11Impl::GetDevice() const
	{
		return myDevice.Get();
	}
	ID3D11DeviceContext* Device_DX11Impl::GetDeviceContext()
	{
		return myDeviceContext.Get();
	}
	const ID3D11DeviceContext* Device_DX11Impl::GetDeviceContext() const
	{
		return myDeviceContext.Get();
	}
	IDXGISwapChain* Device_DX11Impl::GetSwapChain()
	{
		return mySwapChain.Get();
	}
	const IDXGISwapChain* Device_DX11Impl::GetSwapChain() const
	{
		return mySwapChain.Get();
	}
	result::Code Device_DX11Impl::CreateData(Data** outData, const DataDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateData"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc) " cannot be nullptr!");
		GRAPHICS_ASSERT(outData != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outData) " cannot be nullptr!");

		if (*outData != nullptr)
		{
			(**outData).RemoveReference();
		}
		*outData = new Data_DX11Impl(_createGraphicsID(), *aDesc);
		(**outData).AddReference();

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateImmediateDeviceContext(ImmediateDeviceContext** outDeviceContext)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateImmediateDeviceContext"
		GRAPHICS_ASSERT(outDeviceContext != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outDeviceContext) " cannot be nullptr!");

		if (*outDeviceContext != nullptr)
		{
			(**outDeviceContext).RemoveReference();
		}
		*outDeviceContext = new ImmediateDeviceContext_DX11Impl(_createGraphicsID(), GetDeviceContext());
		(**outDeviceContext).AddReference();

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateDeferredDeviceContext(DeferredDeviceContext** outDeviceContext)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateDeferredDeviceContext"
		GRAPHICS_ASSERT(outDeviceContext != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outDeviceContext) " cannot be nullptr!");

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
		HRESULT result = myDevice->CreateDeferredContext(NULL, &deviceContext);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateDeferredContext, LOG_CATEGORY);

		if (*outDeviceContext != nullptr)
		{
			(**outDeviceContext).RemoveReference();
		}
		*outDeviceContext = new DeferredDeviceContext_DX11Impl(_createGraphicsID(), deviceContext.Get());
		(**outDeviceContext).AddReference();

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateTexture2D(Texture2D** outTexture2D, const Texture2DDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateTexture2D"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc) " cannot be nullptr!");
		GRAPHICS_ASSERT(outTexture2D != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outTexture2D) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->Width != 0 && aDesc->Height != 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Width) " and " STRINGIFY(aDesc->Height) " must be larger than 0!");
		GRAPHICS_ASSERT(aDesc->Format != GRAPHICS_FORMAT_UNKNOWN, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Format) " cannot be '" STRINGIFY(GRAPHICS_FORMAT_UNKNOWN) "'!");

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = static_cast<UINT>(aDesc->Width);
		desc.Height = static_cast<UINT>(aDesc->Height);
		desc.MipLevels = static_cast<UINT>(aDesc->MipLevels);
		desc.ArraySize = static_cast<UINT>(aDesc->NumTextures);
		desc.Format = dx11::GetDXGIFormat(aDesc->Format);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = dx11::GetD3D11Usage(aDesc->Usage);
		desc.BindFlags = dx11::GetD3D11BindFlags(aDesc->BindFlags);
		desc.CPUAccessFlags = dx11::GetD3D11CPUAccessFlag(aDesc->CPUAccessFlags);
		desc.MiscFlags = dx11::GetD3D11ResourceMiscFlag(aDesc->ResourceFlags);

		D3D11_SUBRESOURCE_DATA* data = nullptr;
		if (aDesc->Data != nullptr)
		{
			data = new D3D11_SUBRESOURCE_DATA[aDesc->NumData];
			for (size_t i = 0; i < aDesc->NumData; i++)
			{
				data[i].pSysMem = aDesc->Data[i].Data;
				data[i].SysMemPitch = aDesc->Data[i].RowPitch;
				data[i].SysMemSlicePitch = aDesc->Data[i].DepthPitch;
			}
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		HRESULT result = myDevice->CreateTexture2D(&desc, data, &texture);

		if (data != nullptr)
		{
			delete[] data;
		}

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateTexture2D, LOG_CATEGORY);
		result;

		if (*outTexture2D != nullptr)
		{
			(**outTexture2D).RemoveReference();
		}
		*outTexture2D = new Texture2D_DX11Impl(_createGraphicsID(), texture.Get(), *aDesc);
		(**outTexture2D).AddReference();

		_setName(aDesc->Name.c_str(), *outTexture2D);

		return result::OK;

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateShaderResource(ShaderResource** outShaderResource, const ShaderResourceDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateShaderResource"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");
		GRAPHICS_ASSERT(outShaderResource != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outShaderResource)" cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->Texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture)" cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->ViewDimension != GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION_UNKNOWN, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->ViewDimension) " cannot be " STRINGIFY(GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION_UNKNOWN) "!");
		GRAPHICS_ASSERT(IsDeviceObject(aDesc->Texture), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture) " (%u) was not from the same device (%u)!", GetID(), aDesc->Texture->GetID().Device);
		GRAPHICS_ASSERT(aDesc->FirstSlice + aDesc->NumSlices <= aDesc->Texture->GetDesc().NumTextures, LOG_CATEGORY, result::OUT_OF_RANGE, STRINGIFY(aDesc->FirstSlice) " + " STRINGIFY(aDesc->NumSlices) " cannot be higher than size of " STRINGIFY(aDesc->Texture) "!");

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aDesc->Texture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl*) "!");

		D3D11_TEXTURE2D_DESC texDesc = {};
		texture->GetTexture()->GetDesc(&texDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.ViewDimension = dx11::GetD3D11SRVDimension(aDesc->ViewDimension);
		desc.Format = dx11::GetDXGIFormat(aDesc->Format);

		switch (desc.ViewDimension)
		{
		case D3D11_SRV_DIMENSION_TEXTURE1D:
			desc.Texture1D.MostDetailedMip = static_cast<UINT>(aDesc->MostDetailedMip);
			desc.Texture1D.MipLevels = static_cast<UINT>(aDesc->MipLevels);
			break;
		case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
			desc.Texture1DArray.MostDetailedMip = static_cast<UINT>(aDesc->MostDetailedMip);
			desc.Texture1DArray.MipLevels = static_cast<UINT>(aDesc->MipLevels);
			desc.Texture1DArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture1DArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2D:
			desc.Texture2D.MostDetailedMip = static_cast<UINT>(aDesc->MostDetailedMip);
			desc.Texture2D.MipLevels = static_cast<UINT>(aDesc->MipLevels);
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
			desc.Texture2DArray.MostDetailedMip = static_cast<UINT>(aDesc->MostDetailedMip);
			desc.Texture2DArray.MipLevels = static_cast<UINT>(aDesc->MipLevels);
			desc.Texture2DArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture2DArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2DMS:
			break;
		case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
			desc.Texture2DMSArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture2DMSArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_SRV_DIMENSION_TEXTURE3D:
			desc.Texture3D.MostDetailedMip = static_cast<UINT>(aDesc->MostDetailedMip);
			desc.Texture3D.MipLevels = static_cast<UINT>(aDesc->MipLevels);
			break;
		case D3D11_SRV_DIMENSION_TEXTURECUBE:
			desc.TextureCube.MostDetailedMip = static_cast<UINT>(aDesc->MostDetailedMip);
			desc.TextureCube.MipLevels = static_cast<UINT>(aDesc->MipLevels);
			break;
		case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
			desc.TextureCubeArray.MostDetailedMip = static_cast<UINT>(aDesc->MostDetailedMip);
			desc.TextureCubeArray.MipLevels = static_cast<UINT>(aDesc->MipLevels);
			desc.TextureCubeArray.First2DArrayFace = static_cast<UINT>(aDesc->FirstSlice);
			desc.TextureCubeArray.NumCubes = static_cast<UINT>(aDesc->NumSlices) / 6u;
			break;

		default:
			LOG_ERROR(LOG_CATEGORY, STRINGIFY(aDesc->ViewDimension) " '%u', is not implemented yet!", aDesc->ViewDimension);
			return result::INVALID_ARGUMENT;
		}

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		HRESULT result = myDevice->CreateShaderResourceView(texture->GetTexture(), &desc, &srv);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateShaderResourceView, LOG_CATEGORY);
		result;

		if (*outShaderResource != nullptr)
		{
			(**outShaderResource).RemoveReference();
		}
		*outShaderResource = new ShaderResource_DX11Impl(this, _createGraphicsID(), srv.Get(), *aDesc);
		(**outShaderResource).AddReference();

		_setName(aDesc->Name.c_str(), *outShaderResource);

		return result::OK;

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateRenderTarget(RenderTarget** outRenderTarget, const RenderTargetDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateRenderTarget"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");
		GRAPHICS_ASSERT(outRenderTarget != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outRenderTarget) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->Texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture) " cannot be nullptr!");
		//GRAPHICS_ASSERT(aDesc->Texture != myBackbufferTexture.Get(), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture) " cannot be backbuffer texture! Call GetBufferRenderTarget instead.");
		GRAPHICS_ASSERT(aDesc->ViewDimension != GRAPHICS_RENDER_TARGET_VIEW_DIMENSION_UNKNOWN, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->ViewDimension) " cannot be " STRINGIFY(GRAPHICS_RENDER_TARGET_VIEW_DIMENSION_UNKNOWN) "!");
		GRAPHICS_ASSERT(IsDeviceObject(aDesc->Texture), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture) " (%u) was not from the same device (%u)!", GetID(), aDesc->Texture->GetID().Device);

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aDesc->Texture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aDesc->Texture) " could not be cast to Texture2D_DX11Impl*!");

		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = dx11::GetDXGIFormat(aDesc->Format);
		desc.ViewDimension = dx11::GetD3D11RTVDimension(aDesc->ViewDimension);

		switch (desc.ViewDimension)
		{

		case D3D11_RTV_DIMENSION_TEXTURE1D:
			desc.Texture1D.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			break;
		case D3D11_RTV_DIMENSION_TEXTURE1DARRAY:
			desc.Texture1DArray.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			desc.Texture1DArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture1DArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_RTV_DIMENSION_TEXTURE2D:
			desc.Texture2D.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			break;
		case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
			desc.Texture2DArray.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			desc.Texture2DArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture2DArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_RTV_DIMENSION_TEXTURE2DMS:
			break;
		case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
			desc.Texture2DMSArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture2DMSArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_RTV_DIMENSION_TEXTURE3D:
			desc.Texture3D.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			desc.Texture3D.FirstWSlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture3D.WSize = static_cast<UINT>(aDesc->NumSlices);
			break;

		default:
			LOG_ERROR(LOG_CATEGORY, STRINGIFY(aDesc->ViewDimension) " '%u', is not implemented yet!", aDesc->ViewDimension);
			return result::INVALID_ARGUMENT;
		}

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
		HRESULT result = myDevice->CreateRenderTargetView(texture->GetTexture(), &desc, &rtv);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateRenderTargetView, LOG_CATEGORY);
		result;

		if (*outRenderTarget != nullptr)
		{
			(**outRenderTarget).RemoveReference();
		}
		*outRenderTarget = new RenderTarget_DX11Impl(this, _createGraphicsID(), rtv.Get(), *aDesc);
		(**outRenderTarget).AddReference();

		_setName(aDesc->Name.c_str(), *outRenderTarget);

		return result::OK;

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateDepthStencil(DepthStencil** outDepthStencil, const DepthStencilDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateDepthStencil"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");
		GRAPHICS_ASSERT(outDepthStencil != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outDepthStencil) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->Texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->ViewDimension != GRAPHICS_DEPTH_STENCIL_VIEW_DIMENSION_UNKNOWN, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->ViewDimension) " cannot be " STRINGIFY(GRAPHICS_DEPTH_STENCIL_VIEW_DIMENSION_UNKNOWN) "!");
		GRAPHICS_ASSERT(IsDeviceObject(aDesc->Texture), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture) " (%u) was not from the same device (%u)!", GetID(), aDesc->Texture->GetID().Device);
		GRAPHICS_ASSERT((aDesc->Texture->GetDesc().BindFlags & GRAPHICS_BIND_DEPTH_STENCIL) != 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->Texture) " must have flag '" STRINGIFY(GRAPHICS_BIND_DEPTH_STENCIL) "'!");

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aDesc->Texture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl*) "!");

		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = dx11::GetDXGIFormat(aDesc->Format);
		desc.ViewDimension = dx11::GetD3D11DSVDimension(aDesc->ViewDimension);
		desc.Flags = dx11::GetD3D11DSVFlag(aDesc->Flags);

		switch (desc.ViewDimension)
		{
		case D3D11_DSV_DIMENSION_TEXTURE1D:
			desc.Texture1D.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			break;
		case D3D11_DSV_DIMENSION_TEXTURE1DARRAY:
			desc.Texture1DArray.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			desc.Texture1DArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture1DArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_DSV_DIMENSION_TEXTURE2D:
			desc.Texture2D.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			break;
		case D3D11_DSV_DIMENSION_TEXTURE2DARRAY:
			desc.Texture2DArray.MipSlice = static_cast<UINT>(aDesc->MipSlice);
			desc.Texture2DArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture2DArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;
		case D3D11_DSV_DIMENSION_TEXTURE2DMS:
			break;
		case D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY:
			desc.Texture2DMSArray.FirstArraySlice = static_cast<UINT>(aDesc->FirstSlice);
			desc.Texture2DMSArray.ArraySize = static_cast<UINT>(aDesc->NumSlices);
			break;

		default:
			LOG_ERROR(LOG_CATEGORY, STRINGIFY(aDesc->ViewDimension) " '%u', is not implemented yet!", aDesc->ViewDimension);
			return result::INVALID_ARGUMENT;
		}

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;
		HRESULT result = myDevice->CreateDepthStencilView(texture->GetTexture(), &desc, &dsv);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateDepthStencilView, LOG_CATEGORY);
		result;

		if (*outDepthStencil != nullptr)
		{
			(**outDepthStencil).RemoveReference();
		}
		*outDepthStencil = new DepthStencil_DX11Impl(this, _createGraphicsID(), dsv.Get(), *aDesc);
		(**outDepthStencil).AddReference();

		_setName(aDesc->Name.c_str(), *outDepthStencil);

		return result::OK;

#undef LOG_CATEGORY
	}
	//	result::Code Device_DX11Impl::CreateConstantBuffer(ConstantBuffer** outConstantBuffer, const char* aName, const void* aData, size_t aSize, GRAPHICS_USAGE aUsage, GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags)
	//	{
	//#define LOG_CATEGORY CLASS_CATEGORY"::CreateConstantBuffer"
	//
	//		GRAPHICS_ASSERT(outConstantBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outConstantBuffer) " cannot be nullptr!");
	//		GRAPHICS_ASSERT(aSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSize) " must be greater than 0!");
	//		GRAPHICS_ASSERT(aSize % 16 == 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSize) " must be a multiple of 16!");
	//
	//		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	//		result::Code result = _createBuffer(
	//			&buffer,
	//			D3D11_BIND_CONSTANT_BUFFER,
	//			aData,
	//			aSize,
	//			aUsage,
	//			aCPUAccessFlags
	//		);
	//
	//		GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to create Constant buffer!");
	//
	//		*outConstantBuffer = new ConstantBuffer_DX11Impl(_createGraphicsID(), aSize, buffer.Get());
	//		(**outConstantBuffer).AddReference();
	//
	//		_setName(aName, *outConstantBuffer);
	//
	//		return result::OK;
	//#undef LOG_CATEGORY
	//	}
	result::Code Device_DX11Impl::CreateConstantBuffer(ConstantBuffer** outConstantBuffer, const ConstantBufferDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateConstantBuffer"

		GRAPHICS_ASSERT(outConstantBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outConstantBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->Size > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSize) " must be greater than 0!");
		GRAPHICS_ASSERT(aDesc->Size % 16 == 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSize) " must be a multiple of 16!");

		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		result::Code result = _createBuffer(
			&buffer,
			D3D11_BIND_CONSTANT_BUFFER,
			aDesc->InitialData,
			aDesc->Size,
			aDesc->Usage,
			aDesc->CPUAccessFlags
		);

		GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to create Constant buffer!");

		if (*outConstantBuffer != nullptr)
		{
			(**outConstantBuffer).RemoveReference();
		}
		*outConstantBuffer = new ConstantBuffer_DX11Impl(_createGraphicsID(), aDesc->Size, buffer.Get());
		(**outConstantBuffer).AddReference();

		_setName(aDesc->Name.c_str(), *outConstantBuffer);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateVertexBuffer(VertexBuffer** outVertexBuffer, const VertexBufferDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateVertexBuffer"

		GRAPHICS_ASSERT(outVertexBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outConstantBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->Size > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSize) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		result::Code result = _createBuffer(
			&buffer,
			D3D11_BIND_VERTEX_BUFFER,
			aDesc->Data,
			aDesc->Size,
			aDesc->Usage,
			aDesc->CPUAccessFlags
		);

		GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to create Vertex buffer!");

		if (*outVertexBuffer != nullptr)
		{
			(**outVertexBuffer).RemoveReference();
		}
		*outVertexBuffer = new VertexBuffer_DX11Impl(_createGraphicsID(), aDesc->Size, buffer.Get());
		(**outVertexBuffer).AddReference();

		_setName(aDesc->Name.c_str(), *outVertexBuffer);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateIndexBuffer(IndexBuffer** outIndexBuffer, const char* aName, const void* aData, size_t aSize, GRAPHICS_USAGE aUsage, GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateIndexBuffer"

		GRAPHICS_ASSERT(outIndexBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outIndexBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(aSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSize) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		result::Code result = _createBuffer(
			&buffer,
			D3D11_BIND_INDEX_BUFFER,
			aData,
			aSize,
			aUsage,
			aCPUAccessFlags
		);

		GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to create Index buffer!");

		if (*outIndexBuffer != nullptr)
		{
			(**outIndexBuffer).RemoveReference();
		}
		*outIndexBuffer = new IndexBuffer_DX11Impl(_createGraphicsID(), aSize, buffer.Get());
		(**outIndexBuffer).AddReference();

		_setName(aName, *outIndexBuffer);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateShader(VertexShader** outShader, const char* aName, void* aShaderByteCode, size_t aNumBytes)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateShader"

		GRAPHICS_ASSERT(outShader != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outShader) " cannot be nullptr!");
		GRAPHICS_ASSERT(aShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderByteCode) " cannot be nullptr!");
		GRAPHICS_ASSERT(aNumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBytes) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		HRESULT result = myDevice->CreateVertexShader(aShaderByteCode, static_cast<SIZE_T>(aNumBytes), NULL, &shader);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateVertexShader, LOG_CATEGORY);
		result;

		if (*outShader != nullptr)
		{
			(**outShader).RemoveReference();
		}
		*outShader = new VertexShader_DX11Impl(_createGraphicsID(), shader.Get());
		(**outShader).AddReference();

		_setName(aName, *outShader);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateShader(HullShader** outShader, const char* aName, void* aShaderByteCode, size_t aNumBytes)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateShader"

		GRAPHICS_ASSERT(outShader != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outShader) " cannot be nullptr!");
		GRAPHICS_ASSERT(aShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderByteCode) " cannot be nullptr!");
		GRAPHICS_ASSERT(aNumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBytes) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11HullShader> shader;
		HRESULT result = myDevice->CreateHullShader(aShaderByteCode, static_cast<SIZE_T>(aNumBytes), NULL, &shader);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateVertexShader, LOG_CATEGORY);
		result;

		if (*outShader != nullptr)
		{
			(**outShader).RemoveReference();
		}
		*outShader = new HullShader_DX11Impl(_createGraphicsID(), shader.Get());
		(**outShader).AddReference();

		_setName(aName, *outShader);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateShader(DomainShader** outShader, const char* aName, void* aShaderByteCode, size_t aNumBytes)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateShader"

		GRAPHICS_ASSERT(outShader != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outShader) " cannot be nullptr!");
		GRAPHICS_ASSERT(aShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderByteCode) " cannot be nullptr!");
		GRAPHICS_ASSERT(aNumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBytes) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11DomainShader> shader;
		HRESULT result = myDevice->CreateDomainShader(aShaderByteCode, static_cast<SIZE_T>(aNumBytes), NULL, &shader);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateVertexShader, LOG_CATEGORY);
		result;

		if (*outShader != nullptr)
		{
			(**outShader).RemoveReference();
		}
		*outShader = new DomainShader_DX11Impl(_createGraphicsID(), shader.Get());
		(**outShader).AddReference();

		_setName(aName, *outShader);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateShader(GeometryShader** outShader, const char* aName, void* aShaderByteCode, size_t aNumBytes)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateShader"

		GRAPHICS_ASSERT(outShader != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outShader) " cannot be nullptr!");
		GRAPHICS_ASSERT(aShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderByteCode) " cannot be nullptr!");
		GRAPHICS_ASSERT(aNumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBytes) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;
		HRESULT result = myDevice->CreateGeometryShader(aShaderByteCode, static_cast<SIZE_T>(aNumBytes), NULL, &shader);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateVertexShader, LOG_CATEGORY);
		result;

		if (*outShader != nullptr)
		{
			(**outShader).RemoveReference();
		}
		*outShader = new GeometryShader_DX11Impl(_createGraphicsID(), shader.Get());
		(**outShader).AddReference();

		_setName(aName, *outShader);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateShader(PixelShader** outShader, const char* aName, void* aShaderByteCode, size_t aNumBytes)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateShader"

		GRAPHICS_ASSERT(outShader != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outShader) " cannot be nullptr!");
		GRAPHICS_ASSERT(aShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderByteCode) " cannot be nullptr!");
		GRAPHICS_ASSERT(aNumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBytes) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		HRESULT result = myDevice->CreatePixelShader(aShaderByteCode, static_cast<SIZE_T>(aNumBytes), NULL, &shader);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateVertexShader, LOG_CATEGORY);
		result;

		if (*outShader != nullptr)
		{
			(**outShader).RemoveReference();
		}
		*outShader = new PixelShader_DX11Impl(_createGraphicsID(), shader.Get());
		(**outShader).AddReference();

		_setName(aName, *outShader);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateShader(ComputeShader** outShader, const char* aName, void* aShaderByteCode, size_t aNumBytes)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateShader"

		GRAPHICS_ASSERT(outShader != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outShader) " cannot be nullptr!");
		GRAPHICS_ASSERT(aShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderByteCode) " cannot be nullptr!");
		GRAPHICS_ASSERT(aNumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBytes) " must be greater than 0!");

		Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;
		HRESULT result = myDevice->CreateComputeShader(aShaderByteCode, static_cast<SIZE_T>(aNumBytes), NULL, &shader);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateVertexShader, LOG_CATEGORY);
		result;

		if (*outShader != nullptr)
		{
			(**outShader).RemoveReference();
		}
		*outShader = new ComputeShader_DX11Impl(_createGraphicsID(), shader.Get());
		(**outShader).AddReference();

		_setName(aName, *outShader);

		return result::OK;
#undef LOG_CATEGORY	
	}
	//	result::Code Device_DX11Impl::CreateInputLayout(InputLayout** outInputLayout, const char* aName, const LayoutDefinition& aLayoutDefinition, void* aShaderByteCode, size_t aNumBytes)
	//	{
	//#define LOG_CATEGORY CLASS_CATEGORY"::CreateInputLayout"
	//
	//		GRAPHICS_ASSERT(outInputLayout != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outInputLayout) " cannot be nullptr!");
	//		GRAPHICS_ASSERT(aShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aShaderByteCode) " cannot be nullptr!");
	//		GRAPHICS_ASSERT(aNumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aNumBytes) " must be greater than 0!");
	//
	//		const size_t numElements = aLayoutDefinition.Elements.size();
	//		D3D11_INPUT_ELEMENT_DESC* elements = new D3D11_INPUT_ELEMENT_DESC[numElements];
	//		for (size_t i = 0; i < numElements; i++)
	//		{
	//			elements[i].SemanticName = aLayoutDefinition.Elements[i].SemanticName.c_str();
	//			elements[i].SemanticIndex = static_cast<UINT>(aLayoutDefinition.Elements[i].SemanticIndex);
	//			elements[i].Format = dx11::GetDXGIFormat(aLayoutDefinition.Elements[i].Format);
	//			elements[i].InputSlot = static_cast<UINT>(aLayoutDefinition.Elements[i].InputSlot);
	//			elements[i].AlignedByteOffset = static_cast<UINT>(aLayoutDefinition.Elements[i].AlignedByteOffset);
	//			elements[i].InputSlotClass = dx11::GetInputClassification(aLayoutDefinition.Elements[i].InputSlotClass);
	//			elements[i].InstanceDataStepRate = static_cast<UINT>(aLayoutDefinition.Elements[i].InstanceDataStepRate);
	//		}
	//
	//		Microsoft::WRL::ComPtr<ID3D11InputLayout> il;
	//		HRESULT result = myDevice->CreateInputLayout(
	//			elements,
	//			static_cast<UINT>(numElements),
	//			aShaderByteCode,
	//			static_cast<SIZE_T>(aNumBytes),
	//			&il
	//		);
	//
	//		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateInputLayout, LOG_CATEGORY);
	//		result;
	//
	//		*outInputLayout = new InputLayout_DX11Impl(_createGraphicsID(), aLayoutDefinition, il.Get());
	//		(**outInputLayout).AddReference();
	//
	//		_setName(aName, *outInputLayout);
	//
	//		return result::OK;
	//#undef LOG_CATEGORY	
	//	}
	result::Code Device_DX11Impl::CreateInputLayout(InputLayout** outInputLayout, const InputLayoutDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateInputLayout"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");
		GRAPHICS_ASSERT(outInputLayout != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outInputLayout) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->ShaderByteCode != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->ShaderByteCode) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc->NumBytes > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc->NumBytes) " must be greater than 0!");

		const size_t numElements = aDesc->LayoutDefinition.Elements.size();
		D3D11_INPUT_ELEMENT_DESC* elements = new D3D11_INPUT_ELEMENT_DESC[numElements];
		for (size_t i = 0; i < numElements; i++)
		{
			elements[i].SemanticName = aDesc->LayoutDefinition.Elements[i].SemanticName.c_str();
			elements[i].SemanticIndex = static_cast<UINT>(aDesc->LayoutDefinition.Elements[i].SemanticIndex);
			elements[i].Format = dx11::GetDXGIFormat(aDesc->LayoutDefinition.Elements[i].Format);
			elements[i].InputSlot = static_cast<UINT>(aDesc->LayoutDefinition.Elements[i].InputSlot);
			elements[i].AlignedByteOffset = static_cast<UINT>(aDesc->LayoutDefinition.Elements[i].AlignedByteOffset);
			elements[i].InputSlotClass = dx11::GetInputClassification(aDesc->LayoutDefinition.Elements[i].InputSlotClass);
			elements[i].InstanceDataStepRate = static_cast<UINT>(aDesc->LayoutDefinition.Elements[i].InstanceDataStepRate);
		}

		Microsoft::WRL::ComPtr<ID3D11InputLayout> il;
		HRESULT result = myDevice->CreateInputLayout(
			elements,
			static_cast<UINT>(numElements),
			aDesc->ShaderByteCode,
			static_cast<SIZE_T>(aDesc->NumBytes),
			&il
		);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateInputLayout, LOG_CATEGORY);
		result;

		if (*outInputLayout != nullptr)
		{
			(**outInputLayout).RemoveReference();
		}
		*outInputLayout = new InputLayout_DX11Impl(_createGraphicsID(), aDesc->LayoutDefinition, il.Get());
		(**outInputLayout).AddReference();

		_setName(aDesc->Name.c_str(), *outInputLayout);

		return result::OK;
#undef LOG_CATEGORY	
		//		return CreateInputLayout(
		//			outInputLayout,
		//			aDesc->Name.c_str(),
		//			aDesc->LayoutDefinition,
		//			aDesc->ShaderByteCode,
		//			aDesc->NumBytes
		//		);
		//#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::CreateSamplerState(SamplerState** outSamplerState, const SamplerStateDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateSamplerState"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");
		GRAPHICS_ASSERT(outSamplerState != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outSamplerState) " cannot be nullptr!");

		D3D11_SAMPLER_DESC desc = {};
		desc.Filter = dx11::GetD3D11Filter(aDesc->Filter);
		desc.AddressU = dx11::GetD3D11TextureAddressMode(aDesc->AddressU);
		desc.AddressV = dx11::GetD3D11TextureAddressMode(aDesc->AddressV);
		desc.AddressW = dx11::GetD3D11TextureAddressMode(aDesc->AddressW);
		desc.MipLODBias = static_cast<FLOAT>(aDesc->MipLODBias);
		desc.MaxAnisotropy = static_cast<UINT>(aDesc->MaxAnistropy);
		desc.ComparisonFunc = dx11::GetD3D11ComparisonFunction(aDesc->Comparison);
		desc.BorderColor[0] = aDesc->BorderColor[0];
		desc.BorderColor[1] = aDesc->BorderColor[1];
		desc.BorderColor[2] = aDesc->BorderColor[2];
		desc.BorderColor[3] = aDesc->BorderColor[3];
		desc.MinLOD = static_cast<FLOAT>(aDesc->MinLOD);
		desc.MaxLOD = static_cast<FLOAT>(aDesc->MaxLOD);

		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		HRESULT result = myDevice->CreateSamplerState(&desc, &sampler);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateSamplerState, LOG_CATEGORY);
		result;

		if (*outSamplerState != nullptr)
		{
			(**outSamplerState).RemoveReference();
		}
		*outSamplerState = new SamplerState_DX11Impl(_createGraphicsID(), sampler.Get());
		(**outSamplerState).AddReference();

		_setName(aDesc->Name.c_str(), *outSamplerState);

		return result::OK;
#undef LOG_CATEGORY	
		//return CreateSamplerState(
		//	outSamplerState,
		//	aDesc->Name.c_str(),
		//	aDesc->Filter,
		//	aDesc->AddressU,
		//	aDesc->AddressV,
		//	aDesc->AddressW,
		//	aDesc->MipLODBias,
		//	aDesc->MaxAnistropy,
		//	aDesc->Comparison,
		//	aDesc->BorderColor,
		//	aDesc->MinLOD,
		//	aDesc->MaxLOD
		//);
//#undef LOG_CATEGORY	
	}
	//	result::Code Device_DX11Impl::CreateBlendState(BlendState** outBlendState, const char* aName, bool aEnableAlphaToCoverage, bool aEnableIndependentBlend, const RenderTargetBlendDesc aRenderTargetBlendDescriptions[GRAPHICS_MAX_BINDABLE_RENDER_TARGETS])
	//	{
	//#define LOG_CATEGORY CLASS_CATEGORY"::CreateBlendState"
	//
	//		GRAPHICS_ASSERT(outBlendState != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outBlendState) " cannot be nullptr!");
	//
	//		D3D11_BLEND_DESC desc = {};
	//		desc.AlphaToCoverageEnable = aEnableAlphaToCoverage;
	//		desc.IndependentBlendEnable = aEnableIndependentBlend;
	//
	//		for (size_t i = 0; i < GRAPHICS_MAX_BINDABLE_RENDER_TARGETS; i++)
	//		{
	//			desc.RenderTarget[i].BlendEnable = static_cast<BOOL>(aRenderTargetBlendDescriptions[i].EnableBlend);
	//			desc.RenderTarget[i].SrcBlend = dx11::GetD3D11Blend(aRenderTargetBlendDescriptions[i].SourceBlend);
	//			desc.RenderTarget[i].DestBlend = dx11::GetD3D11Blend(aRenderTargetBlendDescriptions[i].DestinationBlend);
	//			desc.RenderTarget[i].BlendOp = dx11::GetD3D11BlendOp(aRenderTargetBlendDescriptions[i].BlendOperation);
	//			desc.RenderTarget[i].SrcBlendAlpha = dx11::GetD3D11Blend(aRenderTargetBlendDescriptions[i].AlphaSourceBlend);
	//			desc.RenderTarget[i].DestBlendAlpha = dx11::GetD3D11Blend(aRenderTargetBlendDescriptions[i].AlphaDestinationBlend);
	//			desc.RenderTarget[i].BlendOpAlpha = dx11::GetD3D11BlendOp(aRenderTargetBlendDescriptions[i].AlphaBlendOperation);
	//			desc.RenderTarget[i].RenderTargetWriteMask = static_cast<UINT8>(dx11::GetD3D11ColorWriteEnable(aRenderTargetBlendDescriptions[i].RenderTargetWriteMask));
	//		}
	//
	//		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	//		HRESULT result = myDevice->CreateBlendState(&desc, &blendState);
	//		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateBlendState, LOG_CATEGORY);
	//		result;
	//
	//		*outBlendState = new BlendState_DX11Impl(_createGraphicsID(), blendState.Get());
	//		(**outBlendState).AddReference();
	//
	//		_setName(aName, *outBlendState);
	//
	//		return result::OK;
	//#undef LOG_CATEGORY
	//	}
	result::Code Device_DX11Impl::CreateBlendState(BlendState** outBlendState, const BlendStateDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateBlendState"
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");
		GRAPHICS_ASSERT(outBlendState != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outBlendState) " cannot be nullptr!");

		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = aDesc->EnableAlphaToCoverage;
		desc.IndependentBlendEnable = aDesc->EnableIndependentBlend;

		for (size_t i = 0; i < GRAPHICS_MAX_BINDABLE_RENDER_TARGETS; i++)
		{
			desc.RenderTarget[i].BlendEnable = static_cast<BOOL>(aDesc->RenderTargetBlendDescriptions[i].EnableBlend);
			desc.RenderTarget[i].SrcBlend = dx11::GetD3D11Blend(aDesc->RenderTargetBlendDescriptions[i].SourceBlend);
			desc.RenderTarget[i].DestBlend = dx11::GetD3D11Blend(aDesc->RenderTargetBlendDescriptions[i].DestinationBlend);
			desc.RenderTarget[i].BlendOp = dx11::GetD3D11BlendOp(aDesc->RenderTargetBlendDescriptions[i].BlendOperation);
			desc.RenderTarget[i].SrcBlendAlpha = dx11::GetD3D11Blend(aDesc->RenderTargetBlendDescriptions[i].AlphaSourceBlend);
			desc.RenderTarget[i].DestBlendAlpha = dx11::GetD3D11Blend(aDesc->RenderTargetBlendDescriptions[i].AlphaDestinationBlend);
			desc.RenderTarget[i].BlendOpAlpha = dx11::GetD3D11BlendOp(aDesc->RenderTargetBlendDescriptions[i].AlphaBlendOperation);
			desc.RenderTarget[i].RenderTargetWriteMask = static_cast<UINT8>(dx11::GetD3D11ColorWriteEnable(aDesc->RenderTargetBlendDescriptions[i].RenderTargetWriteMask));
		}

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		HRESULT result = myDevice->CreateBlendState(&desc, &blendState);
		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateBlendState, LOG_CATEGORY);
		result;

		if (*outBlendState != nullptr)
		{
			(**outBlendState).RemoveReference();
		}
		*outBlendState = new BlendState_DX11Impl(_createGraphicsID(), blendState.Get());
		(**outBlendState).AddReference();

		_setName(aDesc->Name.c_str(), *outBlendState);

		return result::OK;
#undef LOG_CATEGORY
		//		return CreateBlendState(
		//			outBlendState,
		//			aDesc->Name.c_str(),
		//			aDesc->EnableAlphaToCoverage,
		//			aDesc->EnableIndependentBlend,
		//			aDesc->RenderTargetBlendDescriptions
		//		);
		//#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateRasterizerState(RasterizerState** outRasterizerState, const RasterizerStateDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateRasterizerState"
		GRAPHICS_ASSERT(outRasterizerState != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outRasterizerState) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");

		D3D11_RASTERIZER_DESC desc = {};
		desc.FillMode = dx11::GetD3D11FillMode(aDesc->FillMode);
		desc.CullMode = dx11::GetD3D11CullMode(aDesc->FillMode);
		desc.FrontCounterClockwise = dx11::IsWindingOrderCounterClockwise(aDesc->WindingOrder);
		desc.DepthBias = aDesc->DepthBias;
		desc.SlopeScaledDepthBias = aDesc->SlopeScaledDepthBias;
		desc.DepthClipEnable = aDesc->EnableDepthClip;
		desc.ScissorEnable = aDesc->EnableScissor;
		desc.MultisampleEnable = aDesc->EnableMultisample;
		desc.AntialiasedLineEnable = aDesc->EnableAntialiasedLine;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		HRESULT result = myDevice->CreateRasterizerState(&desc, &rasterizerState);
		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateRasterizerState, LOG_CATEGORY);
		result;

		if (*outRasterizerState != nullptr)
		{
			(**outRasterizerState).RemoveReference();
		}
		*outRasterizerState = new RasterizerState_DX11Impl(_createGraphicsID(), rasterizerState.Get(), *aDesc);
		(**outRasterizerState).AddReference();

		_setName(aDesc->Name.c_str(), *outRasterizerState);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::CreateDepthStencilState(DepthStencilState** outDepthStencilState, const DepthStencilStateDesc* aDesc)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateDepthStencilState"
		GRAPHICS_ASSERT(outDepthStencilState != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outDepthStencilState) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDesc != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDesc)" cannot be nullptr!");

		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = aDesc->EnableDepth;
		desc.DepthWriteMask = dx11::GetD3D11DepthWriteMask(aDesc->DepthWriteMask);
		desc.DepthFunc = dx11::GetD3D11ComparisonFunction(aDesc->DepthComparison);
		desc.StencilEnable = aDesc->EnableStencil;
		desc.StencilReadMask = aDesc->StencilReadMask;
		desc.StencilWriteMask = aDesc->StencilWriteMask;

		desc.FrontFace.StencilFailOp = dx11::GetD3D11StencilOp(aDesc->FrontFace.FailOp);
		desc.FrontFace.StencilDepthFailOp = dx11::GetD3D11StencilOp(aDesc->FrontFace.DepthFailOp);
		desc.FrontFace.StencilPassOp = dx11::GetD3D11StencilOp(aDesc->FrontFace.PassOp);
		desc.FrontFace.StencilFunc = dx11::GetD3D11ComparisonFunction(aDesc->FrontFace.Comparison);

		desc.BackFace.StencilFailOp = dx11::GetD3D11StencilOp(aDesc->BackFace.FailOp);
		desc.BackFace.StencilDepthFailOp = dx11::GetD3D11StencilOp(aDesc->BackFace.DepthFailOp);
		desc.BackFace.StencilPassOp = dx11::GetD3D11StencilOp(aDesc->BackFace.PassOp);
		desc.BackFace.StencilFunc = dx11::GetD3D11ComparisonFunction(aDesc->BackFace.Comparison);

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		HRESULT result = myDevice->CreateDepthStencilState(&desc, &depthStencilState);
		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateDepthStencilState, LOG_CATEGORY);
		result;

		if (*outDepthStencilState != nullptr)
		{
			(**outDepthStencilState).RemoveReference();
		}
		*outDepthStencilState = new DepthStencilState_DX11Impl(_createGraphicsID(), depthStencilState.Get(), *aDesc);
		(**outDepthStencilState).AddReference();

		_setName(aDesc->Name.c_str(), *outDepthStencilState);

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::UpdateConstantBuffer(ConstantBuffer* aConstantBuffer, void* aData, size_t aDataSize)
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
	result::Code Device_DX11Impl::UpdateVertexBuffer(VertexBuffer* aVertexBuffer, void* aData, size_t aDataSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateVertexBuffer"

		GRAPHICS_ASSERT(aVertexBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aVertexBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aVertexBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aVertexBuffer) " (%u) was not from the same device (%u)!", GetID(), aVertexBuffer->GetID().Device);
		GRAPHICS_ASSERT(aData != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aData) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDataSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDataSize) " must be greater than 0!");
		GRAPHICS_ASSERT(aDataSize <= aVertexBuffer->GetSize(), LOG_CATEGORY, result::OUT_OF_RANGE, STRINGIFY(aDataSize) " was larger than the size of " STRINGIFY(aVertexBuffer) "!");

		ConstantBuffer_DX11Impl* buffer = dynamic_cast<ConstantBuffer_DX11Impl*>(aVertexBuffer);
		GRAPHICS_ASSERT(buffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aVertexBuffer) " could not be cast to " STRINGIFY(ConstantBuffer_DX11Impl*) "!");

		result::Code result = _updateBuffer(buffer->GetBuffer(), aData, aDataSize);
		if (GRAPHICS_FAILED(result))
		{
			return result;
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::UpdateIndexBuffer(IndexBuffer* aIndexBuffer, void* aData, size_t aDataSize)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateIndexBuffer"

		GRAPHICS_ASSERT(aIndexBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aIndexBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(IsDeviceObject(aIndexBuffer), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aIndexBuffer) " (%u) was not from the same device (%u)!", GetID(), aIndexBuffer->GetID().Device);
		GRAPHICS_ASSERT(aData != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aData) " cannot be nullptr!");
		GRAPHICS_ASSERT(aDataSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDataSize) " must be greater than 0!");
		GRAPHICS_ASSERT(aDataSize <= aIndexBuffer->GetSize(), LOG_CATEGORY, result::OUT_OF_RANGE, STRINGIFY(aDataSize) " was larger than the capacity of " STRINGIFY(aIndexBuffer) "!");

		ConstantBuffer_DX11Impl* buffer = dynamic_cast<ConstantBuffer_DX11Impl*>(aIndexBuffer);
		GRAPHICS_ASSERT(buffer != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aIndexBuffer) " could not be cast to " STRINGIFY(ConstantBuffer_DX11Impl*) "!");

		result::Code result = _updateBuffer(buffer->GetBuffer(), aData, aDataSize);
		if (GRAPHICS_FAILED(result))
		{
			return result;
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::BindVertexShader(VertexShader* aShader)
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
	result::Code Device_DX11Impl::BindHullShader(HullShader* aShader)
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
	result::Code Device_DX11Impl::BindDomainShader(DomainShader* aShader)
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
	result::Code Device_DX11Impl::BindGeometryShader(GeometryShader* aShader)
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
	result::Code Device_DX11Impl::BindPixelShader(PixelShader* aShader)
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
	result::Code Device_DX11Impl::BindComputeShader(ComputeShader* aShader)
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
	result::Code Device_DX11Impl::BindConstantBuffers(ConstantBuffer* const* aConstantBuffers, size_t aNumBuffers, unsigned int aStartSlot, GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags)
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
	result::Code Device_DX11Impl::BindVertexBuffers(VertexBuffer* const* aVertexBuffers, size_t aNumBuffers, unsigned int aStartSlot, const size_t* aStrides, const size_t* aOffsets)
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
	result::Code Device_DX11Impl::BindIndexBuffer(IndexBuffer* aIndexBuffer)
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
	result::Code Device_DX11Impl::BindShaderResources(ShaderResource* const* aShaderResources, size_t aNumShaderResources, unsigned int aStartSlot, GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags)
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
	result::Code Device_DX11Impl::BindTargets(RenderTarget* const* aRenderTargets, size_t aNumRenderTargets, DepthStencil* aDepthStencil)
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
	result::Code Device_DX11Impl::BindSamplerStates(SamplerState* const* aSamplerStates, size_t aNumSamplerStates, unsigned int aStartSlot, GRAPHICS_SHADER_STAGE_FLAGS aShaderFlags)
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
				SamplerState_DX11Impl* shaderResource = dynamic_cast<SamplerState_DX11Impl*>(aSamplerStates[i]);
				GRAPHICS_ASSERT(shaderResource != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, "A " STRINGIFY(SamplerState) " at " STRINGIFY(aSamplerStates) "[%u] failed to cast to " STRINGIFY(SamplerState_DX11Impl*) "!", i);
				GRAPHICS_ASSERT(IsDeviceObject(shaderResource), LOG_CATEGORY, result::INVALID_ARGUMENT, "A " STRINGIFY(ConstantBuffer) " (%u) at " STRINGIFY(aSamplerStates) "[%u] was not from the same device (%u)!", GetID(), i, shaderResource->GetID().Device);
				samplers[i] = shaderResource->GetSamplerState();
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
	result::Code Device_DX11Impl::CopyTexture(Texture2D* aSource, Texture2D* aDestination)
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
	result::Code Device_DX11Impl::Map(Texture2D* aTexture, unsigned int aSubResource, GRAPHICS_MAP aMapType, GRAPHICS_MAP_FLAGS aMapFlags, void** outData, unsigned int* outRowPitch, unsigned int* outDepthPitch)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Map"

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");

		ID3D11Texture2D* dxTexture = texture->GetTexture();
		GRAPHICS_ASSERT(dxTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " has no valid texture assigned.");

		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};

		HRESULT result = myDeviceContext->Map(
			dxTexture,
			aSubResource,
			dx11::GetD3D11Map(aMapType),
			dx11::GetD3D11MapFlag(aMapFlags),
			&mappedSubresource
		);

		if (outData != nullptr)
		{
			*outData = mappedSubresource.pData;
		}
		if (outRowPitch != nullptr)
		{
			*outRowPitch = mappedSubresource.RowPitch;
		}
		if (outDepthPitch != nullptr)
		{
			*outDepthPitch = mappedSubresource.DepthPitch;
		}

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11DeviceContext::Map, LOG_CATEGORY);
		result;

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::Unmap(Texture2D* aTexture, unsigned int aSubResource)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Unmap"

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");

		ID3D11Texture2D* dxTexture = texture->GetTexture();
		GRAPHICS_ASSERT(dxTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " has no valid texture assigned.");

		myDeviceContext->Unmap(
			dxTexture,
			aSubResource
		);

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::ResizeTexture(Texture2D* aTexture, unsigned int aWidth, unsigned int aHeight)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::ResizeTexture"
		GRAPHICS_ASSERT(aTexture != myBackbufferTexture.Get(), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " was the backbuffer texture! It cannot be resized.");

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(aTexture);
		GRAPHICS_ASSERT(texture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aTexture) " could not be cast to " STRINGIFY(Texture2D_DX11Impl) "!");

		Texture2DDesc desc = texture->GetDesc();
		if (desc.Width == aWidth && desc.Height == aHeight)
		{
			return result::OK;	// No need to resize if the texture already is that size.
		}
		desc.Width = aWidth;
		desc.Height = aHeight;

		graphics::PImpl<Texture2D> newTexture;
		result::Code result = CreateTexture2D(
			&newTexture,
			&desc
		);
		GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to resize Texture2D!");

		Texture2D_DX11Impl* newTextureImpl = dynamic_cast<Texture2D_DX11Impl*>(newTexture.Get());

		texture->SetTexture(newTextureImpl->GetTexture());

		return result::OK;
#undef LOG_CATEGORY	
	}
	result::Code Device_DX11Impl::ClearState()
	{
		myDeviceContext->ClearState();
		return result::OK;
	}
	result::Code Device_DX11Impl::GetBufferTexture(Texture2D** outTexture)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::GetBufferTexture"

		GRAPHICS_ASSERT(outTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outTexture) " cannot be nullptr!");

		//Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		//HRESULT result = mySwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), &texture);

		//GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::GetBuffer, LOG_CATEGORY);
		//result;

		//Texture2DDesc desc = {};

		//D3D11_TEXTURE2D_DESC dx11Desc = {};
		//texture->GetDesc(&dx11Desc);

		//desc.Name = "Backbuffer Texture";
		//desc.BindFlags = dx11::GetGraphicsTextureFlags(static_cast<D3D11_BIND_FLAG>(dx11Desc.BindFlags));
		//desc.ResourceFlags = dx11::GetGraphicsResourceFlags(static_cast<D3D11_RESOURCE_MISC_FLAG>(dx11Desc.MiscFlags));
		//desc.Width = dx11Desc.Width;
		//desc.Height = dx11Desc.Height;
		//desc.MipLevels = dx11Desc.MipLevels;
		//desc.NumTextures = dx11Desc.ArraySize;
		//desc.Data = nullptr;
		//desc.NumData = 0;
		//desc.Format = dx11::GetGraphicsFormat(dx11Desc.Format);
		//desc.Usage = dx11::GetGraphicsUsage(dx11Desc.Usage);
		//desc.CPUAccessFlags = dx11::GetGraphicsCPUAccessFlags(static_cast<D3D11_CPU_ACCESS_FLAG>(dx11Desc.CPUAccessFlags));

		//*outBuffer = new Texture2D_DX11Impl(_createGraphicsID(), texture.Get(), desc);
		//(**outBuffer).AddReference();
		if (*outTexture != nullptr)
		{
			(**outTexture).RemoveReference();
		}
		*outTexture = myBackbufferTexture.Get();
		(**outTexture).AddReference();

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::GetBufferRenderTarget(RenderTarget** outRenderTarget)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::GetBufferRenderTarget"
		GRAPHICS_ASSERT(outRenderTarget != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outRenderTarget) " cannot be nullptr!");

		if (*outRenderTarget != nullptr)
		{
			(**outRenderTarget).RemoveReference();
		}
		*outRenderTarget = myBackbufferRenderTarget.Get();
		(**outRenderTarget).AddReference();

		return result::OK;

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::GetBufferDepthStencil(DepthStencil** outDepthStencil)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::GetBufferDepthStencil"

		GRAPHICS_ASSERT(outDepthStencil != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outDepthStencil) " cannot be nullptr!");

		if (*outDepthStencil != nullptr)
		{
			(**outDepthStencil).RemoveReference();
		}
		*outDepthStencil = myBackbufferDepthStencil.Get();
		(**outDepthStencil).AddReference();

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::SetTopology(GRAPHICS_TOPOLOGY aTopology)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetTopology"

		myDeviceContext->IASetPrimitiveTopology(dx11::GetD3D11PrimitiveTopology(aTopology));

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::SetRasterizerState(RasterizerState* aRasterizerState)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SetTopology"

		//GRAPHICS_ASSERT(aRasterizerState != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aRasterizerState) " cannot be nullptr!");
		GRAPHICS_ASSERT(aRasterizerState == nullptr || IsDeviceObject(aRasterizerState), LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aInputLayout) " (%u) was not from the same device (%u)!", GetID(), aRasterizerState->GetID().Device);

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
	result::Code Device_DX11Impl::SetDepthStencilState(DepthStencilState* aDepthStencilState, unsigned int aStencilReference)
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
	result::Code Device_DX11Impl::SetInputLayout(InputLayout* aInputLayout)
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
	result::Code Device_DX11Impl::SetBlendState(BlendState* aBlendState, float aBlendFactors[4], unsigned int aSampleMask)
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
	result::Code Device_DX11Impl::Draw(unsigned int aVertexCount, unsigned int aWhere)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Draw"

		myDeviceContext->Draw(static_cast<UINT>(aVertexCount), static_cast<UINT>(aWhere));

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::DrawIndexed(unsigned int aCount, unsigned int aWhere, unsigned int aVertexWhere)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::DrawIndexed"

		myDeviceContext->DrawIndexed(static_cast<UINT>(aCount), static_cast<UINT>(aWhere), static_cast<UINT>(aVertexWhere));

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::Present(GRAPHICS_PRESENT_FLAGS aFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Present"

		UINT presentationFlags = NULL;

		if (aFlags & GRAPHICS_PRESENT_ALLOW_TEARING)
		{
			GRAPHICS_ASSERT((mySwapChainFlags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING) != 0, LOG_CATEGORY, result::INVALID_STATE, STRINGIFY(aFlags) " contained " STRINGIFY(GRAPHICS_PRESENT_ALLOW_TEARING) ", but " STRINGIFY(mySwapChainFlags) " does not have flag " STRINGIFY(DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING) ".");
			presentationFlags |= DXGI_PRESENT_ALLOW_TEARING;
		}

		HRESULT result = mySwapChain->Present(0, presentationFlags);

		if (result == DXGI_ERROR_DEVICE_REMOVED)
		{
			result = myDevice->GetDeviceRemovedReason();
		}

		if (result != DXGI_STATUS_OCCLUDED)
		{
			GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::Present, LOG_CATEGORY);
		}

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::ClearRenderTarget(RenderTarget* aRenderTarget, const float aColor[4])
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
	result::Code Device_DX11Impl::ClearDepthStencil(DepthStencil* aDepthStencil, float aDepth, unsigned char aStencil)
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
	result::Code Device_DX11Impl::SetViewports(Viewport* aViewports, size_t aNumViewports)
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
	result::Code Device_DX11Impl::UpdateBackbufferSize(unsigned int aWidth, unsigned int aHeight)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::UpdateBackbufferSize"

		Texture2D_DX11Impl* backbufferTexture = static_cast<Texture2D_DX11Impl*>(myBackbufferTexture.Get());
		Texture2D_DX11Impl* backbufferDepthTexture = static_cast<Texture2D_DX11Impl*>(myBackbufferDepthTexture.Get());
		RenderTarget_DX11Impl* backbufferRenderTarget = static_cast<RenderTarget_DX11Impl*>(myBackbufferRenderTarget.Get());

		//ULONG count = backbufferTexture->GetTexture()->Release();
		//count;
		
		backbufferTexture->SetTexture(nullptr);
		backbufferRenderTarget->SetNull();	// Releases captures texture.

		myDeviceContext->ClearState();
		HRESULT result = mySwapChain->ResizeBuffers(0, static_cast<UINT>(aWidth), static_cast<UINT>(aHeight), DXGI_FORMAT_UNKNOWN, mySwapChainFlags);
		GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::ResizeBuffers, LOG_CATEGORY);

		//Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		//result = mySwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), &texture);
		//GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::ResizeBuffers, LOG_CATEGORY);

		PImpl<Texture2D> texture;
		_createBackbufferTexture(&texture);
		backbufferTexture->SetTexture(static_cast<Texture2D_DX11Impl*>(texture.Get())->GetTexture());

		PImpl<Texture2D> depthTexture;
		_createBackbufferDepthTexture(&depthTexture);
		backbufferDepthTexture->SetTexture(static_cast<Texture2D_DX11Impl*>(depthTexture.Get())->GetTexture());

		backbufferRenderTarget->Clean();

		return result::OK;
#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::PushEvent(const char* aName, int* outNumEvents)
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
	result::Code Device_DX11Impl::PopEvent(int* outNumEvents)
	{
		INT numEvents = myUserDefinedAnnotation->EndEvent();

		if (outNumEvents)
		{
			*outNumEvents = static_cast<int>(numEvents);
		}
		return result::OK;
	}
	result::Code Device_DX11Impl::_createBackbufferTexture(Texture2D** outTexture)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::_createBackbufferTexture"
		GRAPHICS_ASSERT(outTexture != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outTexture) " cannot be nullptr!");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		HRESULT result = mySwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), &texture);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, IDXGISwapChain::GetBuffer, LOG_CATEGORY);
		result;

		Texture2DDesc desc = {};

		D3D11_TEXTURE2D_DESC dx11Desc = {};
		texture->GetDesc(&dx11Desc);

		desc.Name = "Backbuffer Texture";
		desc.BindFlags = dx11::GetGraphicsTextureFlags(static_cast<D3D11_BIND_FLAG>(dx11Desc.BindFlags));
		desc.ResourceFlags = dx11::GetGraphicsResourceFlags(static_cast<D3D11_RESOURCE_MISC_FLAG>(dx11Desc.MiscFlags));
		desc.Width = dx11Desc.Width;
		desc.Height = dx11Desc.Height;
		desc.MipLevels = dx11Desc.MipLevels;
		desc.NumTextures = dx11Desc.ArraySize;
		desc.Data = nullptr;
		desc.NumData = 0;
		desc.Format = dx11::GetGraphicsFormat(dx11Desc.Format);
		desc.Usage = dx11::GetGraphicsUsage(dx11Desc.Usage);
		desc.CPUAccessFlags = dx11::GetGraphicsCPUAccessFlags(static_cast<D3D11_CPU_ACCESS_FLAG>(dx11Desc.CPUAccessFlags));

		if (*outTexture != nullptr)
		{
			(**outTexture).RemoveReference();
		}
		*outTexture = new Texture2D_DX11Impl(_createGraphicsID(), texture.Get(), desc);
		(**outTexture).AddReference();

		return result::OK;

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::_createBackbufferDepthTexture(Texture2D** outTexture)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::_createBackbufferDepthTexture"

		GRAPHICS_ASSERT(myBackbufferTexture != nullptr, LOG_CATEGORY, result::INVALID_STATE, "Failed to create Backbuffer Depth Texture as Backbuffer Texture was nullptr!");

		const Texture2DDesc& backbufferDesc = myBackbufferTexture->GetDesc();

		const Texture2DDesc desc = {
			.BindFlags = GRAPHICS_BIND_SHADER_RESOURCE | GRAPHICS_BIND_DEPTH_STENCIL,
			.ResourceFlags = GRAPHICS_RESOURCE_NONE,
			.Name = "Backbuffer Depth Texture",
			.Width = backbufferDesc.Width,
			.Height = backbufferDesc.Height,
			.MipLevels = 1,
			.NumTextures = 1,
			.Data = nullptr,
			.NumData = 0,
			.Format = GRAPHICS_FORMAT_R32_TYPELESS,
			.Usage = GRAPHICS_USAGE_DEFAULT,
			.CPUAccessFlags = GRAPHICS_CPU_ACCESS_NONE
		};

		return CreateTexture2D(outTexture, &desc);

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::_createBackbufferRenderTarget(RenderTarget** outRenderTarget)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::_createBackbufferTexture"

		graphics::RenderTargetDesc desc = {
			.Name = "Backbuffer RenderTarget",
			.Texture = myBackbufferTexture.Get(),
			.Format = graphics::GRAPHICS_FORMAT_UNKNOWN,
			.ViewDimension = graphics::GRAPHICS_RENDER_TARGET_VIEW_DIMENSION_TEXTURE2D,
			.FirstSlice = 0,
			.NumSlices = 1,
			.MipSlice = 0
		};

		D3D11_RENDER_TARGET_VIEW_DESC dx11desc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_RTV {
				.MipSlice = 0
			}
		};

		Texture2D_DX11Impl* texture = dynamic_cast<Texture2D_DX11Impl*>(myBackbufferTexture.Get());

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
		HRESULT result = myDevice->CreateRenderTargetView(texture->GetTexture(), &dx11desc, &rtv);
		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateRenderTargetView, LOG_CATEGORY);
		result;

		if (*outRenderTarget != nullptr)
		{
			(**outRenderTarget).RemoveReference();
		}
		*outRenderTarget = new RenderTarget_DX11Impl(this, _createGraphicsID(), rtv.Get(), desc);
		(**outRenderTarget).AddReference();

		_setName(desc.Name.c_str(), *outRenderTarget);
		
		return result::OK;

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::_createBackbufferDepthStencil(DepthStencil** outDepthStencil)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::_createBackbufferDepthStencil"

		GRAPHICS_ASSERT(myBackbufferDepthTexture != nullptr, LOG_CATEGORY, result::INVALID_STATE, "Failed to create Backbuffer DepthStencil as Backbuffer Depth Texture was nullptr!");

		graphics::DepthStencilDesc desc = {
			.Name = "Backbuffer DepthStencil",
			.Texture = myBackbufferDepthTexture.Get(),
			.Format = graphics::GRAPHICS_FORMAT_D32_FLOAT,
			.ViewDimension = graphics::GRAPHICS_RENDER_TARGET_VIEW_DIMENSION_TEXTURE2D,
			.FirstSlice = 0,
			.NumSlices = 1,
			.MipSlice = 0
		};

		return CreateDepthStencil(outDepthStencil, &desc);

#undef LOG_CATEGORY
	}
	result::Code Device_DX11Impl::_createBuffer(ID3D11Buffer** outBuffer, UINT aBindFlags, const void* aData, size_t aSize, GRAPHICS_USAGE aUsage, GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::_createBuffer"

		GRAPHICS_ASSERT(outBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outBuffer) " cannot be nullptr!");
		GRAPHICS_ASSERT(aSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aSize) " must be greater than 0!");

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = static_cast<UINT>(aSize);
		desc.Usage = dx11::GetD3D11Usage(aUsage);
		desc.BindFlags = aBindFlags;
		desc.CPUAccessFlags = dx11::GetD3D11CPUAccessFlag(aCPUAccessFlags);
		desc.MiscFlags = NULL;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA* dataPtr = NULL;

		if (aData != nullptr)
		{
			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = aData;
			data.SysMemPitch = static_cast<UINT>(aSize);
			data.SysMemSlicePitch = static_cast<UINT>(aSize);

			dataPtr = &data;
		}

		HRESULT result = myDevice->CreateBuffer(&desc, dataPtr, outBuffer);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, ID3D11Device::CreateBuffer, LOG_CATEGORY);
		result;

		return result::OK;
#undef LOG_CATEGORY		
	}
	result::Code Device_DX11Impl::_updateBuffer(ID3D11Buffer* aBuffer, const void* aData, size_t aDataSize)
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
	void Device_DX11Impl::_setName(const char* aName, GraphicsObject* aGraphicsObject)
	{
		if (aName && aGraphicsObject)
		{
			aGraphicsObject->SetName(aName);
		}
	}
}