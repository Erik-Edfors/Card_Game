#include "RenderHardwareInterface.pch.h"

#include "RenderHardwareInterface.h"
#include "GAssert_DX11.h"
#include "DX11/Device_DX11Impl.h"
#include "DX11/Window_DX11Impl.h"

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "RenderHardwareInterface_DX11Impl.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->RenderHardwareInterface"

namespace graphics
{
	result::Code CreateDevice(Device** outDevice, Window* aWindow, GRAPHICS_DEVICE_FLAGS aFlags)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateDevice"

		GRAPHICS_ASSERT(outDevice != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outDevice) " was nullptr!");
		GRAPHICS_ASSERT(aWindow != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aWindow) " was nullptr!");

		Window_DX11Impl* viewport = dynamic_cast<Window_DX11Impl*>(aWindow);
		GRAPHICS_ASSERT(viewport != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aWindow) " could not be cast to " STRINGIFY(Window_DX11Impl*) "!");

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

		UINT flags = NULL;
		UINT swapChainFlags = NULL;

		if (aFlags & GRAPHICS_DEVICE_DEBUG)
		{
			flags |= D3D11_CREATE_DEVICE_DEBUG;
			//flags |= D3D11_CREATE_DEVICE_DEBUGGABLE;
		}
		if (aFlags & GRAPHICS_DEVICE_BGRA_SUPPORT)
		{
			flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		}
		if (aFlags & GRAPHICS_DEVICE_VIDEO_SUPPORT)
		{
			flags |= D3D11_CREATE_DEVICE_VIDEO_SUPPORT;
		}
		if (aFlags & GRAPHICS_DEVICE_ALLOW_TEARING)
		{
			swapChainFlags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		}

		DXGI_SWAP_CHAIN_DESC swDesc = {};
		swDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swDesc.SampleDesc.Count = 1;
		swDesc.SampleDesc.Quality = 0;
		swDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swDesc.BufferCount = 2;
		swDesc.OutputWindow = viewport->GetWindowHandle();
		swDesc.Windowed = true;
		swDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swDesc.Flags = swapChainFlags;

		HRESULT result = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			flags,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&swDesc,
			&swapChain,
			&device,
			NULL,
			&deviceContext
		);

		GRAPHICS_ASSERT_WIN32_API_CALL(result, D3D11CreateDeviceAndSwapChain, LOG_CATEGORY);
		result;

		*outDevice = new Device_DX11Impl(viewport, device.Get(), deviceContext.Get(), swapChain.Get(), swDesc.Flags);
		(**outDevice).AddReference();

		return result::OK;

#undef LOG_CATEGORY
	}
}