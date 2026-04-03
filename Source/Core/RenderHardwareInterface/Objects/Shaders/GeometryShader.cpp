#include "RenderHardwareInterface.pch.h"
#include "GeometryShader.h"

graphics::GeometryShader::GeometryShader(GraphicsID aID) : 
	Shader(aID, GRAPHICS_SHADER_TYPE_GEOMETRY)
{
}
