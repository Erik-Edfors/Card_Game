#pragma once
#include "Random.hpp"

namespace CommonUtilities
{
	namespace detail
	{
		constexpr bool IsHexTable[256]{
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false,

			// Numbers 0-9
			true,
			true,
			true,
			true,
			true,
			true,
			true,
			true,
			true,
			true,

			false, false, false, false, false, false, false,

			// Letters A-F
			true,
			true,
			true,
			true,
			true,
			true,

			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false,

			//Letters a-f
			true,
			true,
			true,
			true,
			true,
			true,

			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false, false, false, false, false, false, false, false,
			false, false, false
		};

		constexpr uint8_t ToHexTable[256]{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,

			// Numbers 0-9
			0,
			1,
			2,
			3,
			4,
			5,
			6,
			7,
			8,
			9,

			0, 0, 0, 0, 0, 0, 0,

			// Letters A-F
			10,
			11,
			12,
			13,
			14,
			15,

			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0,

			//Letters a-f
			10,
			11,
			12,
			13,
			14,
			15,

			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0
		};

		constexpr char HexToCharTable[16]{
			'0',
			'1',
			'2',
			'3',
			'4',
			'5',
			'6',
			'7',
			'8',
			'9',
			'a',
			'b',
			'c',
			'd',
			'e',
			'f'
		};
	}

	class UUID
	{
	public:
		typedef uint8_t data_type;
		static constexpr size_t data_size = 16 / sizeof(data_type);

		static UUID GenV4();

		constexpr UUID() noexcept;
		constexpr UUID(const char* aString) noexcept;

		_CONSTEXPR20 std::string ToString() const noexcept;

		constexpr data_type GetVersion() const noexcept;
		constexpr data_type GetVariant() const noexcept;

		constexpr bool IsNull() const noexcept;

		constexpr data_type* AccessDataNetworkOrdered(size_t aIndex) noexcept;
		constexpr const data_type* AccessDataNetworkOrdered(size_t aIndex) const noexcept;

		constexpr bool operator<(const UUID& aRHS) const noexcept;
		constexpr bool operator>(const UUID& aRHS) const noexcept;
		constexpr bool operator==(const UUID& aRHS) const noexcept;
		constexpr bool operator!=(const UUID& aRHS) const noexcept;

		union
		{
			data_type Data[data_size];
			struct _Fields
			{
				uint32_t time_low;
				uint16_t time_mid;
				uint16_t time_hi_and_version;
				uint8_t clock_seq_hi_and_reserved;
				uint8_t clock_seq_low;
				uint8_t node[6];
			} Fields;
		};

	private:
		inline static std::unique_ptr<std::mt19937> ourRNG = nullptr;

		constexpr void _assignData(const char* aString) noexcept;
	};

	inline UUID UUID::GenV4()
	{
		if (ourRNG == nullptr)
		{
			std::random_device rnd;
			ourRNG = std::make_unique<std::mt19937>();
			Seed(*ourRNG, std::ref(rnd));
		}
		UUID uuid = {};

		constexpr size_t sizeFactor = sizeof(std::mt19937::result_type) / sizeof(data_type);

		for (size_t i = 0; i < sizeof(uuid.Data) / sizeFactor; i++)
		{
			std::mt19937::result_type randomNum = (*ourRNG)();

			for (size_t j = 0; j < sizeFactor; j++)
			{
				uuid.Data[i * sizeFactor + j] = static_cast<data_type>(randomNum >> (j * sizeof(data_type) * 8));
			}
		}

		*uuid.AccessDataNetworkOrdered(6) = (*uuid.AccessDataNetworkOrdered(6) & 0xf) | (4 << 4);
		*uuid.AccessDataNetworkOrdered(8) = (*uuid.AccessDataNetworkOrdered(8) & 0x3f) | (0b10 << 6);

		return uuid;
	}

	inline constexpr UUID::UUID() noexcept
	{
		for (size_t i = 0; i < data_size; i++)
		{
			Data[i] = {};
		}
	}

	inline constexpr UUID::UUID(const char* aString) noexcept
	{
		for (size_t i = 0; i < data_size; i++)
		{
			Data[i] = {};
		}
		if (aString != nullptr)
		{
			if (*aString == '0' && *(aString + 1) == 'x')
			{
				_assignData(aString + 2);
			}
			else
			{
				_assignData(aString);
			}
		}
	}

