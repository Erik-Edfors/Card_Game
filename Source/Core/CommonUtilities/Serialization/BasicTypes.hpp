#pragma once
#include "Exceptions.hpp"
#include "Writer.hpp"
#include "Reader.hpp"

#include <ostream>
#include <istream>
#include <type_traits>
#include <format>

// Type used to assure the global namespace is associated when evaluating templated functions.
struct __NBS_GlobalNamespaceType {};

namespace CommonUtilities
{
	template<typename _Ty, size_t _Size>
	using CStyleArray = _Ty(&)[_Size];

	template<class _Ty, size_t _Size> requires std::is_trivially_copyable_v<_Ty>
	size_t Serialize(const CStyleArray<_Ty, _Size>& aArray, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template<class _Ty, size_t _Size> requires std::is_trivially_copyable_v<_Ty>
	size_t Deserialize(CStyleArray<_Ty, _Size>& outArray, Reader& aReader, __NBS_GlobalNamespaceType = {});

	template<class _Ty>
		requires std::is_trivially_copyable_v<_Ty>
	size_t Serialize(const _Ty& aValue, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template<class _Ty>
		requires std::is_trivially_copyable_v<_Ty>
	size_t Deserialize(_Ty& outValue, Reader& aReader, __NBS_GlobalNamespaceType = {});

	template<class _Ty, size_t _Size>
		requires(not std::is_trivially_copyable_v<_Ty>)
	size_t Serialize(const CStyleArray<_Ty, _Size>& aArray, Writer& aWriter, __NBS_GlobalNamespaceType = {});
	template<class _Ty, size_t _Size>
		requires(not std::is_trivially_copyable_v<_Ty>)
	size_t Deserialize(CStyleArray<_Ty, _Size>& outArray, Reader& aReader, __NBS_GlobalNamespaceType = {});



	template<class _Ty>
		requires std::is_trivially_copyable_v<_Ty>
	size_t Serialize(const _Ty& aValue, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		constexpr size_t dataSize = sizeof(aValue);

		size_t size = 0;

		if (not aWriter.Writable())
		{
			throw Exception("Writer was not writable!");
		}

		size_t count = aWriter.Write(reinterpret_cast<const char*>(&aValue), dataSize);
		if (count != dataSize)
		{
			throw WriteException("Writer failed to write!");
		}
		size += count;

		return size;
	}
	template<class _Ty>
		requires std::is_trivially_copyable_v<_Ty>
	size_t Deserialize(_Ty& outValue, Reader& aReader, __NBS_GlobalNamespaceType)
	{
		constexpr size_t dataSize = sizeof(outValue);

		size_t size = 0;

		if (not aReader.Readable())
		{
			throw Exception("Reader was not readable!");
		}

		size_t count = aReader.Read(reinterpret_cast<char*>(&outValue), dataSize);
		if (count != dataSize)
		{
			throw ReadException("Reader failed to read!");
		}
		size += count;

		return size;
	}



	template<class _Ty, size_t _Size>
		requires std::is_trivially_copyable_v<_Ty>
	size_t Serialize(const CStyleArray<_Ty, _Size>& aArray, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		constexpr size_t dataSize = sizeof(aArray);

		size_t size = 0;
		if (not aWriter.Writable())
		{
			throw Exception("Writer was not writable!");
		}

		size_t count = aWriter.Write(reinterpret_cast<const char*>(aArray), dataSize);
		if (count != dataSize)
		{
			throw WriteException("Writer failed to write!");
		}
		size += count;

		return size;
	}
	template<class _Ty, size_t _Size>
		requires std::is_trivially_copyable_v<_Ty>
	size_t Deserialize(CStyleArray<_Ty, _Size>& outArray, Reader& aReader, __NBS_GlobalNamespaceType)
	{
		constexpr size_t dataSize = sizeof(outArray);

		size_t size = 0;
		if (not aReader.Readable())
		{
			throw Exception("Reader was not readable!");
		}

		size_t count = aReader.Read(reinterpret_cast<char*>(outArray), dataSize);
		if (count != dataSize)
		{
			throw ReadException("Reader failed to read!");
		}
		size += count;

		return size;
	}



	template<class _Ty, size_t _Size>
		requires(not std::is_trivially_copyable_v<_Ty>)
	size_t Serialize(const CStyleArray<_Ty, _Size>& aArray, Writer& aWriter, __NBS_GlobalNamespaceType)
	{
		size_t size = 0;

		if (not aWriter.Writable())
		{
			throw Exception("Writer was not writable!");
		}

		for (size_t i = 0; i < _Size; i++)
		{
			size += Serialize(aArray[i], aWriter);
		}

		return size;
	}
	template<class _Ty, size_t _Size>
		requires(not std::is_trivially_copyable_v<_Ty>)
	size_t Deserialize(CStyleArray<_Ty, _Size>& outArray, Reader& aReader, __NBS_GlobalNamespaceType)
	{
		size_t size = 0;

		if (not aReader.Readable())
		{
			throw ReadException("Reader was not readable!");
		}

		for (size_t i = 0; i < _Size; i++)
		{
			size += Deserialize(outArray[i], aReader);
		}

		return size;
	}
}