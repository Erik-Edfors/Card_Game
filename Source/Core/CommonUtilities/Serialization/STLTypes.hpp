#pragma once
#include "BasicTypes.hpp"
#include <xmemory>
#include <xstring>

namespace std
{
	template <class _Elem, class _Traits, class _Alloc>
	class basic_string;

	template <class _Ty1, class _Ty2>
	struct pair;

	using string = basic_string<char, char_traits<char>, allocator<char>>;

	template <class _Ty, size_t _Size>
	class array;

	template <class _Ty, class _Alloc>
	class vector;

	template <class _Ty, class _Container>
	class queue;

	template <class _Kty, class _Ty, class _Pr, class _Alloc>
	class map;

	template <class _Kty, class _Ty, class _Hasher, class _Keyeq, class _Alloc>
	class unordered_map;

	template <class _Kty, class _Ty, class _Pr, class _Alloc>
	class multimap;

	template <class _Kty, class _Pr, class _Alloc>
	class set;

	template <class _Kty, class _Hasher, class _Keyeq, class _Alloc>
	class unordered_set;

	template <class _Kty, class _Pr, class _Alloc>
	class multiset;
}

namespace CommonUtilities
{
	template <class _Elem, class _Traits, class _Alloc>
	inline size_t Serialize(const std::basic_string<_Elem, _Traits, _Alloc>& aString, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Elem, class _Traits, class _Alloc>
	inline size_t Deserialize(std::basic_string<_Elem, _Traits, _Alloc>& aString, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template <class _Ty1, class _Ty2>
	inline size_t Serialize(const std::pair<_Ty1, _Ty2>& aPair, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Ty1, class _Ty2>
	inline size_t Deserialize(std::pair<_Ty1, _Ty2>& aPair, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template<class _ElemTy, size_t _Size>
	inline size_t Serialize(const std::array<_ElemTy, _Size>& aArray, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template<class _ElemTy, size_t _Size>
	inline size_t Deserialize(std::array<_ElemTy, _Size>& aArray, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template <class _Ty, class _Container>
	inline size_t Serialize(const std::queue<_Ty, _Container>& aQueue, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Ty, class _Container>
	inline size_t Deserialize(std::queue<_Ty, _Container>& aQueue, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template<class _ElemTy, typename _Alloc>
	inline size_t Serialize(const std::vector<_ElemTy, _Alloc>& aVector, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template<class _ElemTy, typename _Alloc>
	inline size_t Deserialize(std::vector<_ElemTy, _Alloc>& aVector, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});


	////////////////////////
	// map, unordered_map and multimap
	////////////////////////

	template <class _Kty, class _Ty, class _Pr, class _Alloc>
	inline size_t Serialize(const std::map<_Kty, _Ty, _Pr, _Alloc>& aMap, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Kty, class _Ty, class _Pr, class _Alloc>
	inline size_t Deserialize(std::map<_Kty, _Ty, _Pr, _Alloc>& aMap, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template <class _Kty, class _Ty, class _Hasher, class _Keyeq, class _Alloc>
	inline size_t Serialize(const std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>& aMap, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Kty, class _Ty, class _Hasher, class _Keyeq, class _Alloc>
	inline size_t Deserialize(std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>& aMap, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template <class _Kty, class _Ty, class _Pr, class _Alloc>
	inline size_t Serialize(const std::multimap<_Kty, _Ty, _Pr, _Alloc>& aMap, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Kty, class _Ty, class _Pr, class _Alloc>
	inline size_t Deserialize(std::multimap<_Kty, _Ty, _Pr, _Alloc>& aMap, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});


	////////////////////////
	// set, unordered_set and multiset
	////////////////////////

	template <class _Kty, class _Pr, class _Alloc>
	inline size_t Serialize(const std::set<_Kty, _Pr, _Alloc>& aSet, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Kty, class _Pr, class _Alloc>
	inline size_t Deserialize(std::set<_Kty, _Pr, _Alloc>& aSet, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template <class _Kty, class _Hasher, class _Keyeq, class _Alloc>
	inline size_t Serialize(const std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& aSet, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Kty, class _Hasher, class _Keyeq, class _Alloc>
	inline size_t Deserialize(std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& aSet, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});

	template <class _Kty, class _Pr, class _Alloc>
	inline size_t Serialize(const std::multiset<_Kty, _Pr, _Alloc>& aSet, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template <class _Kty, class _Pr, class _Alloc>
	inline size_t Deserialize(std::multiset<_Kty, _Pr, _Alloc>& aSet, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType = {});



	template<class _Elem, class _Traits, class _Alloc>
	size_t Serialize(const std::basic_string<_Elem, _Traits, _Alloc>& aString, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::basic_string<_Elem, _Traits, _Alloc>::value_type;
		using size_type = typename std::basic_string<_Elem, _Traits, _Alloc>::size_type;

		size_t size = 0;

		const size_t dataSize = sizeof(value_type) * aString.size();
		if (not aWriter.Writable())
		{
			throw Exception("Writer was not writable!");
		}

		size_t count = Serialize(aString.size(), aWriter, __NBS_GlobalNamespaceType{});
		if (count == 0)
		{
			throw WriteException("Writer failed to write!");
		}
		size += count;

		count = aWriter.Write(reinterpret_cast<const char*>(aString.data()), dataSize);
		if (count != dataSize)
		{
			throw WriteException("Writer failed to write!");
		}
		size += count;

		return size;
	}
	template<class _Elem, class _Traits, class _Alloc>
	size_t Deserialize(std::basic_string<_Elem, _Traits, _Alloc>& outString, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::basic_string<_Elem, _Traits, _Alloc>::value_type;
		using size_type = typename std::basic_string<_Elem, _Traits, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});

		const size_t dataSize = containerSize * sizeof(value_type);

		if (dataSize == 0)
		{
			return size;
		}

		if (dataSize + size > aAllocationLimit)
		{
			throw AllocationException("Allocation limit reached.");
		}

		if (not aReader.Readable())
		{
			throw Exception("Reader was not readable!");
		}

		outString.resize(containerSize);
		size_t count = aReader.Read(reinterpret_cast<char*>(outString.data()), dataSize);
		if (count != dataSize)
		{
			throw ReadException("Reader failed to read!");
		}
		size += count;

		return size;
	}



	template<class _Ty1, class _Ty2>
	size_t Serialize(const std::pair<_Ty1, _Ty2>& aPair, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		size_t size = 0;
		size += Serialize(aPair.first, aWriter, __NBS_GlobalNamespaceType{});
		size += Serialize(aPair.second, aWriter, __NBS_GlobalNamespaceType{});

		return size;
	}
	template<class _Ty1, class _Ty2>
	size_t Deserialize(std::pair<_Ty1, _Ty2>& outPair, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		size_t size = 0;
		size += Deserialize(outPair.first, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		size += Deserialize(outPair.second, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});

		return size;
	}



	template<class _ElemTy, size_t _Size>
	size_t Serialize(const std::array<_ElemTy, _Size>& aArray, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		size_t size = 0;
		for (size_t i = 0; i < aArray.size(); i++)
		{
			size += Serialize(aArray[i], aWriter, __NBS_GlobalNamespaceType{});
		}

		return size;
	}
	template<class _ElemTy, size_t _Size>
	size_t Deserialize(std::array<_ElemTy, _Size>& outArray, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		size_t size = 0;
		for (size_t i = 0; i < outArray.size(); i++)
		{
			size += Deserialize(outArray[i], aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		}

		return size;
	}


	template<class _Ty, class _Container>
	size_t Serialize(const std::queue<_Ty, _Container>& aQueue, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using size_type = typename std::queue<_Ty, _Container>::size_type;

		size_t size = 0;

		std::queue<_Ty, _Container> copy = aQueue;

		size += Serialize(copy.size(), aWriter, __NBS_GlobalNamespaceType{});
		while (not copy.empty())
		{
			size += Serialize(copy.front(), aWriter, __NBS_GlobalNamespaceType{});
			copy.pop();
		}
		return size;
	}
	template<class _Ty, class _Container>
	size_t Deserialize(std::queue<_Ty, _Container>& outQueue, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::queue<_Ty, _Container>::value_type;
		using size_type = typename std::queue<_Ty, _Container>::size_type;

		size_t size = 0;

		size_type size = 0;
		size += Deserialize(size, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});

		for (size_type i = 0; i < size; i++)
		{
			value_type value = {};
			size += Deserialize(value, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outQueue.push(std::move(value));
		}
		return size;
	}


	template<class _ElemTy, typename _Alloc>
	size_t Serialize(const std::vector<_ElemTy, _Alloc>& aVector, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::vector<_ElemTy, _Alloc>::value_type;

		size_t size = 0;

		size += Serialize(aVector.size(), aWriter, __NBS_GlobalNamespaceType{});
		for (size_t i = 0; i < aVector.size(); i++)
		{
			size += Serialize(aVector[i], aWriter, __NBS_GlobalNamespaceType{});
		}
		return size;
	}
	template<class _ElemTy, typename _Alloc>
	size_t Deserialize(std::vector<_ElemTy, _Alloc>& outVector, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::vector<_ElemTy, _Alloc>::value_type;
		using size_type = typename std::vector<_ElemTy, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		//DO NOT reserve vector. size can be ANY value, and it is not guarenteed that Reader will be able to read up to the size given.

		for (size_type i = 0; i < containerSize; i++)
		{
			value_type value = {};
			size += Deserialize(value, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outVector.push_back(std::move(value));
		}
		return size;
	}



	template<class _Kty, class _Ty, class _Pr, class _Alloc>
	size_t Serialize(const std::map<_Kty, _Ty, _Pr, _Alloc>& aMap, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::map<_Kty, _Ty, _Pr, _Alloc>::value_type;

		size_t size = 0;

		size += Serialize(aMap.size(), aWriter, __NBS_GlobalNamespaceType{});
		for (const value_type& entry : aMap)
		{
			size += Serialize(entry, aWriter, __NBS_GlobalNamespaceType{});
		}
		return size;
	}
	template<class _Kty, class _Ty, class _Pr, class _Alloc>
	size_t Deserialize(std::map<_Kty, _Ty, _Pr, _Alloc>& outMap, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = std::pair<_Kty, _Ty>;
		using size_type = typename std::map<_Kty, _Ty, _Pr, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		for (size_type i = 0; i < containerSize; i++)
		{
			value_type entry = {};
			size += Deserialize(entry, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outMap.insert({ std::move(entry.first), std::move(entry.second) });
		}
		return size;
	}



	template<class _Kty, class _Ty, class _Hasher, class _Keyeq, class _Alloc>
	size_t Serialize(const std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>& aMap, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>::value_type;

		size_t size = 0;

		size += Serialize(aMap.size(), aWriter, __NBS_GlobalNamespaceType{});
		for (const value_type& entry : aMap)
		{
			size += Serialize(entry, aWriter, __NBS_GlobalNamespaceType{});
		}
		return size;
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq, class _Alloc>
	size_t Deserialize(std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>& outMap, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = std::pair<_Kty, _Ty>;
		using size_type = typename std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		for (size_type i = 0; i < containerSize; i++)
		{
			value_type entry = {};
			size += Deserialize(entry, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outMap.insert({ std::move(entry.first), std::move(entry.second) });
		}
		return size;
	}



	template<class _Kty, class _Ty, class _Pr, class _Alloc>
	size_t Serialize(const std::multimap<_Kty, _Ty, _Pr, _Alloc>& aMap, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::multimap<_Kty, _Ty, _Pr, _Alloc>::value_type;

		size_t size = 0;

		size += Serialize(aMap.size(), aWriter, __NBS_GlobalNamespaceType{});
		for (const value_type& entry : aMap)
		{
			size += Serialize(entry, aWriter, __NBS_GlobalNamespaceType{});
		}
		return size;
	}
	template<class _Kty, class _Ty, class _Pr, class _Alloc>
	size_t Deserialize(std::multimap<_Kty, _Ty, _Pr, _Alloc>& outMap, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = std::pair<_Kty, _Ty>;
		using size_type = typename std::multimap<_Kty, _Ty, _Pr, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		for (size_type i = 0; i < containerSize; i++)
		{
			value_type entry = {};
			size += Deserialize(entry, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outMap.insert({ std::move(entry.first), std::move(entry.second) });
		}
		return size;
	}



	template<class _Kty, class _Pr, class _Alloc>
	size_t Serialize(const std::set<_Kty, _Pr, _Alloc>& aSet, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::set<_Kty, _Pr, _Alloc>::value_type;

		size_t size = 0;

		size += Serialize(aSet.size(), aWriter, __NBS_GlobalNamespaceType{});
		for (const value_type& entry : aSet)
		{
			size += Serialize(entry, aWriter, __NBS_GlobalNamespaceType{});
		}
		return size;
	}
	template<class _Kty, class _Pr, class _Alloc>
	size_t Deserialize(std::set<_Kty, _Pr, _Alloc>& outSet, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::set<_Kty, _Pr, _Alloc>::value_type;
		using size_type = typename std::set<_Kty, _Pr, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		for (size_type i = 0; i < containerSize; i++)
		{
			value_type entry = {};
			size += Deserialize(entry, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outSet.insert(std::move(entry));
		}
		return size;
	}



	template<class _Kty, class _Hasher, class _Keyeq, class _Alloc>
	size_t Serialize(const std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& aSet, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>::value_type;

		size_t size = 0;

		size += Serialize(aSet.size(), aWriter, __NBS_GlobalNamespaceType{});
		for (const value_type& entry : aSet)
		{
			size += Serialize(entry, aWriter, __NBS_GlobalNamespaceType{});
		}
		return size;
	}
	template<class _Kty, class _Hasher, class _Keyeq, class _Alloc>
	size_t Deserialize(std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>& outSet, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>::value_type;
		using size_type = typename std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		for (size_type i = 0; i < containerSize; i++)
		{
			value_type entry = {};
			size += Deserialize(entry, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outSet.insert(std::move(entry));
		}
		return size;
	}



	template<class _Kty, class _Pr, class _Alloc>
	size_t Serialize(const std::multiset<_Kty, _Pr, _Alloc>& aSet, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::multiset<_Kty, _Pr, _Alloc>::value_type;

		size_t size = 0;

		size += Serialize(aSet.size(), aWriter, __NBS_GlobalNamespaceType{});
		for (const value_type& entry : aSet)
		{
			size += Serialize(entry, aWriter, __NBS_GlobalNamespaceType{});
		}
		return size;
	}
	template<class _Kty, class _Pr, class _Alloc>
	size_t Deserialize(std::multiset<_Kty, _Pr, _Alloc>& outSet, Reader& aReader, size_t aAllocationLimit, __NBS_GlobalNamespaceType)
	{
		using value_type = typename std::multiset<_Kty, _Pr, _Alloc>::value_type;
		using size_type = typename std::multiset<_Kty, _Pr, _Alloc>::size_type;

		size_t size = 0;

		size_type containerSize = 0;
		size += Deserialize(containerSize, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
		for (size_type i = 0; i < containerSize; i++)
		{
			value_type entry = {};
			size += Deserialize(entry, aReader, aAllocationLimit - size, __NBS_GlobalNamespaceType{});
			outSet.insert(std::move(entry));
		}
		return size;
	}
}