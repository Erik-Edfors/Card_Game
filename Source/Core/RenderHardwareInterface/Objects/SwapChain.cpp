#include "RenderHardwareInterface.pch.h"
#include "SwapChain.h"

namespace graphics
{
	SwapChain::SwapChain(GraphicsID aID, const SwapChainDesc& aDesc) : 
		GraphicsObject(aID),
		myDesc(aDesc)
	{
	}
	SwapChain::~SwapChain()
	{
	}
	const SwapChainDesc& SwapChain::GetDesc() const noexcept
	{
		return myDesc;
	}
	void SwapChain::SetDesc(const SwapChainDesc& aDesc) noexcept
	{
		myDesc = aDesc;
	}
}
