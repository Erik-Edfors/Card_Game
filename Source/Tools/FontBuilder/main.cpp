#include "FileIteration.h"
#include "FontBuilder.h"

#include <Core/CommonUtilities/ArgsParser.hpp>
#include <Core/CommonUtilities/StringHash.hpp>

#include <iostream>

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
			target.replace_extension(".bf");

			std::unique_ptr<wchar_t[]> extendedASCIIChars = std::make_unique<wchar_t[]>(256);
			for (wchar_t c = 0; c < 256; c++)
			{
				extendedASCIIChars[c] = c;
			}

			bool result = data.FontBuilder->BuildSimpleAtlas(aPath, target, data.FontSize, extendedASCIIChars.get(), 256);
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