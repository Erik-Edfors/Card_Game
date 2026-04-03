#pragma once
#include "Objects/BlendState.h"
#include <wrl.h>

struct ID3D11BlendState;
namespace graphics
{
	class BlendState_DX11Impl : public BlendState
	{
	public:
		BlendState_DX11Impl(GraphicsID aID, ID3D11BlendState* aBlendState);
		~BlendState_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11BlendState* GetBlendState();

	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> myBlendState;
	};
}