#pragma once
#include "Objects/RasterizerState.h"
#include <wrl.h>

struct ID3D11RasterizerState;

namespace graphics
{
	class RasterizerState_DX11Impl : public RasterizerState
	{
	public:
		RasterizerState_DX11Impl(GraphicsID aID, ID3D11RasterizerState* aRasterizerState, const RasterizerStateDesc& aDesc);
		~RasterizerState_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11RasterizerState* GetRasterizerState();
		const ID3D11RasterizerState* GetRasterizerState() const;

	private:

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> myRasterizerState;
	};
}