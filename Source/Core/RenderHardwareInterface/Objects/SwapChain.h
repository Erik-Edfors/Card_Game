#pragma once
#include "GraphicsObject.h"

namespace graphics
{
	struct SwapChainDesc
	{

	};

	class SwapChain : public GraphicsObject
	{
	public:
		virtual ~SwapChain();

		const SwapChainDesc& GetDesc() const noexcept;

	protected:
		SwapChain(GraphicsID aID, const SwapChainDesc& aDesc);

		void SetDesc(const SwapChainDesc& aDesc) noexcept;

	private:
		SwapChainDesc myDesc;
	};
}