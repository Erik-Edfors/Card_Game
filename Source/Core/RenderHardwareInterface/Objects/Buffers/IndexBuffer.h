#pragma once
#include "Buffer.h"

namespace graphics
{
	struct IndexBufferDesc
	{

	};

	class IndexBuffer : public Buffer
	{
	protected:
		IndexBuffer(GraphicsID aID, size_t aSize);
	};
}