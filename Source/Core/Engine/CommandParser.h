#pragma once
#include <string>
#include <unordered_map>

class CommandParser
{
public:
	struct CallbackData
	{
		const char* const* Arguments;
		size_t NumArguments;
		void* UserData;
	};
	using CommandCallback = void(*)(const CallbackData& aData);

	CommandParser();
	~CommandParser();

	void AddCommand(std::string_view aSignature, CommandCallback aCallback, void* aUserData);
	void RemoveCommand(std::string_view aSignature);

	bool Call(std::string_view aCommand);

private:
	struct _CommandEntry
	{
		CommandCallback Callback;
		void* UserData;
	};

	std::unordered_map<std::string, _CommandEntry> myCommands;
};