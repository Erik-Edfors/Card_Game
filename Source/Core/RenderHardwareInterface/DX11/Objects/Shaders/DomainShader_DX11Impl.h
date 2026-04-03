#pragma once
#include "Objects/Shaders/DomainShader.h"
#include <wrl.h>

struct ID3D11DomainShader;

namespace graphics
{
	class DomainShader_DX11Impl : public DomainShader
	{
	public:
		DomainShader_DX11Impl(GraphicsID aID, ID3D11DomainShader* aDomainShader);

		void SetName(const char* aName) override;

		ID3D11DomainShader* GetDomainShader();

	private:
		Microsoft::WRL::ComPtr<ID3D11DomainShader> myDomainShader;
	};
}