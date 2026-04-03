#pragma once
#include "GraphicsObject.h"
#include "../PImpl.hpp"

namespace graphics
{
	class Texture2D;

	struct DepthStencilDesc
	{
		GRAPHICS_DEPTH_STENCIL_FLAGS Flags;
		std::string Name;
		Texture2D* Texture;
		GRAPHICS_FORMAT Format;
		GRAPHICS_DEPTH_STENCIL_VIEW_DIMENSION ViewDimension;				//The dimensionality of the view.
		unsigned int FirstSlice;											//The first slice for the DepthStencil to target.
		unsigned int NumSlices;												//The number of slices for the DepthStencil to target.
		unsigned int MipSlice;
	};

	class DepthStencil : public GraphicsObject
	{
	public:
		~DepthStencil();

		Texture2D* GetTexture();
		const DepthStencilDesc& GetDesc() const;

	protected:
		DepthStencil(GraphicsID aID, const DepthStencilDesc& aDesc);

	private:
		PImpl<Texture2D> myTexture;
		DepthStencilDesc myDesc;
	};
}