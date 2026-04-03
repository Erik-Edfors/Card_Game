#include "RenderHardwareInterface.pch.h"
#include "Texture2D.h"
#include "RenderTarget.h"

const graphics::Texture2DDesc& graphics::Texture2D::GetDesc() const
{
	return myDesc;
}

unsigned int graphics::Texture2D::GetVersion() const
{
	return myVersion;
}

void graphics::Texture2D::SetName(const char* aName)
{
	SetLocalName(aName);
	myDesc.Name = aName;
}

graphics::Texture2D::Texture2D(GraphicsID aID, const graphics::Texture2DDesc& aDesc) :
	myDesc(aDesc),
	myVersion(0u),
	GraphicsObject(aID)
{
	myDesc.Data = nullptr;
	myDesc.NumData = 0u;
}

void graphics::Texture2D::SetVersion(unsigned int aVersion)
{
	myVersion = aVersion;
}

void graphics::Texture2D::SetDesc(const graphics::Texture2DDesc& aDesc)
{
	myDesc = aDesc;
}
