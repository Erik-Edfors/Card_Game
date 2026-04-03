#pragma once
#include "Buffer.h"

namespace graphics
{
	struct ConstantBufferDesc
	{
		std::string Name;													//A name to assign the object.
		const void* InitialData;											//A pointer to data to fill the constant buffer with. If nullptr, will only allocate space. Can only be nullptr if aUsage is not GRAPHICS_USAGE_IMMUTABLE.
		size_t Size;														//The size of constant buffer in bytes.
		GRAPHICS_USAGE Usage;												//The usage of the constant buffer.
		GRAPHICS_CPU_ACCESS_FLAGS CPUAccessFlags;							//CPU access flags. Non-immutable buffers should be writable by the cpu.
	};

	class ConstantBuffer : public Buffer
	{
	public:

	protected:
		ConstantBuffer(GraphicsID aID, size_t aSize);
	};
}