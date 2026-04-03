#pragma once
#include "Objects/DepthStencil.h"
#include <wrl.h>

struct ID3D11DepthStencilView;

namespace graphics
{
	class Device;

	class DepthStencil_DX11Impl : public DepthStencil
	{
	public:
		DepthStencil_DX11Impl(Device* aDevice, GraphicsID aID, ID3D11DepthStencilView* aDepthStencilView, const DepthStencilDesc& aDesc);
		~DepthStencil_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11DepthStencilView* GetDepthStencilView();

	private:
		PImpl<Device> myDevice;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> myDepthStencilView;
		unsigned int myLastTextureVersion;
	};
}