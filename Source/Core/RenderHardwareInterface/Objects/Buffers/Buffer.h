#pragma once
#include "../GraphicsObject.h"

namespace graphics
{
	class Buffer : public GraphicsObject
	{
	public:
		//virtual result::Code Write(void* aData, size_t aDataSize) = 0;
		//virtual result::Code Read(void* aBuffer, size_t aBufferSize, size_t aCount = 0) = 0;

		size_t GetSize() const;

	protected:
		Buffer(GraphicsID aID, size_t aSize);

	private:
		size_t mySize;
	};
}