#pragma once
#include "../../Objects/ShaderResource.h"
#include <wrl.h>

struct ID3D11ShaderResourceView;

namespace graphics
{
	class Device;

	class ShaderResource_DX11Impl : public ShaderResource
	{
	public:
		ShaderResource_DX11Impl(Device* aDevice, GraphicsID aID, ID3D11ShaderResourceView* aShaderResourceView, const ShaderResourceDesc& aDesc);
		~ShaderResource_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11ShaderResourceView* GetShaderResourceView();

	private:
		PImpl<Device> myDevice;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myShaderResourceView;
		unsigned int myLastTextureVersion;
	};
}