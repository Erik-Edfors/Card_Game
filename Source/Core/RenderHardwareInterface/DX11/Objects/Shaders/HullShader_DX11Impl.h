#pragma once
#include "Objects/Shaders/HullShader.h"
#include <wrl.h>

struct ID3D11HullShader;

namespace graphics
{
	class HullShader_DX11Impl : public HullShader
	{
	public:
		HullShader_DX11Impl(GraphicsID aID, ID3D11HullShader* aHullShader);

		void SetName(const char* aName) override;

		ID3D11HullShader* GetHullShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11HullShader> myHullShader;
	};
}