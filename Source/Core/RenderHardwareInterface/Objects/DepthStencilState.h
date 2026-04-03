#pragma once
#include "GraphicsObject.h"
#include "../PImpl.hpp"

namespace graphics
{
	class Texture2D;

	struct DepthStencilOperationDesc
	{
		GRAPHICS_STENCIL_OPERATION FailOp;
		GRAPHICS_STENCIL_OPERATION DepthFailOp;
		GRAPHICS_STENCIL_OPERATION PassOp;
		GRAPHICS_COMPARISON Comparison;
	};

	struct DepthStencilStateDesc
	{
		std::string Name;
		bool EnableDepth;
		GRAPHICS_DEPTH_WRITE_MASK DepthWriteMask;
		GRAPHICS_COMPARISON DepthComparison;
		bool EnableStencil;
		unsigned char StencilReadMask;
		unsigned char StencilWriteMask;
		DepthStencilOperationDesc FrontFace;
		DepthStencilOperationDesc BackFace;
	};

	class DepthStencilState : public GraphicsObject
	{
	public:
		~DepthStencilState();

		const DepthStencilStateDesc& GetDesc() const;

	protected:
		DepthStencilState(GraphicsID aID, const DepthStencilStateDesc& aDesc);

	private:
		DepthStencilStateDesc myDesc;
	};
}