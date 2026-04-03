#pragma once
#include "Objects/Texture2DArray.h"
#include <wrl.h>

struct ID3D11Texture2D;

namespace graphics
{
	class Texture2DArray_DX11Impl : public Texture2DArray
	{
	public:
		Texture2DArray_DX11Impl(GraphicsID aID, ID3D11Texture2D* aTexture, GRAPHICS_BIND_FLAGS aFlags, unsigned int aSize);
		~Texture2DArray_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11Texture2D* GetTexture();

		result::Code GetSize(
			unsigned int* outWidth,
			unsigned int* outHeight
		) const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> myTexture;
	};
}