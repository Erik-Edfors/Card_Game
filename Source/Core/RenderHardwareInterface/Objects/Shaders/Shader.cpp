#include "RenderHardwareInterface.pch.h"
#include "Shader.h"

graphics::GRAPHICS_SHADER_TYPE graphics::Shader::GetType() const
{
	return myType;
}

graphics::Shader::Shader(GraphicsID aID, GRAPHICS_SHADER_TYPE aShaderType) :
	myType(aShaderType),
	GraphicsObject(aID)
{
}
