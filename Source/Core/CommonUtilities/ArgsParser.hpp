#pragma once

namespace CommonUtilities
{
	namespace detail
	{
		template<typename _CharTy>
		struct ParserValues
		{
			typedef _CharTy Char;

			static const Char SeperatorChar;
			static const Char EndChar;
			static const Char FlagChar;
			static const Char OptionChar;
		};

		template<typename _CharTy>
		void SafeCopyString(const _CharTy* aSource, _CharTy** outDestination) noexcept;

		template<typename _CharTy>
		void SafeReleaseString(_CharTy** outString) noexcept;
	}

	template<typename _CharTy, typename _SizeTy = size_t>
	class ArgsParser;

	template<typename _CharTy>
	class Option
	{
		template<typename _1, typename _2>
		friend class ArgsParser;
	public:
		typedef _CharTy Char;

		~Option() noexcept;
		Option(const Option& aOption) noexcept;
		Option(Option&& aOption) noexcept;

		const Char* GetIdentity() const noexcept;
		const Char* GetValue() const noexcept;
		void Clean() noexcept;

		Option& operator=(Option&& aOption) noexcept;
		Option& operator=(const Option& aOption) noexcept;

	private:
		Option() noexcept;
		Option(const Char* aIdentity, const Char* aValue) noexcept;

		Char* myIdentity;
		Char* myValue;
	};

	template<typename _CharTy>
	class Argument
	{
		template<typename _1, typename _2>
		friend class ArgsParser;
	public:
		typedef _CharTy Char;

		~Argument() noexcept;
		Argument(const Argument& aArgument) noexcept;
		Argument(Argument&& aArgument) noexcept;

		const Char* GetValue() const noexcept;
		void Clean() noexcept;

		Argument& operator=(Argument&& aArgument) noexcept;
		Argument& operator=(const Argument& aArgument) noexcept;

	private:
		Argument() noexcept;
		Argument(const Char* aValue) noexcept;

		Char* myValue;
	};

	template<typename _CharTy>
	class Flag
	{
		template<typename _1, typename _2>
		friend class ArgsParser;
	public:
		typedef _CharTy Char;

		~Flag() noexcept;
		Flag(const Flag& aFlag) noexcept;
		Flag(Flag&& aFlag) noexcept;

		const Char* GetValue() const noexcept;
		void Clean() noexcept;

		Flag& operator=(Flag&& aFlag) noexcept;
		Flag& operator=(const Flag& aFlag) noexcept;

	private:
		Flag() noexcept;
		Flag(const Char* aFlag) noexcept;

		Char* myFlag;
	};

	template<typename _CharTy, typename _SizeTy>
	class ArgsParser
	{
	public:
		typedef _CharTy Char;
		typedef _SizeTy Size;
		typedef Argument<_CharTy> Argument;
		typedef Option<_CharTy> Option;
		typedef Flag<_CharTy> Flag;

		ArgsParser();
		~ArgsParser();
		ArgsParser(const ArgsParser& aArgsParser) noexcept;
		ArgsParser(ArgsParser&& aArgsParser) noexcept;

		bool Parse(const Char* const* aArgs, Size aNumArgs);

		void GetOptions(const Option** outOptions, Size* outNumOptions) noexcept;
		void GetArguments(const Argument** outArguments, Size* outNumArguments) noexcept;
		void GetFlags(const Flag** outFlags, Size* outNumFlags) noexcept;
		void Clean() noexcept;

		ArgsParser& operator=(const ArgsParser& aArgsParser) noexcept;
		ArgsParser& operator=(ArgsParser&& aArgsParser) noexcept;

	private:
		template<typename _ElementTy>
		void _assureArraySize(_ElementTy** outArray, Size aOldSize, Size aNewSize) noexcept;

		template<typename _ElementTy>
		void _copyArray(_ElementTy* aSource, Size aSourceSize, _ElementTy* aDestination);

		template<typename _ElementTy>
		void _moveArray(_ElementTy* aSource, Size aSourceSize, _ElementTy* aDestination);

