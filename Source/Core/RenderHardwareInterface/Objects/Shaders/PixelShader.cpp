#include "RenderHardwareInterface.pch.h"
#include "PixelShader.h"

graphics::PixelShader::PixelShader(GraphicsID aID) : 
	Shader(aID, GRAPHICS_SHADER_TYPE_PIXEL)
{
}
