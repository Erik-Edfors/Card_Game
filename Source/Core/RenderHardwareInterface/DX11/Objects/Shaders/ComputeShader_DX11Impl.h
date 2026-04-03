#pragma once
#include "Objects/Shaders/ComputeShader.h"
#include <wrl.h>

struct ID3D11ComputeShader;

namespace graphics
{
	class ComputeShader_DX11Impl : public ComputeShader
	{
	public:
		ComputeShader_DX11Impl(GraphicsID aID, ID3D11ComputeShader* aComputeShader);

		void SetName(const char* aName) override;

		ID3D11ComputeShader* GetComputeShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> myComputeShader;
	};
}