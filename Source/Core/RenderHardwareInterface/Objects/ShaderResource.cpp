#include "RenderHardwareInterface.pch.h"
#include "ShaderResource.h"
#include "Texture2D.h"

graphics::ShaderResource::ShaderResource(GraphicsID aID, const ShaderResourceDesc& aDesc) :
	GraphicsObject(aID),
	myTexture(aDesc.Texture),
	myDesc(aDesc)
{
}

graphics::ShaderResource::~ShaderResource() = default;

graphics::Texture2D* graphics::ShaderResource::GetTexture()
{
	return myTexture.Get();
}

const graphics::ShaderResourceDesc& graphics::ShaderResource::GetDesc() const
{
	return myDesc;
}