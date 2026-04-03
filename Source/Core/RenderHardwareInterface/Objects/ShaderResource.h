#pragma once
#include "GraphicsObject.h"
#include "../PImpl.hpp"

namespace graphics
{
	class Texture2D;

	struct ShaderResourceDesc
	{
		std::string Name;													//A name to assign the object.
		Texture2D* Texture;													//A texture array to use as shader resource. Must have GRAPHICS_BIND_SHADER_RESOURCE flag.
		GRAPHICS_FORMAT Format;												//A format to interpret the texture with.
		GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION ViewDimension;				//The dimensionality of the view.
		unsigned int FirstSlice;											//The first slice for the shader resource to target.
		unsigned int NumSlices;												//The number of slices for the shader resource to target.
		int MipLevels;														//The amount of mip levels to use.
		unsigned int MostDetailedMip;										//The index of most detailed mip level to use.
	};

	class ShaderResource : public GraphicsObject
	{
	public:
		~ShaderResource();

		Texture2D* GetTexture();
		const ShaderResourceDesc& GetDesc() const;

	protected:
		ShaderResource(GraphicsID aID, const ShaderResourceDesc& aDesc);

	private:
		PImpl<Texture2D> myTexture;
		ShaderResourceDesc myDesc;
	};
}