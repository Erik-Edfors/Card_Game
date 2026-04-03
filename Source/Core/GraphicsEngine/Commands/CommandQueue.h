#pragma once
#include <memory>
#include <assert.h>

class CommandQueue
{
public:
	typedef char Byte;
	static_assert(sizeof(Byte) == 1, "Byte must have a size of 1!");

	CommandQueue() noexcept;
	CommandQueue(CommandQueue&& aCommandQueue) noexcept;
	CommandQueue(const CommandQueue& aCommandQueue) = delete;
	CommandQueue(size_t aInitialSize);
	~CommandQueue();

	template<typename _CommandTy, void(_CommandTy::* _Func)() = &_CommandTy::Execute>
	void Push(_CommandTy&& aCommand);
	template<typename _CommandTy, void(_CommandTy::* _Func)() = &_CommandTy::Execute>
	void Push(const _CommandTy& aCommand);

	void Run();

	CommandQueue& operator=(CommandQueue&& aCommandQueue) noexcept;

private:
	typedef void(*_entryDestructor)(const void*);
	typedef void(*_entryExecutor)(void*);
	
	void _resize(size_t aNewSize);

	template<typename _Ty>
	void _pushItem(_Ty&& aItem);
	template<typename _Ty>
	void _pushItem(const _Ty& aItem);

	template<typename _Ty>
	_Ty* _popItem();

	void* _popBySize(size_t aSize);

	Byte* myQueueStart;
	Byte* myQueueEnd;
	size_t myQueueSize;

	size_t myCursor;
};

template<typename _CommandTy, void(_CommandTy::*_Func)()>
inline void CommandQueue::Push(_CommandTy&& aCommand)
{
	constexpr _entryExecutor executor = [](void* aEntry) { (reinterpret_cast<_CommandTy*>(aEntry)->*_Func)(); };
	constexpr _entryDestructor destructor = [](const void* aEntry) { reinterpret_cast<const _CommandTy*>(aEntry)->~_CommandTy(); };

	_pushItem<_entryExecutor>(executor);
	_pushItem<_entryDestructor>(destructor);
	_pushItem<size_t>(sizeof(_CommandTy));
	_pushItem<_CommandTy>(std::move(aCommand));
}

template<typename _CommandTy, void(_CommandTy::* _Func)()>
inline void CommandQueue::Push(const _CommandTy& aCommand)
{
	constexpr _entryExecutor executor = [](void* aEntry) { (reinterpret_cast<_CommandTy*>(aEntry)->*_Func)(); };
	constexpr _entryDestructor destructor = [](const void* aEntry) { reinterpret_cast<const _CommandTy*>(aEntry)->~_CommandTy(); };

	_pushItem<_entryExecutor>(executor);
	_pushItem<_entryDestructor>(destructor);
	_pushItem<size_t>(sizeof(_CommandTy));
	_pushItem<_CommandTy>(aCommand);
}

template<typename _Ty>
inline void CommandQueue::_pushItem(_Ty&& aItem)
{
	constexpr size_t TypeSize = sizeof(_Ty);

	if (myCursor + TypeSize >= myQueueSize)
	{
		size_t newSize = myQueueSize << 1;
		while (myCursor + TypeSize > newSize)
		{
			newSize = newSize << 1;
		}
		_resize(newSize);
	}

	_Ty* ptr = reinterpret_cast<_Ty*>(&myQueueStart[myCursor]);
	::new(ptr) _Ty(std::move(aItem));
	myCursor += TypeSize;
	myQueueEnd = myQueueStart + myCursor;
}

template<typename _Ty>
inline void CommandQueue::_pushItem(const _Ty& aItem)
{
	constexpr size_t TypeSize = sizeof(_Ty);

	if (myCursor + TypeSize >= myQueueSize)
	{
		size_t newSize = myQueueSize << 1;
		while (myCursor + TypeSize > newSize)
		{
			newSize = newSize << 1;
		}
		_resize(newSize);
	}

	_Ty* ptr = reinterpret_cast<_Ty*>(&myQueueStart[myCursor]);
	::new(ptr) _Ty(aItem);
	myCursor += TypeSize;
	myQueueEnd = myQueueStart + myCursor;
}

template<typename _Ty>
inline _Ty* CommandQueue::_popItem()
{
	_Ty* ptr = reinterpret_cast<_Ty*>(myQueueStart + myCursor);
	myCursor += sizeof(_Ty);
	return ptr;
}
