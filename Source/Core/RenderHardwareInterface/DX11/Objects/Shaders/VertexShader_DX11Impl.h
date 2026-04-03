#pragma once
#include "Objects/Shaders/VertexShader.h"
#include <wrl.h>

struct ID3D11VertexShader;

namespace graphics
{
	class VertexShader_DX11Impl : public VertexShader
	{
	public:
		VertexShader_DX11Impl(GraphicsID aID, ID3D11VertexShader* aVertexShader);

		void SetName(const char* aName) override;

		ID3D11VertexShader* GetVertexShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> myVertexShader;
	};
}