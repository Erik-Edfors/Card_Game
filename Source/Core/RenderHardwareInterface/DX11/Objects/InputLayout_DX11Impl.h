#pragma once
#include "Objects\InputLayout.h"
#include <wrl.h>

struct ID3D11InputLayout;

namespace graphics
{
	class InputLayout_DX11Impl : public InputLayout
	{
	public:
		InputLayout_DX11Impl(GraphicsID aID, const LayoutDefinition& aLayoutDefinition, ID3D11InputLayout* aInputLayout);
		~InputLayout_DX11Impl();

		void SetName(const char* aName) override;

		ID3D11InputLayout* GetInputLayout();
		const ID3D11InputLayout* GetInputLayout() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> myInputLayout;
	};
}