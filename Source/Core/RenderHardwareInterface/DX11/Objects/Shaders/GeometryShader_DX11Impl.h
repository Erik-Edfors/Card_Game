#pragma once
#include "Objects/Shaders/GeometryShader.h"
#include <wrl.h>

struct ID3D11GeometryShader;

namespace graphics
{
	class GeometryShader_DX11Impl : public GeometryShader
	{
	public:
		GeometryShader_DX11Impl(GraphicsID aID, ID3D11GeometryShader* aGeometryShader);

		void SetName(const char* aName) override;

		ID3D11GeometryShader* GetGeometryShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> myGeometryShader;
	};
}