#include <iostream>
#include <Core/CommonUtilities/ArgsParser.hpp>
#include <Core/CommonUtilities/StringHash.hpp>
#include <filesystem>
#include <regex>
#include <fstream>

#include "ModelBuilder.h"
#include "FontBuilder.h"

static const wchar_t locManual[] =
L"| AssetBuilder                            -> Prints manual.\n"
L"| AssetBuilder <input> <ouput> <type>     -> Builds raw asset into asset of type from input and places it at output.\n"
L"| \n"
L"| # Arguments\n"
L"| | <input>                               -> A file/directory location to process.\n"
L"| | <output>                              -> A file/directory location to output result(s).\n"
L"| | <type>                                -> An asset type to build. [model/font]\n"
L"|\n"
L"| # Options\n"
L"| | --type=<assetType>                    -> An asset type to build. [model/font].\n"
L"| | --input=<input>                       -> A file/directory location to process. More explicit syntax.\n"
L"| | --output=<output>                     -> A file/directory location to output result(s). More explicit syntax.\n"
L"| | --regex=<regexpr>                     -> A RegExpr used to filter input files in a directory.\n"
L"| | --extension=<fileExtension>           -> A file extension for the output file.\n"
L"| | --recursive=<true/false>              -> If input should iterate over files in sub-directories recursively. Default: false.\n"
L"| | --print=<true/false>                  -> If result should be printed. Format '<input file> -> <output file>'. Default: false.\n"
L"| |\n"
L"| | # Font Specific Options\n"
L"| | | --emsize=<emSize>                   -> The size, in pixels, of one EM typeset unit. Default: 24.0.\n"
L"| | | --sdlength=<length>                 -> The length in pixels of the signed distance field. Default: 4.0.\n"
L"| | |\n"
L"| | # Model Specific Options\n"
L"| | | ...\n"
L"|\n"
L"| # Flags\n"
L"| | /r                                    -> Flags that input should iterate over files in sub-directories recursively. Equivelant to '--recursive=true'.\n"
L"| | /p                                    -> Flags that result should be printed. Format '<input file> -> <output file>'. Equivelant to '--print=true'."
;

bool BuildAsset(const wchar_t* aType, const std::filesystem::path& aInput, const std::filesystem::path& aOutput, const CommonUtilities::Option<wchar_t>* aOptions, size_t aNumOptions)
{
	const std::filesystem::path outputDirectory = std::filesystem::path(aOutput).remove_filename();

	if (not outputDirectory.empty())
	{
		std::filesystem::create_directories(outputDirectory);
	}

	switch (CommonUtilities::WStringHash(aType))
	{
	case L"model"_hash:
	{
		ModelBuilder builder;
		if (not builder.Build(aInput, aOutput))
		{
			std::wcout << L"Failed to build model!" << std::endl;
			return false;
		}
		break;
	}
	case L"font"_hash:
	{
		FontBuilder builder;

		for (size_t i = 0; i < aNumOptions; i++)
		{
			const wchar_t* identity = aOptions[i].GetIdentity();
			const wchar_t* value = aOptions[i].GetValue();
			switch (CommonUtilities::WStringHash(identity))
			{
			case "sdlength"_hash:
				try
				{
					const float sdlength = std::stof(value);
					builder.SetSignedDistanceLength(sdlength);
				}
				catch (std::exception) {}
				break;
			case "emsize"_hash:
				try
				{
					const float emsize = std::stof(value);
					builder.SetEmSize(emsize);
				}
				catch (std::exception) {}
				break;
			}
		}

		if (not builder.Build(aInput, aOutput))
		{
			std::wcout << L"Failed to build font!" << std::endl;
			return false;
		}
		break;
	}
	default:
		std::wcout << "Failed to build asset! Unknown type '" << aType << "'." << std::endl;
		return false;
	}

	return true;
}

