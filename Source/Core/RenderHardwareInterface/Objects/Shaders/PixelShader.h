#pragma once
#include "Shader.h"

namespace graphics
{
	struct PixelShaderDesc
	{

	};

	class PixelShader : public Shader
	{
	protected:
		PixelShader(GraphicsID aID);
	};
}