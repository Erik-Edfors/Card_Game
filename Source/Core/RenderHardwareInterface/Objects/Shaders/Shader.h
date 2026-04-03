#pragma once
#include "../GraphicsObject.h"

namespace graphics
{
	class Shader : public GraphicsObject
	{
	public:
		GRAPHICS_SHADER_TYPE GetType() const;
	protected:
		Shader(GraphicsID aID, GRAPHICS_SHADER_TYPE aShaderType);
	private:
		GRAPHICS_SHADER_TYPE myType;
	};
}