	_CONSTEXPR20 std::string UUID::ToString() const noexcept
	{
		std::string str(36, '-');

		constexpr uint8_t higher = static_cast<uint8_t>(0xf << 4);
		constexpr uint8_t lower = static_cast<uint8_t>(0xf);

		for (size_t i = 0; i < 4; i++)
		{
			const data_type value = *AccessDataNetworkOrdered(i);
			const char high = detail::HexToCharTable[(value & higher) >> 4];
			const char low = detail::HexToCharTable[value & lower];
			str[2 * i] = high;
			str[2 * i + 1] = low;
		}

		for (size_t j = 0; j < 3; j++)
		{
			for (size_t i = 4 + j * 2; i < 6 + j * 2; i++)
			{
				const data_type value = *AccessDataNetworkOrdered(i);
				const char high = detail::HexToCharTable[(value & higher) >> 4];
				const char low = detail::HexToCharTable[value & lower];
				str[2 * i + j + 1] = high;
				str[2 * i + j + 1 + 1] = low;
			}
		}

		for (size_t i = 10; i < 16; i++)
		{
			const data_type value = *AccessDataNetworkOrdered(i);
			const char high = detail::HexToCharTable[(value & higher) >> 4];
			const char low = detail::HexToCharTable[value & lower];
			str[2 * i + 4] = high;
			str[2 * i + 1 + 4] = low;
		}

		return str;
	}

	inline constexpr typename UUID::data_type UUID::GetVersion() const noexcept
	{
		return (*AccessDataNetworkOrdered(6) & (0xf << 4)) >> 4;
	}

	inline constexpr typename UUID::data_type UUID::GetVariant() const noexcept
	{
		if ((*AccessDataNetworkOrdered(8) & (0b11 << 6)) >> 6 == 0b10)
		{
			return static_cast<data_type>(1);
		}
		else if ((*AccessDataNetworkOrdered(8) & (0b111 << 5)) >> 5 == 0b110)
		{
			return static_cast<data_type>(2);
		}
		return static_cast<data_type>(0);
	}

	inline constexpr bool UUID::IsNull() const noexcept
	{
		for (size_t i = 0; i < data_size; i++)
		{
			if (Data[i] != 0)
			{
				return false;
			}
		}
		return true;
	}

	inline constexpr typename UUID::data_type* UUID::AccessDataNetworkOrdered(size_t aIndex) noexcept
	{
		if (aIndex >= data_size)
		{
			return nullptr;
		}
		constexpr size_t indexTable[16] = {
			3, 2, 1, 0, 5, 4, 7, 6, 8, 9, 10, 11, 12, 13, 14, 15
		};

		return &Data[indexTable[aIndex]];
	}

	inline constexpr typename const UUID::data_type* UUID::AccessDataNetworkOrdered(size_t aIndex) const noexcept
	{
		if (aIndex >= data_size)
		{
			return nullptr;
		}
		constexpr size_t indexTable[16] = {
			3, 2, 1, 0, 5, 4, 7, 6, 8, 9, 10, 11, 12, 13, 14, 15
		};

		return &Data[indexTable[aIndex]];
	}

	inline constexpr bool UUID::operator<(const UUID& aRHS) const noexcept
	{
		if (Fields.time_hi_and_version < aRHS.Fields.time_hi_and_version)
		{
			return true;
		}
		else if (Fields.time_hi_and_version > aRHS.Fields.time_hi_and_version)
		{
			return false;
		}

		if (Fields.time_mid < aRHS.Fields.time_mid)
		{
			return true;
		}
		else if (Fields.time_mid > aRHS.Fields.time_mid)
		{
			return false;
		}

		if (Fields.time_low < aRHS.Fields.time_low)
		{
			return true;
		}
		else if (Fields.time_low > aRHS.Fields.time_low)
		{
			return false;
		}

		if (Fields.clock_seq_hi_and_reserved < aRHS.Fields.clock_seq_hi_and_reserved)
		{
			return true;
		}
		else if (Fields.clock_seq_hi_and_reserved > aRHS.Fields.clock_seq_hi_and_reserved)
		{
			return false;
		}

		if (Fields.clock_seq_low < aRHS.Fields.clock_seq_low)
		{
			return true;
		}
		else if (Fields.clock_seq_low > aRHS.Fields.clock_seq_low)
		{
			return false;
		}

		for (size_t i = 0; i < 6; i++)
		{
			if (Fields.node[i] < aRHS.Fields.node[i])
			{
				return true;
			}
			else if (Fields.node[i] > aRHS.Fields.node[i])
			{
				return false;
			}
		}
		return false;
	}

