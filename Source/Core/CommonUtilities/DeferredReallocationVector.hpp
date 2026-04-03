#pragma once
#include <vector>
#include <type_traits>

namespace CommonUtilities
{
	template<typename _Ty, typename _SizeTy = size_t>
	class DeferredReallocationVector;

	template<typename _Ty, typename _SizeTy = size_t>
	class DRVIterator;

	template<typename _Ty, typename _SizeTy = size_t>
	class DRVConstIterator;

	template<typename _Ty, typename _SizeTy = size_t>
	struct DRVAllocation
	{
		typedef _Ty value_type;
		typedef _SizeTy size_type;

		value_type* Start;
		value_type* Finish;
		value_type* Begin;
		value_type* End;
	};

	template<typename _Ty, typename _SizeTy = size_t>
	class DRVIterator
	{
		friend class DeferredReallocationVector<_Ty, _SizeTy>;
		friend class DRVConstIterator<_Ty, _SizeTy>;
	public:
		typedef _Ty value_type;
		typedef _SizeTy size_type;
		typedef DRVAllocation<_Ty, _SizeTy> allocation;

		DRVIterator();
		DRVIterator(value_type* aPtr, allocation* aAllocations, size_type aNumAllocations);

		DRVIterator operator++();
		DRVIterator& operator++(int);

		DRVIterator operator--();
		DRVIterator& operator--(int);

		DRVIterator operator+(size_type aSize) const;
		DRVIterator operator-(size_type aSize) const;

		_Ty* operator->() const;
		_Ty& operator*() const;

	private:
		value_type* myPtr;
		size_type myIndex;
		allocation* myAllocations;
		size_type myNumAllocation;
	};

	template<typename _Ty, typename _SizeTy = size_t>
	class DRVConstIterator
	{
		friend class DeferredReallocationVector<_Ty, _SizeTy>;
	public:
		typedef _Ty value_type;
		typedef _SizeTy size_type;
		typedef DRVAllocation<_Ty, _SizeTy> allocation;

		DRVIterator();
		DRVIterator(value_type* aPtr, allocation* aAllocations, size_type aNumAllocations);
		DRVIterator(const DRVIterator<_Ty, _SizeTy>& aIterator);

		DRVIterator operator++();
		DRVIterator& operator++(int);

		DRVIterator operator--();
		DRVIterator& operator--(int);

		DRVIterator operator+(size_type aSize) const;
		DRVIterator operator-(size_type aSize) const;

		const _Ty* operator->() const;
		const _Ty& operator*() const;

	private:
		value_type* myPtr;
		size_type myIndex;
		allocation* myAllocations;
		size_type myNumAllocation;
	};

	template<typename _Ty, typename _SizeTy = size_t>
	class DeferredReallocationVector
	{
	public:
		typedef _Ty value_type;
		typedef _Ty& reference;
		typedef _SizeTy size_type;
		typedef DRVAllocation<_Ty, _SizeTy> allocation;
		typedef DRVIterator<_Ty, _SizeTy> iterator;
		typedef DRVConstIterator<_Ty, _SizeTy> const_iterator;

		DeferredReallocationVector();
		DeferredReallocationVector(size_type aSize, const value_type& aValue);
		DeferredReallocationVector(const const_iterator& aBegin, const const_iterator& aEnd);
		DeferredReallocationVector(size_type aInitialSize);

		template<typename _IteratorTy>
		void assign(const _IteratorTy& aBegin, const _IteratorTy& aEnd);

		void erase(const const_iterator& aWhere);
		void erase(const const_iterator& aBegin, const const_iterator& aEnd);

		reference at(size_type aIndex);
		const reference at(size_type aIndex) const;

		reference front();
		const reference front() const;

		reference back();
		const reference back() const;

		size_type size() const;
		size_type capacity() const;
		bool empty() const;

		void reserve(size_type aSize);
		void resize(size_type aSize);

		void clear();

		void push_back(const value_type& aValue);
		void push_back(value_type&& aValue);

		template<class... _Args>
		reference emplace_back(_Args&& aArgs...);

		void pop_back();

		void swap(DeferredReallocationVector& aOther);

		iterator insert(const const_iterator& aWhere, const value_type& aValue);
		iterator insert(const const_iterator& aWhere, value_type&& aValue);
		iterator insert(const const_iterator& aWhere, std::initializer_list<value_type> aInitializerList);

		template<typename _IteratorTy>
		iterator insert(const const_iterator& aWhere, const _IteratorTy& aBegin, const _IteratorTy& aEnd);

		template<class... _Args>
		iterator emplace(const const_iterator& aWhere, _Args&& aArgs...);

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		const_iterator cbegin();
		const_iterator cend();

