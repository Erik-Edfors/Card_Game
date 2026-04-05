#pragma once
#include "../../Objects\SamplerState.h"
#include <wrl.h>

struct ID3D11SamplerState;

namespace graphics
{
	class SamplerState_DX11Impl : public SamplerState
	{
	public:
		SamplerState_DX11Impl(GraphicsID aID, ID3D11SamplerState* aSamplerState);

		void SetName(const char* aName) override;

		ID3D11SamplerState* GetSamplerState();
		const ID3D11SamplerState* GetSamplerState() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mySamplerState;
	};
}