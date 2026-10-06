#pragma once
#include <string>

class Console
{
public:
	using InputCallback = void(*)(Console& aConsole, std::string_view aInput, void* aUserData);

	Console();
	~Console();

	void SetOpen(bool aOpen) noexcept;
	bool IsOpen() const noexcept;

	void Print(std::string_view aString);
	void PrintLine(std::string_view aString);
	void NewLine();

	void SetInputCallback(InputCallback aCallback, void* aUserData);

	void Update();
private:
	bool myOpen;

	std::string myInput;
	std::vector<std::string> myContents;
	float myLastScroll;
	size_t myScrollToBottom;
	bool myShouldScrollToBottom;

	InputCallback myInputCallback;
	void* myInputCallbackUserData;
};