		_Ty& operator[](size_type aIndex);
		const _Ty& operator[](size_type aIndex) const;

	private:
		allocation _allocate(size_type aSize);
		void _free(allocation& aAllocation);
		void _moveForwards(size_type aCount);

		std::vector<allocation> myAllocations;
	};



	template<typename _Ty, typename _SizeTy>
	inline DeferredReallocationVector<_Ty, _SizeTy>::DeferredReallocationVector()
	{
		reserve(2);
	}
	template<typename _Ty, typename _SizeTy>
	inline DeferredReallocationVector<_Ty, _SizeTy>::DeferredReallocationVector(size_type aSize, const value_type& aValue)
	{
		reserve(aSize);
		for (size_type i = 0; i < aSize; i++)
		{
			push_back(aValue);
		}
	}
	template<typename _Ty, typename _SizeTy>
	inline DeferredReallocationVector<_Ty, _SizeTy>::DeferredReallocationVector(const const_iterator& aBegin, const const_iterator& aEnd)
	{
		assign(aBegin, aEnd);
	}
	template<typename _Ty, typename _SizeTy>
	inline DeferredReallocationVector<_Ty, _SizeTy>::DeferredReallocationVector(size_type aInitialSize)
	{
		reserve(aInitialSize);
	}
	template<typename _Ty, typename _SizeTy>
	template<typename _IteratorTy>
	inline void DeferredReallocationVector<_Ty, _SizeTy>::assign(const _IteratorTy& aBegin, const _IteratorTy& aEnd)
	{
		auto it = aBegin;
		while (it != aEnd)
		{
			push_back(*it);
			++it;
		}
	}
	template<typename _Ty, typename _SizeTy>
	inline void DeferredReallocationVector<_Ty, _SizeTy>::erase(const const_iterator& aWhere)
	{
		allocation& alloc = myAllocations[aWhere.myIndex];

		const_iterator next = aWhere;
		const_iterator it = next++;

		while (next.myPtr != alloc.End)
		{
			insert(it, std::move(next));
			it = next;
			++next;
		}

		it->~value_type();
		--alloc.End;
	}
	template<typename _Ty, typename _SizeTy>
	inline void DeferredReallocationVector<_Ty, _SizeTy>::erase(const const_iterator& aBegin, const const_iterator& aEnd)
	{
		allocation& alloc = myAllocations[aWhere.myIndex];

		const_iterator next = aEnd;
		const_iterator it = aBegin;

		while (next.myPtr != alloc.End)
		{
			insert(it, std::move(next));
			it = next;
			++next;
		}

		while (it != aEnd)
		{
			it->~value_type();
			++it;
			--alloc.End;
		}
	}
	template<typename _Ty, typename _SizeTy>
	inline typename DeferredReallocationVector<_Ty, _SizeTy>::allocation DeferredReallocationVector<_Ty, _SizeTy>::_allocate(size_type aSize)
	{
		void* mem = malloc(aSize * sizeof(value_type));

		allocation alloc = {};
		alloc.Start = reinterpret_cast<_Ty*>(mem);
		alloc.Finish = alloc.Start + aSize;
		alloc.Begin = alloc.Start;
		alloc.End = alloc.Start;

		return alloc;
	}
	template<typename _Ty, typename _SizeTy>
	inline void DeferredReallocationVector<_Ty, _SizeTy>::_free(allocation& aAllocation)
	{
		auto it = iterator(aAllocation.End, &aAllocation, 1);
		auto begin = iterator(aAllocation.Begin, &aAllocation, 1);
		while (it != begin)
		{
			--it;
			it->~size_type();
		}
		free(reinterpret_cast<void*>(aAllocation.Start));
		aAllocation.Start = nullptr;
		aAllocation.Finish = nullptr;
		aAllocation.Begin = nullptr;
		aAllocation.End = nullptr;
	}
	template<typename _Ty, typename _SizeTy>
	inline void DeferredReallocationVector<_Ty, _SizeTy>::_moveForwards(size_type aCount)
	{
		size_type count = aCount;
		allocation* sourceAlloc = &myAllocations.front();
		allocation* targetAlloc = &myAllocations.back();

		while (count > 0 && myAllocations.size() > 1)
		{
			value_type& source = *--sourceAlloc.End;

			push_back(std::move(source));
			targetAlloc = &myAllocations.back();

			if (sourceAlloc.Begin == sourceAlloc.End)
			{
				_free(sourceAlloc);
				myAllocations.erase(myAllocations.begin());
				sourceAlloc = &myAllocations.front();
			}

			--count;
		}
	}
}