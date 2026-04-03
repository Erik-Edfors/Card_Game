#pragma once
#include "Shader.h"

namespace graphics
{
	struct ComputeShaderDesc
	{

	};

	class ComputeShader : public Shader
	{
	protected:
		ComputeShader(GraphicsID aID);
	};
}