		template<typename _ElementTy>
		void _pushBackElement(_ElementTy** outArray, Size* outSize, _ElementTy&& aElement) noexcept;

		Argument* myArguments;
		Size myNumArguments;

		Option* myOptions;
		Size myNumOptions;

		Flag* myFlags;
		Size myNumFlags;
	};

#pragma region detail

	namespace detail
	{
#pragma region ParserValues

		template<>
		const wchar_t ParserValues<wchar_t>::SeperatorChar = L'=';
		template<>
		const wchar_t ParserValues<wchar_t>::EndChar = L'\0';
		template<>
		const wchar_t ParserValues<wchar_t>::FlagChar = L'/';
		template<>
		const wchar_t ParserValues<wchar_t>::OptionChar = L'-';

		template<>
		const char ParserValues<char>::SeperatorChar = '=';
		template<>
		const char ParserValues<char>::EndChar = '\0';
		template<>
		const char ParserValues<char>::FlagChar = '/';
		template<>
		const char ParserValues<char>::OptionChar = '-';

#pragma endregion

		template<typename _CharTy>
		inline void SafeCopyString(const _CharTy* aSource, _CharTy** outDestination) noexcept
		{
			if (*outDestination != nullptr)
			{
				delete[] * outDestination;
				*outDestination = nullptr;
			}
			if (aSource == nullptr)
			{
				return;
			}
			size_t size = 0;
			while (aSource[size++] != ParserValues<_CharTy>::EndChar);

			*outDestination = new _CharTy[size];
			for (size_t i = 0; i < size; i++)
			{
				(*outDestination)[i] = aSource[i];
			}
		}

		template<typename _CharTy>
		inline void SafeReleaseString(_CharTy** outString) noexcept
		{
			if (*outString != nullptr)
			{
				delete[] * outString;
				*outString = nullptr;
			}
		}
	}

#pragma endregion

#pragma region ArgsParser

	template<typename _CharTy, typename _SizeTy>
	inline ArgsParser<_CharTy, _SizeTy>::ArgsParser() :
		myArguments(nullptr),
		myNumArguments(0),
		myOptions(nullptr),
		myNumOptions(0),
		myFlags(nullptr),
		myNumFlags(0)
	{
	}

	template<typename _CharTy, typename _SizeTy>
	inline ArgsParser<_CharTy, _SizeTy>::~ArgsParser()
	{
		Clean();
	}

	template<typename _CharTy, typename _SizeTy>
	inline ArgsParser<_CharTy, _SizeTy>::ArgsParser(const ArgsParser& aArgsParser) noexcept :
		ArgsParser()
	{
		*this = aArgsParser;
	}

	template<typename _CharTy, typename _SizeTy>
	inline ArgsParser<_CharTy, _SizeTy>::ArgsParser(ArgsParser&& aArgsParser) noexcept :
		ArgsParser()
	{
		*this = static_cast<ArgsParser&&>(aArgsParser);
	}

