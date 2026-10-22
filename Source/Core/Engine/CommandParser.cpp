#include "Engine.pch.h"
#include "CommandParser.h"
#include <regex>

static char tolowerc(char c)
{
	return static_cast<char>(std::tolower(static_cast<int>(c)));
}

CommandParser::CommandParser()
{
}

CommandParser::~CommandParser() = default;

void CommandParser::AddCommand(std::string_view aSignature, CommandCallback aCallback, void* aUserData)
{
	myCommands.emplace(aSignature.data(), _CommandEntry{.Callback = aCallback, .UserData = aUserData});
}

void CommandParser::RemoveCommand(std::string_view aSignature)
{
	myCommands.erase(aSignature.data());
}

bool CommandParser::Call(std::string_view aCommand)
{
	const std::regex firstRx("^\\s*(?:([^\"][^\\s]*)|\"([^\"]*)\")\\s*(.*)$");
	std::smatch matches;
	std::vector<std::string> strArgs;
	std::string current = aCommand.data();
	while (std::regex_match(current, matches, firstRx))
	{
		strArgs.push_back(matches[1].matched ? matches[1] : matches[2]);
		current = matches[3];
	}

	std::string commandSignature = strArgs.front();
	std::transform(commandSignature.begin(), commandSignature.end(), commandSignature.begin(), tolowerc);

	auto it = myCommands.find(commandSignature);
	if (it != myCommands.end())
	{
		const size_t numArguments = strArgs.size();
		const char** arguments = new const char* [numArguments];
		for (size_t i = 0; i < numArguments; i++)
		{
			arguments[i] = strArgs[i].c_str();
		}
		_CommandEntry& entry = it->second;

		const CallbackData data = {
			.Arguments = arguments,
			.NumArguments = numArguments,
			.UserData = entry.UserData
		};

		entry.Callback(data);
		return true;
	}
	return false;
}