#pragma once
#include "Shader.h"

namespace graphics
{
	struct DomainShaderDesc
	{

	};

	class DomainShader : public Shader
	{
	protected:
		DomainShader(GraphicsID aID);
	};
}