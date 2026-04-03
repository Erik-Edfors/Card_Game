#include "RenderHardwareInterface.pch.h"
#include "HullShader.h"

graphics::HullShader::HullShader(GraphicsID aID) : 
	Shader(aID, GRAPHICS_SHADER_TYPE_HULL)
{
}
