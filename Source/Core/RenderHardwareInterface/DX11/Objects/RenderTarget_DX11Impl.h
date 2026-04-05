#pragma once
#include "../../Objects/RenderTarget.h"
#include <wrl.h>

struct ID3D11RenderTargetView;

namespace graphics
{
	class Device;

	class RenderTarget_DX11Impl : public RenderTarget
	{
	public:
		RenderTarget_DX11Impl(Device* aDevice, GraphicsID aID, ID3D11RenderTargetView* aRenderTargetView, const RenderTargetDesc& aDesc);
		~RenderTarget_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11RenderTargetView* GetRenderTargetView();

	private:
		PImpl<Device> myDevice;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myRenderTargetView;
		unsigned int myLastTextureVersion;
	};
}