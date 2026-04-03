#pragma once
#include "Objects/Shaders/PixelShader.h"
#include <wrl.h>

struct ID3D11PixelShader;

namespace graphics
{
	class PixelShader_DX11Impl : public PixelShader
	{
	public:
		PixelShader_DX11Impl(GraphicsID aID, ID3D11PixelShader* aPixelShader);

		void SetName(const char* aName) override;

		ID3D11PixelShader* GetPixelShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> myPixelShader;
	};
}