	template<typename _CharTy, typename _SizeTy>
	inline bool ArgsParser<_CharTy, _SizeTy>::Parse(const Char* const* aArgs, Size aNumArgs)
	{
		constexpr Size charBufferSize = 4096;
		const Char seperator = detail::ParserValues<Char>::SeperatorChar;
		const Char end = detail::ParserValues<Char>::EndChar;
		const Char flag = detail::ParserValues<Char>::FlagChar;
		const Char option = detail::ParserValues<Char>::OptionChar;

		Clean();
		if (aArgs == nullptr)
		{
			return false;
		}

		Char charBuffer[charBufferSize] = {};

		const Char* argument = nullptr;
		for (Size i = 0; i < aNumArgs; i++)
		{
			argument = aArgs[i];
			if (argument == nullptr)
			{
				return false;
			}

			Size cursor = 0;
			Size firstSize = 0;
			Size secondSize = 0;
			Size* argumentSizePtr = &firstSize;
			bool isFlag = argument[0] == flag;
			bool isOption = false;
			do
			{
				++*argumentSizePtr;
				if (cursor >= charBufferSize)
				{
					return false;
				}

				if (cursor == 1 && argument[0] == option && argument[1] == option)
				{
					isOption = true;
				}

				if (isOption && argument[cursor] == seperator && argumentSizePtr != &secondSize)
				{
					charBuffer[cursor] = end;
					argumentSizePtr = &secondSize;
				}
				else
				{
					charBuffer[cursor] = argument[cursor];
				}

				++cursor;
			} while (argument[cursor] != end);
			charBuffer[cursor] = end;

			if (firstSize != 0)
			{
				if (isFlag)
				{
					_pushBackElement<Flag>(&myFlags, &myNumFlags, Flag(&charBuffer[1]));
				}
				else if (isOption)
				{
					if (secondSize == 0)
					{
						_pushBackElement<Option>(&myOptions, &myNumOptions, Option(&charBuffer[2], nullptr));
					}
					else
					{
						_pushBackElement<Option>(&myOptions, &myNumOptions, Option(&charBuffer[2], &charBuffer[firstSize]));
					}
				}
				else
				{
					_pushBackElement<Argument>(&myArguments, &myNumArguments, Argument(&charBuffer[0]));
				}
			}
		}

		return true;
	}
	template<typename _CharTy, typename _SizeTy>
	inline void ArgsParser<_CharTy, _SizeTy>::GetOptions(const Option** outOptions, Size* outNumOptions) noexcept
	{
		*outOptions = myOptions;
		*outNumOptions = myNumOptions;
	}
	template<typename _CharTy, typename _SizeTy>
	inline void ArgsParser<_CharTy, _SizeTy>::GetArguments(const Argument** outArguments, Size* outNumArguments) noexcept
	{
		*outArguments = myArguments;
		*outNumArguments = myNumArguments;
	}
	template<typename _CharTy, typename _SizeTy>
	inline void ArgsParser<_CharTy, _SizeTy>::GetFlags(const Flag** outFlags, Size* outNumFlags) noexcept
	{
		*outFlags = myFlags;
		*outNumFlags = myNumFlags;
	}
	template<typename _CharTy, typename _SizeTy>
	inline void ArgsParser<_CharTy, _SizeTy>::Clean() noexcept
	{
		if (myArguments != nullptr)
		{
			delete[] myArguments;
			myArguments = nullptr;
		}
		if (myOptions != nullptr)
		{
			delete[] myOptions;
			myOptions = nullptr;
		}
		if (myFlags != nullptr)
		{
			delete[] myFlags;
			myFlags = nullptr;
		}

		myNumArguments = 0;
		myNumOptions = 0;
		myNumFlags = 0;
	}
	template<typename _CharTy, typename _SizeTy>
	inline ArgsParser<_CharTy, _SizeTy>& ArgsParser<_CharTy, _SizeTy>::operator=(const ArgsParser& aArgsParser) noexcept
	{
		Clean();

		_assureArraySize<Argument>(&myArguments, myNumArguments, aArgsParser.myNumArguments);
		_assureArraySize<Option>(&myOptions, myNumOptions, aArgsParser.myNumOptions);
		_assureArraySize<Flag>(&myFlags, myNumFlags, aArgsParser.myNumFlags);

		_copyArray<Argument>(aArgsParser.myArguments, aArgsParser.myNumArguments, myArguments);
		myNumArguments = aArgsParser.myNumArguments;

		_copyArray<Option>(aArgsParser.myOptions, aArgsParser.myNumOptions, myOptions);
		myNumOptions = aArgsParser.myNumOptions;

		_copyArray<Flag>(aArgsParser.myFlags, aArgsParser.myNumFlags, myFlags);
		myNumFlags = aArgsParser.myNumFlags;
		return *this;
	}
	template<typename _CharTy, typename _SizeTy>
	inline ArgsParser<_CharTy, _SizeTy>& ArgsParser<_CharTy, _SizeTy>::operator=(ArgsParser&& aArgsParser) noexcept
	{
		Clean();

		myArguments = aArgsParser.myArguments;
		myOptions = aArgsParser.myOptions;
		myFlags = aArgsParser.myFlags;

		myNumArguments = aArgsParser.myNumArguments;
		myNumOptions = aArgsParser.myNumOptions;
		myNumFlags = aArgsParser.myNumFlags;

		aArgsParser.myArguments = nullptr;
		aArgsParser.myOptions = nullptr;
		aArgsParser.myFlags = nullptr;

		aArgsParser.myNumArguments = 0;
		aArgsParser.myNumOptions = 0;
		aArgsParser.myNumFlags = 0;
		return *this;
	}
	template<typename _CharTy, typename _SizeTy>
	template<typename _ElementTy>
	inline void ArgsParser<_CharTy, _SizeTy>::_assureArraySize(_ElementTy** outArray, Size aOldSize, Size aNewSize) noexcept
	{
		if (*outArray == nullptr)
		{
			*outArray = new _ElementTy[aNewSize];
			return;
		}

		_ElementTy* newArray = new _ElementTy[aNewSize];
		_copyArray<_ElementTy>(*outArray, aOldSize, newArray);
		delete[] * outArray;
		*outArray = newArray;
	}
	template<typename _CharTy, typename _SizeTy>
	template<typename _ElementTy>
	inline void ArgsParser<_CharTy, _SizeTy>::_copyArray(_ElementTy* aSource, Size aSourceSize, _ElementTy* aDestination)
	{
		for (Size i = 0; i < aSourceSize; i++)
		{
			aDestination[i] = aSource[i];
		}
	}
	template<typename _CharTy, typename _SizeTy>
	template<typename _ElementTy>
	inline void ArgsParser<_CharTy, _SizeTy>::_moveArray(_ElementTy* aSource, Size aSourceSize, _ElementTy* aDestination)
	{
		for (Size i = 0; i < aSourceSize; i++)
		{
			aDestination[i] = static_cast<_ElementTy&&>(aSource[i]);
		}
	}
	template<typename _CharTy, typename _SizeTy>
	template<typename _ElementTy>
	inline void ArgsParser<_CharTy, _SizeTy>::_pushBackElement(_ElementTy** outArray, Size* outSize, _ElementTy&& aElement) noexcept
	{
		_assureArraySize<_ElementTy>(outArray, *outSize, *outSize + 1);
		(*outArray)[*outSize] = aElement;
		++*outSize;
	}

#pragma endregion

#pragma region Flag

