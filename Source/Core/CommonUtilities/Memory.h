#pragma once
#include <ostream>
#include <istream>

namespace CommonUtilities
{
	class Memory
	{
	public:
		Memory() noexcept;
		Memory(size_t aSize) noexcept;
		Memory(void* aPtr, size_t aSize) noexcept;
		Memory(Memory&& aOwnedMemory) noexcept;
		Memory(const Memory& aOwnedMemory) noexcept;
		~Memory();

		bool Read(void* aPtr, size_t aSize) const noexcept;
		bool Write(void* aPtr, size_t aSize) noexcept;

		bool Read(std::ostream& aStream, size_t aCount) const noexcept;
		bool Write(std::istream& aStream, size_t aCount) noexcept;

		void* Get() const noexcept;
		size_t Size() const noexcept;
		void Reset() noexcept;

		Memory& operator=(Memory&& aOwnedMemory) noexcept;
		Memory& operator=(const Memory& aOwnedMemory) noexcept;

	private:
		void _reallocate(size_t aSize);

		void* myMemory;
		size_t mySize;
	};
}