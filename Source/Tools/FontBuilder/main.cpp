#include <Core/CommonUtilities/ArgsParser.hpp>
#include <Core/CommonUtilities/StringHash.hpp>
#include <iostream>

#include "FileIteration.h"
#include "FontBuilder.h"

int wmain(int argc, wchar_t** argv)
{
	FontBuilder builder;
	if (not builder.Initialize())
	{
		std::wcout << "Failed to initialize FontBuilder!" << std::endl;
		return -1;
	}

	CommonUtilities::ArgsParser<wchar_t> parser;
	if (not parser.Parse(argv, static_cast<size_t>(argc)))
	{
		std::wcout << "Failed to parse arguments!" << std::endl;
		return -1;
	}

	const CommonUtilities::Argument<wchar_t>* arguments;
	size_t numArguments;

	const CommonUtilities::Option<wchar_t>* options;
	size_t numOptions;

	const CommonUtilities::Flag<wchar_t>* flags;
	size_t numFlags;

	parser.GetArguments(&arguments, &numArguments);
	parser.GetOptions(&options, &numOptions);
	parser.GetFlags(&flags, &numFlags);

	std::filesystem::path path;
	std::filesystem::path target;

	bool recursive = false;
	bool print = false;
	bool checkDate = false;
	bool useRegex = false;

	unsigned int fontSize = 24;

	std::wregex regex;

	if (numArguments < 2 || numArguments > 3)
	{
		std::wcout << "Invalid number of arguments!" << std::endl;
		return -1;
	}

	path = arguments[1].GetValue();

	if (numArguments > 2)
	{
		target = arguments[2].GetValue();
	}

	path = path.lexically_normal();
	target = target.lexically_normal();

	for (size_t i = 0; i < numFlags; i++)
	{
		const wchar_t* cursor = flags[i].GetValue();
		while (*cursor != L'\0')
		{
			switch (*cursor)
			{
				case L'r': recursive = true; break;
				case L'p': print = true; break;
				case L'd': checkDate = true; break;
			}
			++cursor;
		}
	}

	for (size_t i = 0; i < numOptions; i++)
	{
		const wchar_t* identity = options[i].GetIdentity();
		const wchar_t* value = options[i].GetValue();
		switch (CommonUtilities::WStringHash(identity))
		{
			case L"regex"_hash:
				regex = std::wregex(value, std::wregex::icase);
				useRegex = true;
				break;
			case L"size"_hash:
				try
				{
					fontSize = std::stoi(value);
				}
				catch (std::exception e)
				{
					std::wcout << "Invalid option value! --" << identity << "=" << value << std::endl;
					return -1;
				}
				break;
		}
	}

	std::wregex* regexPtr = nullptr;
	if (useRegex)
	{
		regexPtr = &regex;
	}

	struct _userData
	{
		bool Print;
		bool CheckDate;
		std::filesystem::path Root;
		std::filesystem::path Target;
		FontBuilder* FontBuilder;
		unsigned int FontSize;
	} userData = {
		.Print = print,
		.CheckDate = checkDate,
		.Root = path,
		.Target = target.empty() ? std::filesystem::path(path).remove_filename() : target,
		.FontBuilder = &builder,
		.FontSize = fontSize
	};

	IterateFiles(
		path,
		regexPtr,
		recursive,
		[](const std::filesystem::path& aPath, void* aUserData) {
			const _userData& data = *reinterpret_cast<_userData*>(aUserData);

			std::filesystem::path target = data.Target / aPath.lexically_relative(data.Root);
			target.replace_extension(".bmp");

			constexpr const wchar_t glyphs[] = {
				L'!', L'"', L'#', L'$', L'%', L'&', L'\'', L'(', L')', L'*', L'+', L',', L'-', L'/',
				L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L':', L';', L'<', L'=',
				L'>', L'?', L'@', L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K',
				L'L', L'M', L'N', L'O', L'P', L'Q', L'R', L'S', L'T', L'U', L'V', L'W', L'X', L'Y',
				L'Z', L'[', L'\\', L']', L'^', L'_', L'`', L'a', L'b', L'c', L'd', L'e', L'f', L'g',
				L'h', L'i', L'j', L'k', L'l', L'm', L'n', L'o', L'p', L'q', L'r', L's', L't', L'u',
				L'v', L'w', L'x', L'y', L'z', L'{', L'|', L'}', L'~', L'€', L'‚', L'ƒ', L'„', L'…',
				L'†', L'‡', L'ˆ', L'‰', L'Š', L'‹', L'Œ', L'U', L'Ž', L'U', L'U', L'‘', L'’', L'“',
				L'”', L'•', L'–', L'—', L'˜', L'™', L'š', L'›', L'œ', L'U', L'ž', L'Ÿ', L'N', L'¡',
				L'¢', L'£', L'¤', L'¥', L'¦', L'§', L'¨', L'©', L'ª', L'«', L'¬', L'S', L'®', L'¯',
				L'°', L'±', L'²', L'³', L'´', L'µ', L'¶', L'·', L'¸', L'¹', L'º', L'»', L'¼', L'½',
				L'¾', L'¿', L'À', L'Á', L'Â', L'Ã', L'Ä', L'Å', L'Æ', L'Ç', L'È', L'É', L'Ê', L'Ë',
				L'Ì', L'Í', L'Î', L'Ï', L'Ð', L'Ñ', L'Ò', L'Ó', L'Ô', L'Õ', L'Ö', L'×', L'Ø', L'Ù',
				L'Ú', L'Û', L'Ü', L'Ý', L'Þ', L'ß', L'à', L'á', L'â', L'ã', L'ä', L'å', L'æ', L'ç',
				L'è', L'é', L'ê', L'ë', L'ì', L'í', L'î', L'ï', L'ð', L'ñ', L'ò', L'ó', L'ô', L'õ',
				L'ö', L'÷', L'ø', L'ù', L'ú', L'û', L'ü', L'ý', L'þ', L'ÿ'
			};

			//constexpr const wchar_t glyphs[] = {
			//	L'a'
			//};

			bool result = data.FontBuilder->Build(aPath, target, data.FontSize, glyphs, sizeof(glyphs) / sizeof(glyphs[0]));
			if (not result)
			{
				std::wcout << "Failed to build!\n";
			}
			if (data.Print || not result)
			{
				std::wcout << aPath << " -> " << target << "\n";
			}
		},
		&userData
	);

	return 0;
}