	inline constexpr bool UUID::operator>(const UUID& aRHS) const noexcept
	{
		if (Fields.time_hi_and_version > aRHS.Fields.time_hi_and_version)
		{
			return true;
		}
		else if (Fields.time_hi_and_version < aRHS.Fields.time_hi_and_version)
		{
			return false;
		}

		if (Fields.time_mid > aRHS.Fields.time_mid)
		{
			return true;
		}
		else if (Fields.time_mid < aRHS.Fields.time_mid)
		{
			return false;
		}

		if (Fields.time_low > aRHS.Fields.time_low)
		{
			return true;
		}
		else if (Fields.time_low < aRHS.Fields.time_low)
		{
			return false;
		}

		if (Fields.clock_seq_hi_and_reserved > aRHS.Fields.clock_seq_hi_and_reserved)
		{
			return true;
		}
		else if (Fields.clock_seq_hi_and_reserved < aRHS.Fields.clock_seq_hi_and_reserved)
		{
			return false;
		}

		if (Fields.clock_seq_low > aRHS.Fields.clock_seq_low)
		{
			return true;
		}
		else if (Fields.clock_seq_low < aRHS.Fields.clock_seq_low)
		{
			return false;
		}

		for (size_t i = 0; i < 6; i++)
		{
			if (Fields.node[i] > aRHS.Fields.node[i])
			{
				return true;
			}
			else if (Fields.node[i] < aRHS.Fields.node[i])
			{
				return false;
			}
		}
		return false;
	}

	inline constexpr bool UUID::operator==(const UUID& aRHS) const noexcept
	{
		for (int i = static_cast<int>(data_size) - 1; i >= 0; i--)
		{
			if (Data[i] != aRHS.Data[i])
			{
				return false;
			}
		}
		return true;
	}

	inline constexpr bool UUID::operator!=(const UUID& aRHS) const noexcept
	{
		for (int i = static_cast<int>(data_size) - 1; i >= 0; i--)
		{
			if (Data[i] != aRHS.Data[i])
			{
				return true;
			}
		}
		return false;
	}

	inline constexpr void UUID::_assignData(const char* aString) noexcept
	{
		const char* cursor = aString;

		for (size_t i = 0; i < data_size * 2; i++)
		{
			size_t dataInd = i / 2;
			bool significant = i % 2 == 0;

			while (not detail::IsHexTable[static_cast<uint8_t>(*cursor)])
			{
				if (*cursor == '\0')
				{
					return;
				}
				++cursor;
			}
			const unsigned char value = static_cast<uint8_t>(*cursor++);

			*AccessDataNetworkOrdered(dataInd) |= (detail::ToHexTable[value] << (significant ? 4 : 0));
		}
	}
}

inline ::std::ostream& operator<<(std::ostream& aStream, const CommonUtilities::UUID& aUUID)
{
	char format[37] = "********-****-****-****-************";

	char* formatCursor = format;
	size_t dataInd = 0;
	while (dataInd < CommonUtilities::UUID::data_size)
	{
		while (*formatCursor != '*')
		{
			if (*formatCursor == '\0')
			{
				goto formatting_end;
			}
			formatCursor++;
		}

		*formatCursor = CommonUtilities::detail::HexToCharTable[((*aUUID.AccessDataNetworkOrdered(dataInd) & (0xf << 4)) >> 4)];
		formatCursor++;

		while (*formatCursor != '*')
		{
			if (*formatCursor == '\0')
			{
				goto formatting_end;
			}
			formatCursor++;
		}
		*formatCursor = CommonUtilities::detail::HexToCharTable[(*aUUID.AccessDataNetworkOrdered(dataInd) & 0xf)];
		formatCursor++;

		dataInd++;
	}

formatting_end:
	aStream << format;
	return aStream;
}

template<>
struct ::std::hash<CommonUtilities::UUID>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr std::size_t operator()(const CommonUtilities::UUID& aUUID) const noexcept
	{
		constexpr size_t sizeFactor = sizeof(size_t) / sizeof(CommonUtilities::UUID::data_type);
		size_t hash = 0;
		for (size_t i = 0; i < CommonUtilities::UUID::data_size; i++)
		{
			size_t index = i % sizeFactor;
			hash ^= static_cast<size_t>(aUUID.Data[i]) << (index * sizeof(CommonUtilities::UUID::data_type));
		}
		return hash;
	}
};