#pragma once
#include "Shader.h"

namespace graphics
{
	struct GeometryShaderDesc
	{

	};

	class GeometryShader : public Shader
	{
	protected:
		GeometryShader(GraphicsID aID);
	};
}