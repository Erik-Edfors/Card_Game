#pragma once
#include "Buffer.h"

namespace graphics
{
	struct VertexBufferDesc
	{
		std::string Name;											//A name to assign the object.
		const void* Data;											//A pointe to data to fill the vertex buffer with. If nullptr, will only allocate space. Can only be nullptr if aUsage is not GRAPHICS_USAGE_IMMUTABLE.
		size_t Size;												//The size of the vertex buffer in bytes.
		GRAPHICS_USAGE Usage;										//The usage of the vertex buffer.
		GRAPHICS_CPU_ACCESS_FLAGS CPUAccessFlags;					//CPU Access flags. Non-immutable buffers should be writable by the cpu.
	};

	class VertexBuffer : public Buffer
	{
	protected:
		VertexBuffer(GraphicsID aID, size_t aSize);
	};
}