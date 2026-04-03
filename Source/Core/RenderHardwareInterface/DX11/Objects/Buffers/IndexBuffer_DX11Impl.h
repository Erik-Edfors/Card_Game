#pragma once
#include "Objects/Buffers/IndexBuffer.h"
#include <wrl.h>

struct ID3D11Buffer;

namespace graphics
{
	class IndexBuffer_DX11Impl : public IndexBuffer
	{
	public:
		IndexBuffer_DX11Impl(GraphicsID aID, size_t aSize, ID3D11Buffer* aBuffer);
		~IndexBuffer_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11Buffer* GetBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
	};
}