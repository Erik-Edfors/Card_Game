#pragma once
#include <algorithm>
#include "Nullable.hpp"

namespace CommonUtilities
{
	template<typename _IndexTy, typename _ValueTy, typename _SizeTy = unsigned long long>
	class Table
	{
	public:
		typedef _IndexTy index;
		typedef _ValueTy value;
		typedef _SizeTy size;

		static constexpr size Default_Size = static_cast<size>(4);

		Table();
		Table(
			size aInitialSize			// The initial size of the table. Note that the table will resize when reaching half capacity to reduce collisions.
		);
		Table(Table&& aTable);
		Table(const Table& aTable);
		~Table();

		// If the table is smaller than aSize, resizes the table to contain the specified size. This will cause reallocation if resized.
		void reserve(
			size aSize					// A minimum size of the table. Note that the table will resize when reaching half capacity to reduce collisions.
		);

		// Emplaces a value by move into the table with the associated index.
		void emplace(
			const index& aIndex,		// A index to associate with the emplaced value.
			value&& aValue				// A value to move into the table.
		);
		// Emplaces a value by copy into the table with the associated index.
		void emplace(
			const index& aIndex,		// A index to associate with the emplaced value.
			const value& aValue			// A value to copy into the table.
		);

		// Retrieves a reference to the value associated with the given index. Will throw if no value is associated with the index.
		value& at(
			const index& aIndex			// The index associated with the value.
		);
		// Retrieves a reference to the value associated with the given index. Will throw if no value is associated with the index.
		const value& at(
			const index& aIndex			// The index associated with the value.
		) const;

		// Retrieves the pointer to the value associated with aIndex. n = size -> O(n) time complexity if index has no associated value.
		value* find(
			const index& aIndex			// A index.
		);
		// Retrieves the pointer to the value associated with aIndex. n = size -> O(n) time complexity if index has no associated value.
		const value* find(
			const index& aIndex			// A index.
		) const;

		// Erases the value associated with the index. n = size -> O(n) time complexity if index has no associated value.
		bool erase(
			const index& aIndex			// A index.
		);

		// Erases all values in the table. n = size -> O(n) time complexity.
		void clear();

		// Retrieves a reference to the value associated with the given index. Will throw if no value is associated with the index.
		value& operator[](
			const index& aIndex			// The index associated with the value.
			);
		// Retrieves a reference to the value associated with the given index. Will throw if no value is associated with the index.
		const value& operator[](
			const index& aIndex			// The index associated with the value.
			) const;

	private:
		struct _entry
		{
			index Index;
			Nullable<value> Value;
		};

		void _assureSpace();
		_entry& _getEmpty(index aIndex);
		_entry* _find(index aIndex);
		const _entry* _find(index aIndex) const;
		void _reallocate(size aSize);

