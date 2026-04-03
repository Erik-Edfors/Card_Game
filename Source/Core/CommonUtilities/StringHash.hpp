#pragma once
#include "crc32.hpp"
#include <type_traits>
#include <string_view>
#include <string>
#include <bit>

namespace CommonUtilities
{
	namespace detail
	{
		template<size_t _Size>
		struct _MemoryArray
		{
			char Memory[_Size];
		};
	}

	template<class _CharTy>
	struct TStringHash
	{
	public:
		using Char = _CharTy;

		constexpr TStringHash() noexcept;
		constexpr TStringHash(const _CharTy* aString) noexcept;
		constexpr TStringHash(const nullptr_t) noexcept;
		constexpr TStringHash(const _CharTy* aString, size_t aLength) noexcept;
		constexpr TStringHash(const std::basic_string<_CharTy, std::char_traits<_CharTy>, std::allocator<_CharTy>>& aString) noexcept;
		constexpr TStringHash(std::basic_string_view<_CharTy> aStringView) noexcept;

		constexpr operator unsigned int() const noexcept;

		unsigned int Hash;
	};

	using StringHash = TStringHash<char>;
	using WStringHash = TStringHash<wchar_t>;

	template<class _CharTy>
	inline constexpr TStringHash<_CharTy>::TStringHash() noexcept :
		Hash(0u)
	{
	}

	template<class _CharTy>
	inline constexpr TStringHash<_CharTy>::TStringHash(const _CharTy* aString) noexcept :
		Hash(0u)
	{
		if (aString != nullptr)
		{
			const _CharTy* cursor = aString;
			while (*cursor != static_cast<_CharTy>(0))
			{
				detail::_MemoryArray<sizeof(_CharTy)> mem = std::bit_cast<detail::_MemoryArray<sizeof(_CharTy)>>(*cursor);
				for (size_t i = 0; i < sizeof(mem.Memory); i++)
				{
					Hash = CRC32Increment(mem.Memory[i], Hash);
				}
				++cursor;
			}
		}
	}

	template<class _CharTy>
	inline constexpr TStringHash<_CharTy>::TStringHash(const nullptr_t) noexcept :
		Hash(0u)
	{
	}

	template<class _CharTy>
	inline constexpr TStringHash<_CharTy>::TStringHash(const _CharTy* aString, size_t aLength) noexcept :
		Hash(0u)
	{
		for (size_t i = 0; i < aLength; i++)
		{
			detail::_MemoryArray<sizeof(_CharTy)> mem = std::bit_cast<detail::_MemoryArray<sizeof(_CharTy)>>(aString[i]);
			for (size_t j = 0; j < sizeof(mem.Memory); j++)
			{
				Hash = CRC32Increment(mem.Memory[j], Hash);
			}
		}
	}

	template<class _CharTy>
	inline constexpr TStringHash<_CharTy>::TStringHash(const std::basic_string<_CharTy, std::char_traits<_CharTy>, std::allocator<_CharTy>>& aString) noexcept :
		Hash(0u)
	{
		for (size_t i = 0; i < aString.length(); i++)
		{
			detail::_MemoryArray<sizeof(_CharTy)> mem = std::bit_cast<detail::_MemoryArray<sizeof(_CharTy)>>(aString[i]);
			for (size_t j = 0; j < sizeof(mem.Memory); j++)
			{
				Hash = CRC32Increment(mem.Memory[j], Hash);
			}
		}
	}

	template<class _CharTy>
	inline constexpr TStringHash<_CharTy>::TStringHash(std::basic_string_view<_CharTy> aStringView) noexcept :
		Hash(0u)
	{
		for (size_t i = 0; i < aStringView.length(); i++)
		{
			detail::_MemoryArray<sizeof(_CharTy)> mem = std::bit_cast<detail::_MemoryArray<sizeof(_CharTy)>>(aStringView[i]);
			for (size_t j = 0; j < sizeof(mem); j++)
			{
				Hash = CRC32Increment(mem.Memory[j], Hash);
			}
		}
	}

	template<class _CharTy>
	inline constexpr TStringHash<_CharTy>::operator unsigned int() const noexcept
	{
		return Hash;
	}

	template<class _CharTy>
	constexpr inline bool operator<(const TStringHash<_CharTy>& aLHS, const TStringHash<_CharTy>& aRHS) noexcept
	{
		return aLHS.Hash < aRHS.Hash;
	}
	template<class _CharTy>
	constexpr inline bool operator>(const TStringHash<_CharTy>& aLHS, const TStringHash<_CharTy>& aRHS) noexcept
	{
		return aLHS.Hash > aRHS.Hash;
	}
	template<class _CharTy>
	constexpr inline bool operator==(const TStringHash<_CharTy>& aLHS, const TStringHash<_CharTy>& aRHS) noexcept
	{
		return aLHS.Hash == aRHS.Hash;
	}
	template<class _CharTy>
	constexpr inline bool operator!=(const TStringHash<_CharTy>& aLHS, const TStringHash<_CharTy>& aRHS) noexcept
	{
		return aLHS.Hash != aRHS.Hash;
	}
}

inline constexpr CommonUtilities::TStringHash<char> operator"" _hash(const char* aString, size_t aLength)
{
	return CommonUtilities::TStringHash<char>(aString, aLength);
};
inline constexpr CommonUtilities::TStringHash<wchar_t> operator"" _hash(const wchar_t* aString, size_t aLength)
{
	return CommonUtilities::TStringHash<wchar_t>(aString, aLength);
};
inline constexpr CommonUtilities::TStringHash<char16_t> operator"" _hash(const char16_t* aString, size_t aLength)
{
	return CommonUtilities::TStringHash<char16_t>(aString, aLength);
};
inline constexpr CommonUtilities::TStringHash<char32_t> operator"" _hash(const char32_t* aString, size_t aLength)
{
	return CommonUtilities::TStringHash<char32_t>(aString, aLength);
};

template<class _CharTy>
struct ::std::hash<CommonUtilities::TStringHash<_CharTy>>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr std::size_t operator()(const CommonUtilities::TStringHash<_CharTy>& aStringHash) const noexcept
	{
		return static_cast<size_t>(aStringHash.Hash);
	}
};