int wmain(const int argc, const wchar_t* const* argv)
{
	if (argc <= 1)
	{
		std::wcout << locManual << std::endl;
		return 0;
	}

	CommonUtilities::ArgsParser<wchar_t> parser;
	if (not parser.Parse(argv, argc))
	{
		std::wcout << L"Failed to parse arguments! An error occurred." << std::endl;
		return -1;
	}

	const CommonUtilities::Argument<wchar_t>* arguments = nullptr;
	const CommonUtilities::Option<wchar_t>* options = nullptr;
	const CommonUtilities::Flag<wchar_t>* flags = nullptr;

	size_t numArguments = 0;
	size_t numOptions = 0;
	size_t numFlags = 0;

	parser.GetArguments(&arguments, &numArguments);
	parser.GetOptions(&options, &numOptions);
	parser.GetFlags(&flags, &numFlags);

	bool recursive = false;
	bool print = false;
	std::filesystem::path input;
	std::filesystem::path output;
	std::wstring fileType;
	std::wstring fileExtension;
	std::wregex regex = std::wregex(L".*", std::wregex::icase);

	if (numArguments > 1)
	{
		input = arguments[1].GetValue();
	}
	if (numArguments > 2)
	{
		output = arguments[2].GetValue();
	}
	if (numArguments > 3)
	{
		fileType = arguments[3].GetValue();
	}

	for (size_t i = 0; i < numOptions; i++)
	{
		const CommonUtilities::Option<wchar_t>& option = options[i];

		switch (CommonUtilities::WStringHash(option.GetIdentity()))
		{
		case L"recursive"_hash:
			switch (CommonUtilities::WStringHash(option.GetValue()))
			{
			case L"true"_hash: case L"yes"_hash:
				recursive = true;
				break;
			case L"false"_hash: case L"no"_hash:
				recursive = false;
				break;
			}
			break;

		case L"print"_hash:
			switch (CommonUtilities::WStringHash(option.GetValue()))
			{
			case L"true"_hash: case L"yes"_hash:
				print = true;
				break;
			case L"false"_hash: case L"no"_hash:
				recursive = false;
				break;
			}
			break;

		case L"regex"_hash:
			regex = std::wregex(option.GetValue(), std::wregex::icase);
			break;

		case L"type"_hash:
			fileType = option.GetValue();
			break;

		case L"input"_hash:
			input = option.GetValue();
			break;

		case L"output"_hash:
			output = option.GetValue();
			break;

		case L"extension"_hash:
			fileExtension = option.GetValue();
			break;
		}
	}
	for (size_t i = 0; i < numFlags; i++)
	{
		const wchar_t* chars = flags->GetValue();
		while (*chars != L'\0')
		{
			switch (*chars)
			{
			case L'r': recursive = true; break;
			case L'p': print = true; break;
			}
			chars = chars + 1;
		}
	}

	if (input.empty())
	{
		std::wcout << L"Failed to find file! No argument for input given." << std::endl;
		return -1;
	}
	if (output.empty())
	{
		std::wcout << L"Failed to process file! No argument for output given." << std::endl;
		return -1;
	}
	if (fileType.empty())
	{
		std::wcout << L"Failed to process file! No argument for type given." << std::endl;
		return -1;
	}

	if (input.has_filename())
	{
		if (recursive)
		{
			std::wcout << L"Failed to process files! Input was file, but recursive was flagged." << std::endl;
			return -1;
		}

		if (std::regex_match(input.filename().wstring(), regex))
		{
			const std::filesystem::path inputFile = input;
			std::filesystem::path outputFile = output;

			if (not output.has_filename())
			{
				outputFile /= inputFile.filename();
			}

			if (not fileExtension.empty())
			{
				outputFile.replace_extension(fileExtension);
			}

			if (not BuildAsset(fileType.c_str(), inputFile, outputFile, options, numOptions))
			{
				std::wcout << inputFile.wstring() << " Failed!" << std::endl;
			}
			else
			{
				std::wcout << inputFile.wstring() << L" -> " << outputFile.wstring() << std::endl;
			}
		}
	}
	else
	{
		if (output.has_filename())
		{
			std::wcout << L"Failed to process files! Output was not a directory." << std::endl;
			return -1;
		}

		if (recursive)
		{
			for (auto& entry : std::filesystem::recursive_directory_iterator(input))
			{
				if (entry.is_regular_file() && std::regex_match(entry.path().filename().wstring(), regex))
				{
					const std::filesystem::path inputFile = entry.path();
					std::filesystem::path outputFile = output / inputFile.lexically_relative(input);

					if (not fileExtension.empty())
					{
						outputFile.replace_extension(fileExtension);
					}

					if (not BuildAsset(fileType.c_str(), inputFile, outputFile, options, numOptions))
					{
						std::wcout << inputFile.wstring() << " Failed!" << std::endl;
					}
					else
					{
						std::wcout << inputFile.wstring() << L" -> " << outputFile.wstring() << std::endl;
					}
				}
			}
		}
		else
		{
			for (auto& entry : std::filesystem::directory_iterator(input))
			{
				if (entry.is_regular_file() && std::regex_match(entry.path().filename().wstring(), regex))
				{
					const std::filesystem::path inputFile = entry.path();
					std::filesystem::path outputFile = output / inputFile.lexically_relative(input);

					if (not fileExtension.empty())
					{
						outputFile.replace_extension(fileExtension);
					}

					if (not BuildAsset(fileType.c_str(), inputFile, outputFile, options, numOptions))
					{
						std::wcout << inputFile.wstring() << " Failed!" << std::endl;
					}
					else
					{
						std::wcout << inputFile.wstring() << L" -> " << outputFile.wstring() << std::endl;
					}
				}
			}
		}
	}
}