	template<typename _CharTy>
	inline Flag<_CharTy>::Flag() noexcept :
		myFlag(nullptr)
	{
	}
	template<typename _CharTy>
	inline Flag<_CharTy>::Flag(const Char* aFlag) noexcept :
		Flag()
	{
		detail::SafeCopyString<Char>(aFlag, &myFlag);
	}
	template<typename _CharTy>
	inline Flag<_CharTy>::~Flag() noexcept
	{
		Clean();
	}
	template<typename _CharTy>
	inline Flag<_CharTy>::Flag(const Flag& aFlag) noexcept :
		Flag()
	{
		*this = aFlag;
	}
	template<typename _CharTy>
	inline Flag<_CharTy>::Flag(Flag&& aFlag) noexcept :
		Flag()
	{
		*this = static_cast<Flag&&>(aFlag);
	}

	template<typename _CharTy>
	inline const typename Flag<_CharTy>::Char* Flag<_CharTy>::GetValue() const noexcept
	{
		return myFlag;
	}

	template<typename _CharTy>
	inline void Flag<_CharTy>::Clean() noexcept
	{
		detail::SafeReleaseString<Char>(&myFlag);
	}

	template<typename _CharTy>
	inline Flag<_CharTy>& Flag<_CharTy>::operator=(Flag&& aFlag) noexcept
	{
		Clean();

		myFlag = aFlag.myFlag;
		aFlag.myFlag = nullptr;
	}
	template<typename _CharTy>
	inline Flag<_CharTy>& Flag<_CharTy>::operator=(const Flag& aFlag) noexcept
	{
		detail::SafeCopyString<Char>(aFlag.myFlag, &myFlag);
		return *this;
	}

#pragma endregion

#pragma region Argument

