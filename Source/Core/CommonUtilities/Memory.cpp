#include "Memory.h"
#include <malloc.h>
#include <cstring>
#include <utility>

CommonUtilities::Memory::Memory() noexcept :
	myMemory(nullptr),
	mySize(0ull)
{
}

CommonUtilities::Memory::Memory(size_t aSize) noexcept :
	myMemory(malloc(aSize)),
	mySize(aSize)
{
}

CommonUtilities::Memory::Memory(void* aPtr, size_t aSize) noexcept :
	myMemory(malloc(aSize)),
	mySize(aSize)
{
	if (myMemory != nullptr && aPtr != nullptr)
	{
		memcpy(myMemory, aPtr, aSize);
	}
}

CommonUtilities::Memory::Memory(Memory&& aOwnedMemory) noexcept :
	myMemory(std::move(aOwnedMemory.myMemory)),
	mySize(std::move(aOwnedMemory.mySize))
{
	aOwnedMemory.myMemory = nullptr;
	aOwnedMemory.mySize = 0;
}

CommonUtilities::Memory::Memory(const Memory& aOwnedMemory) noexcept :
	myMemory(malloc(aOwnedMemory.mySize)),
	mySize(aOwnedMemory.mySize)
{
	if (myMemory != nullptr)
	{
		memcpy(myMemory, aOwnedMemory.myMemory, mySize);
	}
}

CommonUtilities::Memory::~Memory()
{
	Reset();
}

bool CommonUtilities::Memory::Read(void* aPtr, size_t aSize) const noexcept
{
	if (myMemory == nullptr || mySize < aSize || aPtr == nullptr)
	{
		return false;
	}
	memcpy(myMemory, aPtr, aSize);
	return true;
}

bool CommonUtilities::Memory::Write(void* aPtr, size_t aSize) noexcept
{
	if (aPtr == nullptr)
	{
		return false;
	}
	_reallocate(aSize);
	memcpy(aPtr, myMemory, aSize);
	return true;
}

bool CommonUtilities::Memory::Read(std::ostream& aStream, size_t aCount) const noexcept
{
	if (myMemory == nullptr || aCount > mySize)
	{
		return false;
	}
	aStream.write(reinterpret_cast<const char*>(myMemory), aCount);
	return true;
}

bool CommonUtilities::Memory::Write(std::istream& aStream, size_t aCount) noexcept
{
	_reallocate(aCount);
	size_t counter = 0;

	while (counter != aCount)
	{
		aStream.read(reinterpret_cast<char*>(myMemory) + counter, aCount - counter);
		counter += aStream.gcount();
		if (aStream.fail())
		{
			return false;
		}
	}
	return true;
}

void* CommonUtilities::Memory::Get() const noexcept
{
	return myMemory;
}

size_t CommonUtilities::Memory::Size() const noexcept
{
	return mySize;
}

void CommonUtilities::Memory::Reset() noexcept
{
	if (myMemory != nullptr)
	{
		free(myMemory);
		myMemory = nullptr;
	}
	mySize = 0;
}

CommonUtilities::Memory& CommonUtilities::Memory::operator=(Memory&& aOwnedMemory) noexcept
{
	Reset();
	myMemory = std::move(aOwnedMemory.myMemory);
	mySize = std::move(aOwnedMemory.mySize);

	aOwnedMemory.myMemory = nullptr;
	aOwnedMemory.mySize = 0;

	return *this;
}

CommonUtilities::Memory& CommonUtilities::Memory::operator=(const Memory& aOwnedMemory) noexcept
{
	Reset();
	if (aOwnedMemory.myMemory != nullptr)
	{
		mySize = aOwnedMemory.mySize;
		myMemory = malloc(mySize);

		if (myMemory != nullptr)
		{
			memcpy(myMemory, aOwnedMemory.myMemory, mySize);
		}
	}
	return *this;
}

void CommonUtilities::Memory::_reallocate(size_t aSize)
{
	Reset();
	myMemory = malloc(aSize);
	mySize = aSize;
}
