#pragma once
#include <string>

namespace shared
{
	class Console
	{
	public:
		using InputCallback = void(*)(Console& aConsole, std::string_view aInput, void* aUserData);

		Console();
		~Console();

		void Print(std::string_view aString);
		void PrintLine(std::string_view aString);
		void NewLine();

		void SetInputCallback(InputCallback aCallback, void* aUserData);

		void Update();
	private:
		std::string myInput;
		std::vector<std::string> myContents;
		float myLastScroll;
		size_t myScrollToBottom;
		bool myShouldScrollToBottom;

		InputCallback myInputCallback;
		void* myInputCallbackUserData;
	};
}