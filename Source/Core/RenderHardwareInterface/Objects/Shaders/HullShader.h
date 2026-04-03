#pragma once
#include "Shader.h"

namespace graphics
{
	struct HullShaderDesc
	{

	};

	class HullShader : public Shader
	{
	protected:
		HullShader(GraphicsID aID);
	};
}