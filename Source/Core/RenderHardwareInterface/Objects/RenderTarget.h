#pragma once
#include "GraphicsObject.h"
#include "../PImpl.hpp"

namespace graphics
{
	class Texture2D;

	struct RenderTargetDesc
	{
		std::string Name;
		Texture2D* Texture;
		GRAPHICS_FORMAT Format;
		GRAPHICS_RENDER_TARGET_VIEW_DIMENSION ViewDimension;				//The dimensionality of the view.
		unsigned int FirstSlice;											//The first slice for the DepthStencil to target.
		unsigned int NumSlices;												//The number of slices for the DepthStencil to target.
		unsigned int MipSlice;
	};

	class RenderTarget : public GraphicsObject
	{
	public:
		~RenderTarget();

		Texture2D* GetTexture();
		const RenderTargetDesc& GetDesc() const;

	protected:
		RenderTarget(GraphicsID aID, const RenderTargetDesc& aDesc);

	private:
		RenderTargetDesc myDesc;
		PImpl<Texture2D> myTexture;
	};
}