	template<typename _CharTy>
	inline Argument<_CharTy>::Argument() noexcept :
		myValue(nullptr)
	{
	}
	template<typename _CharTy>
	inline Argument<_CharTy>::Argument(const Char* aValue) noexcept :
		Argument()
	{
		detail::SafeCopyString<Char>(aValue, &myValue);
	}
	template<typename _CharTy>
	inline Argument<_CharTy>::~Argument() noexcept
	{
		Clean();
	}
	template<typename _CharTy>
	inline Argument<_CharTy>::Argument(const Argument& aArgument) noexcept :
		Argument()
	{
		*this = aArgument;
	}
	template<typename _CharTy>
	inline Argument<_CharTy>::Argument(Argument&& aArgument) noexcept :
		Argument()
	{
		*this = static_cast<Argument&&>(aArgument);
	}

	template<typename _CharTy>
	inline const typename Argument<_CharTy>::Char* Argument<_CharTy>::GetValue() const noexcept
	{
		return myValue;
	}
	template<typename _CharTy>
	inline void Argument<_CharTy>::Clean() noexcept
	{
		detail::SafeReleaseString<Char>(&myValue);
	}

	template<typename _CharTy>
	inline Argument<_CharTy>& Argument<_CharTy>::operator=(Argument&& aArgument) noexcept
	{
		Clean();

		myValue = aArgument.myValue;
		aArgument.myValue = nullptr;
		return *this;
	}

	template<typename _CharTy>
	inline Argument<_CharTy>& Argument<_CharTy>::operator=(const Argument& aArgument) noexcept
	{
		detail::SafeCopyString<Char>(aArgument.myValue, &myValue);
		return *this;
	}

#pragma endregion

#pragma region Option

	template<typename _CharTy>
	inline Option<_CharTy>::Option() noexcept :
		myIdentity(nullptr),
		myValue(nullptr)
	{
	}
	template<typename _CharTy>
	inline Option<_CharTy>::Option(const Char* aIdentity, const Char* aValue) noexcept :
		Option()
	{
		detail::SafeCopyString<Char>(aIdentity, &myIdentity);
		detail::SafeCopyString<Char>(aValue, &myValue);
	}
	template<typename _CharTy>
	inline Option<_CharTy>::~Option() noexcept
	{
		Clean();
	}
	template<typename _CharTy>
	inline Option<_CharTy>::Option(const Option& aOption) noexcept :
		Option()
	{
		*this = aOption;
	}
	template<typename _CharTy>
	inline Option<_CharTy>::Option(Option&& aOption) noexcept :
		Option()
	{
		*this = static_cast<Option&&>(aOption);
	}

	template<typename _CharTy>
	inline const typename Option<_CharTy>::Char* Option<_CharTy>::GetIdentity() const noexcept
	{
		return myIdentity;
	}
	template<typename _CharTy>
	inline const typename Option<_CharTy>::Char* Option<_CharTy>::GetValue() const noexcept
	{
		return myValue;
	}
	template<typename _CharTy>
	inline void Option<_CharTy>::Clean() noexcept
	{
		detail::SafeReleaseString<Char>(&myIdentity);
		detail::SafeReleaseString<Char>(&myValue);
	}

	template<typename _CharTy>
	inline Option<_CharTy>& Option<_CharTy>::operator=(Option&& aOption) noexcept
	{
		myIdentity = aOption.myIdentity;
		myValue = aOption.myValue;

		aOption.myIdentity = nullptr;
		aOption.myValue = nullptr;
		return *this;
	}
	template<typename _CharTy>
	inline Option<_CharTy>& Option<_CharTy>::operator=(const Option& aOption) noexcept
	{
		detail::SafeCopyString(aOption.myIdentity, &myIdentity);
		detail::SafeCopyString(aOption.myValue, &myValue);
		return *this;
	}

#pragma endregion
}