#include "RenderHardwareInterface.pch.h"
#include "ComputeShader.h"

graphics::ComputeShader::ComputeShader(GraphicsID aID) : 
	Shader(aID, GRAPHICS_SHADER_TYPE_COMPUTE)
{
}
