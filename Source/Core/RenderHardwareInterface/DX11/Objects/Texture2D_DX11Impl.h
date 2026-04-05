#pragma once
#include "../../Objects/Texture2D.h"
#include <wrl.h>

struct ID3D11Texture2D;

namespace graphics
{
	class Texture2D_DX11Impl : public Texture2D
	{
	public:
		Texture2D_DX11Impl(GraphicsID aID, ID3D11Texture2D* aTexture, const Texture2DDesc& aDesc);
		~Texture2D_DX11Impl();

		void SetName(const char* aName) override;

		void SetTexture(ID3D11Texture2D* aTexture);
		ID3D11Texture2D* GetTexture();

	private:
		unsigned int _incrementVersion();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> myTexture;
	};
}