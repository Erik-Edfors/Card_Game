#include "RenderHardwareInterface.pch.h"
#include "Data_DX11Impl.h"

graphics::Data_DX11Impl::Data_DX11Impl(GraphicsID aID, const DataDesc& aDesc) : 
	Data(aID, aDesc)
{
}

void graphics::Data_DX11Impl::SetName(const char* aName)
{
	myDesc.Name = aName;
	SetLocalName(aName);
}
