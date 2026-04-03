#pragma once
#include "Shader.h"

namespace graphics
{
	struct VertexShaderDesc
	{

	};

	class VertexShader : public Shader
	{
	protected:
		VertexShader(GraphicsID aID);
	};
}