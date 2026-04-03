#pragma once
#include "Buffer.h"

namespace graphics
{
	struct VertexBufferDesc
	{

	};

	class VertexBuffer : public Buffer
	{
	protected:
		VertexBuffer(GraphicsID aID, size_t aSize);
	};
}