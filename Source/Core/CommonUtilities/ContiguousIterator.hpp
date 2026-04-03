#pragma once
#include <iterator>
#include <cstddef>

namespace CommonUtilities
{
	template<typename _Ty>
	class ContiguousIterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = _Ty;
		using difference_type = std::ptrdiff_t;
		using pointer = _Ty*;
		using reference = _Ty&;

		// Constructor
		explicit ContiguousIterator(pointer ptr = nullptr) noexcept : myPtr(ptr) {}

		// Dereference operators
		reference operator*() const { return *myPtr; }
		pointer operator->() const noexcept { return myPtr; }

		// Increment and decrement operators
		ContiguousIterator& operator++() noexcept { ++myPtr; return *this; }
		ContiguousIterator operator++(int) noexcept { ContiguousIterator temp = *this; ++(*this); return temp; }
		ContiguousIterator& operator--() noexcept { --myPtr; return *this; }
		ContiguousIterator operator--(int) noexcept { ContiguousIterator temp = *this; --(*this); return temp; }

		// Arithmetic operators
		ContiguousIterator operator+(difference_type n) const noexcept { return ContiguousIterator(myPtr + n); }
		ContiguousIterator operator-(difference_type n) const noexcept { return ContiguousIterator(myPtr - n); }
		difference_type operator-(const ContiguousIterator& other) const noexcept { return myPtr - other.myPtr; }

		// Comparison operators
		bool operator==(const ContiguousIterator& other) const noexcept { return myPtr == other.myPtr; }
		bool operator!=(const ContiguousIterator& other) const noexcept { return myPtr != other.myPtr; }
		bool operator<(const ContiguousIterator& other) const noexcept { return myPtr < other.myPtr; }
		bool operator>(const ContiguousIterator& other) const noexcept { return myPtr > other.myPtr; }
		bool operator<=(const ContiguousIterator& other) const noexcept { return myPtr <= other.myPtr; }
		bool operator>=(const ContiguousIterator& other) const noexcept { return myPtr >= other.myPtr; }

	private:
		pointer myPtr;
	};
}