		_entry* myEntries;
		size myCount;
		size mySize;
	};

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline Table<_IndexTy, _ValueTy, _SizeTy>::Table() :
		myEntries(new _entry[static_cast<unsigned long long>(Default_Size)]),
		myCount(static_cast<size>(0)),
		mySize(Default_Size)
	{
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline Table<_IndexTy, _ValueTy, _SizeTy>::Table(size aInitialSize) :
		myEntries(new _entry[static_cast<unsigned long long>(aInitialSize)]),
		myCount(static_cast<size>(0)),
		mySize(aInitialSize)
	{
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline Table<_IndexTy, _ValueTy, _SizeTy>::Table(Table&& aTable) :
		myEntries(std::move(aTable.myEntries)),
		myCount(std::move(aTable.myCount)),
		mySize(std::move(aTable.mySize))
	{
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline Table<_IndexTy, _ValueTy, _SizeTy>::Table(const Table& aTable) :
		myEntries(new _entry[static_cast<unsigned long long>(aTable.mySize)]),
		myCount(aTable.myCount),
		mySize(aTable.mySize)
	{
		std::copy(aTable.myEntries, aTable.myEntries + aTable.mySize, myEntries);
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline Table<_IndexTy, _ValueTy, _SizeTy>::~Table()
	{
		delete[] myEntries;
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline void Table<_IndexTy, _ValueTy, _SizeTy>::reserve(size aSize)
	{
		if (mySize < aSize)
		{
			_reallocate(aSize);
		}
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline void Table<_IndexTy, _ValueTy, _SizeTy>::emplace(const index& aIndex, value&& aValue)
	{
		_entry& entry = _getEmpty(aIndex);
		entry.Index = aIndex;
		entry.Value = aValue;
		myCount++;
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline void Table<_IndexTy, _ValueTy, _SizeTy>::emplace(const index& aIndex, const value& aValue)
	{
		_entry& entry = _getEmpty(aIndex);
		entry.Index = aIndex;
		entry.Value = aValue;
		myCount++;
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline typename Table<_IndexTy, _ValueTy, _SizeTy>::value& Table<_IndexTy, _ValueTy, _SizeTy>::at(const index& aIndex)
	{
		return *find(aIndex);
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline const typename Table<_IndexTy, _ValueTy, _SizeTy>::value& Table<_IndexTy, _ValueTy, _SizeTy>::at(const index& aIndex) const
	{
		return *find(aIndex);
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline typename Table<_IndexTy, _ValueTy, _SizeTy>::value* Table<_IndexTy, _ValueTy, _SizeTy>::find(const index& aIndex)
	{
		_entry* entry = _find(aIndex);
		if (entry != nullptr)
		{
			return entry->Value.Get();
		}
		else
		{
			return nullptr;
		}
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline const typename Table<_IndexTy, _ValueTy, _SizeTy>::value* Table<_IndexTy, _ValueTy, _SizeTy>::find(const index& aIndex) const
	{
		_entry* entry = _find(aIndex);
		if (entry != nullptr)
		{
			return entry->Value.Get();
		}
		else
		{
			return nullptr;
		}
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline bool Table<_IndexTy, _ValueTy, _SizeTy>::erase(const index& aIndex)
	{
		_entry* entry = _find(aIndex);
		if (entry != nullptr)
		{
			entry->Value.SetNull();
			--myCount;
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline void Table<_IndexTy, _ValueTy, _SizeTy>::clear()
	{
		for (size i = 0; i < mySize; i++)
		{
			myEntries[i].Value.SetNull();
		}
		myCount = 0;
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline typename Table<_IndexTy, _ValueTy, _SizeTy>::value& Table<_IndexTy, _ValueTy, _SizeTy>::operator[](const index& aIndex)
	{
		return at(aIndex);
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline const typename Table<_IndexTy, _ValueTy, _SizeTy>::value& Table<_IndexTy, _ValueTy, _SizeTy>::operator[](const index& aIndex) const
	{
		return at(aIndex);
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline void Table<_IndexTy, _ValueTy, _SizeTy>::_assureSpace()
	{
		if (mySize < static_cast<size>(2) * myCount)
		{
			_reallocate(mySize << 1);
		}
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline typename Table<_IndexTy, _ValueTy, _SizeTy>::_entry& Table<_IndexTy, _ValueTy, _SizeTy>::_getEmpty(index aIndex)
	{
		_assureSpace();

		size ind = static_cast<size>(aIndex) % mySize;

		while (myEntries[ind].Value.IsNull() == false)
		{
			++ind;
		}
		return myEntries[ind];
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline typename Table<_IndexTy, _ValueTy, _SizeTy>::_entry* Table<_IndexTy, _ValueTy, _SizeTy>::_find(index aIndex)
	{
		size ind = static_cast<size>(aIndex) % mySize;

		size count = static_cast<size>(0);

		while (myEntries[ind].Index != aIndex)
		{
			++ind;
			++count;

			if (count >= mySize)
			{
				return nullptr;
			}
		}
		return &myEntries[ind];
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline const typename Table<_IndexTy, _ValueTy, _SizeTy>::_entry* Table<_IndexTy, _ValueTy, _SizeTy>::_find(index aIndex) const
	{
		size ind = static_cast<size>(aIndex) % mySize;

		size count = static_cast<size>(0);

		while (myEntries[ind].Index != aIndex)
		{
			++ind;
			++count;

			if (count >= mySize)
			{
				return nullptr;
			}
		}
		return &myEntries[ind];
	}

	template<typename _IndexTy, typename _ValueTy, typename _SizeTy>
	inline void Table<_IndexTy, _ValueTy, _SizeTy>::_reallocate(size aSize)
	{
		_entry* oldEntries = myEntries;
		size oldSize = mySize;

		mySize = aSize;
		myEntries = new _entry[mySize];
		myCount = 0;

		for (size i = 0; i < oldSize; i++)
		{
			if (oldEntries[i].Value.IsNull() == false)
			{
				emplace(oldEntries[i].Index, oldEntries[i].Value);
			}
		}

		delete[] oldEntries;
	}
}