#pragma once
#include <string>
#include <unordered_map>

namespace shared
{
	class CommandParser
	{
	public:
		using CommandCallback = void(*)(const char* const* aArguments, size_t aNumArguments, void* aUserData);

		CommandParser();
		~CommandParser();

		void AddCommand(const char* aSignature, CommandCallback aCallback, void* aUserData);

		bool Call(const char* aCommand);

	private:
		struct _CommandEntry
		{
			CommandCallback Callback;
			void* UserData;
		};

		std::unordered_map<std::string, _CommandEntry> myCommands;
	};
}