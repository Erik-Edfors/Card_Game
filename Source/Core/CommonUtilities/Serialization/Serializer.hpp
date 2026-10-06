#pragma once
#include "BasicTypes.hpp"
#include "STLTypes.hpp"
#include <iostream>

#include "Writer.hpp"
#include "Reader.hpp"

namespace CommonUtilities
{
	class Serializer
	{
	public:
		Serializer();
		~Serializer() = default;

		template<class _Ty>
		bool Write(const _Ty& aValue, Writer& aWriter, size_t* outSize = nullptr);

		template<class _Ty>
		bool Read(_Ty& aValue, Reader& aReader, size_t aAllocationLimit, size_t* outSize = nullptr);

		const Exception& GetLastException() const noexcept { return *myLastException; }

	private:
		std::unique_ptr<Exception> myLastException;
	};

	inline Serializer::Serializer() :
		myLastException(std::make_unique<Exception>())
	{
	}

	template<class _Ty>
	inline bool Serializer::Write(const _Ty& aValue, Writer& aWriter, size_t* outSize)
	{
		size_t size = 0;

		try
		{
			size = Serialize(aValue, aWriter, __NBS_GlobalNamespaceType{});
		}
		catch (Exception e)
		{
			myLastException = std::make_unique<Exception>(e);
			return false;
		}

		if (outSize != nullptr)
		{
			*outSize = size;
		}

		return true;
	}
	template<class _Ty>
	inline bool Serializer::Read(_Ty& aValue, Reader& aReader, size_t aAllocationLimit, size_t* outSize)
	{
		size_t size = 0;

		try
		{
			size = Deserialize(aValue, aReader, aAllocationLimit, __NBS_GlobalNamespaceType{});
		}
		catch (Exception e)
		{
			myLastException = std::make_unique<Exception>(e);
			return false;
		}
		if (outSize != nullptr)
		{
			*outSize = size;
		}
		return true;
	}
}