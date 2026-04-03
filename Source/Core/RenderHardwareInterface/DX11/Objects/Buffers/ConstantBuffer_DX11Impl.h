#pragma once
#include "Objects/Buffers/ConstantBuffer.h"
#include <wrl.h>

struct ID3D11Buffer;

namespace graphics
{
	class ConstantBuffer_DX11Impl : public ConstantBuffer
	{
	public:
		ConstantBuffer_DX11Impl(GraphicsID aID, size_t aSize, ID3D11Buffer* aBuffer);
		~ConstantBuffer_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11Buffer* GetBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> myBuffer;
	};
}