#pragma once
#include "Objects/DepthStencilState.h"
#include <wrl.h>

struct ID3D11DepthStencilState;

namespace graphics
{
	class Device;

	class DepthStencilState_DX11Impl : public DepthStencilState
	{
	public:
		DepthStencilState_DX11Impl(GraphicsID aID, ID3D11DepthStencilState* aDepthStencilView, const DepthStencilStateDesc& aDesc);
		~DepthStencilState_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11DepthStencilState* GetDepthStencilState();
		const ID3D11DepthStencilState* GetDepthStencilState() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> myDepthStencilState;
	};
}