#pragma once
#include "Objects/Buffers/VertexBuffer.h"
#include <wrl.h>

struct ID3D11Buffer;

namespace graphics
{
	class VertexBuffer_DX11Impl : public VertexBuffer
	{
	public:
		VertexBuffer_DX11Impl(GraphicsID aID, size_t aSize, ID3D11Buffer* aBuffer);
		~VertexBuffer_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11Buffer* GetBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
	};
}