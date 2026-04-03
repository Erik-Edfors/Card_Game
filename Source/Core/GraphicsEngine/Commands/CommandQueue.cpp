#include "GraphicsEngine.pch.h"
#include "CommandQueue.h"
#include <new>

CommandQueue::CommandQueue() noexcept :
	myQueueSize(64),
	myCursor(0)
{
	myQueueStart = new char[myQueueSize];
	myQueueEnd = myQueueStart;
}

CommandQueue::CommandQueue(CommandQueue&& aCommandQueue) noexcept :
	myQueueSize(std::move(aCommandQueue.myQueueSize)),
	myCursor(std::move(aCommandQueue.myCursor)),
	myQueueStart(std::move(aCommandQueue.myQueueStart)),
	myQueueEnd(std::move(aCommandQueue.myQueueEnd))
{
	aCommandQueue.myQueueStart = nullptr;
	aCommandQueue.myQueueEnd = nullptr;
}

CommandQueue::CommandQueue(size_t aInitialSize) :
	myQueueSize(aInitialSize),
	myCursor(0)
{
	myQueueStart = new char[myQueueSize];
	myQueueEnd = myQueueStart;
}

CommandQueue::~CommandQueue()
{
	myCursor = 0;

	if (myQueueStart)
	{
		while (myQueueStart + myCursor < myQueueEnd)
		{
			_popItem<_entryExecutor>();
			_entryDestructor* destructor = _popItem<_entryDestructor>();
			size_t* typeSize = _popItem<size_t>();
			void* cmd = _popBySize(*typeSize);

			(*destructor)(cmd);
		}

		delete[] myQueueStart;
	}
}

void CommandQueue::Run()
{
	myCursor = 0;

	while (myQueueStart + myCursor < myQueueEnd)
	{
		_entryExecutor* executor = _popItem<_entryExecutor>();
		_entryDestructor* destructor = _popItem<_entryDestructor>();
		size_t* typeSize = _popItem<size_t>();
		void* cmd = _popBySize(*typeSize);

		(*executor)(cmd);
		(*destructor)(cmd);
	}

	myCursor = 0;
	myQueueEnd = myQueueStart;
}

CommandQueue& CommandQueue::operator=(CommandQueue&& aCommandQueue) noexcept
{
	myQueueSize = std::move(aCommandQueue.myQueueSize);
	myCursor = std::move(aCommandQueue.myCursor);
	myQueueStart = std::move(aCommandQueue.myQueueStart);
	myQueueEnd = std::move(aCommandQueue.myQueueEnd);

	aCommandQueue.myQueueStart = nullptr;
	aCommandQueue.myQueueEnd = nullptr;
	return *this;
}

void CommandQueue::_resize(size_t aNewSize)
{
	size_t count = myQueueEnd - myQueueStart;
	Byte* newQueue = new Byte[aNewSize];
	memcpy(newQueue, myQueueStart, count);

	delete[] myQueueStart;

	myQueueSize = aNewSize;
	myQueueStart = newQueue;
	myQueueEnd = newQueue + count;
}

void* CommandQueue::_popBySize(size_t aSize)
{
	void* ptr = myQueueStart + myCursor;
	myCursor += aSize;
	return ptr;
}
