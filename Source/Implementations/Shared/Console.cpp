#include "Shared.pch.h"
#include "Console.h"

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

#define FILE_CATEGORY "Console.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Console"

namespace shared
{
	Console::Console() :
		myShouldScrollToBottom(true),
		myLastScroll(0.f),
		myScrollToBottom(2),
		myInputCallback(nullptr),
		myInputCallbackUserData(nullptr)
	{
		NewLine();
	}
	Console::~Console()
	{
	}

	void Console::Print(std::string_view aString)
	{
		using size = typename std::string::size_type;
		size begin = 0;
		size end = aString.find('\n', begin);
		myContents.back() += aString.substr(begin, end);
		if (end != std::string::npos)
		{
			NewLine();
			Print(aString.substr(end + 1));
		}
	}

	void Console::PrintLine(std::string_view aString)
	{
		Print(aString);
		NewLine();
	}

	void Console::NewLine()
	{
		myContents.push_back("");
		myScrollToBottom = 2;
	}

	void Console::SetInputCallback(InputCallback aCallback, void* aUserData)
	{
		myInputCallback = aCallback;
		myInputCallbackUserData = aUserData;
	}

	void Console::Update()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Update"

		if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoScrollbar))
		{
			const ImGuiStyle& style = ImGui::GetStyle();

			const float height = ImGui::GetWindowSize().y - 2.f * style.WindowPadding.y - 4.f * style.FramePadding.y - ImGui::GetFontSize() - 3.f * style.ItemSpacing.y - 2.f * style.ItemInnerSpacing.y;
			if (ImGui::BeginChild("##ConsoleTextBox", ImVec2(-1, height), ImGuiChildFlags_Borders, ImGuiWindowFlags_None))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 0.f });
				for (std::string& str : myContents)
				{
					ImGui::Text(str.c_str());
				}
				ImGui::PopStyleVar();

				if (ImGui::GetScrollY() != myLastScroll)
				{
					myShouldScrollToBottom = false;
				}

				if (myShouldScrollToBottom && myScrollToBottom != 0)
				{
					--myScrollToBottom;
					ImGui::SetScrollY(ImGui::GetScrollMaxY());
				}
				else
				{
					myShouldScrollToBottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY();
				}

				myLastScroll = ImGui::GetScrollY();
			}
			ImGui::EndChild();

			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputText("##Console Input", &myInput, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (ImGui::IsItemDeactivatedAfterEdit())
				{
					if (myInputCallback != nullptr)
					{
						(myInputCallback)(*this, myInput, myInputCallbackUserData);
					}

					myInput.clear();
					ImGui::SetKeyboardFocusHere(-1);
				}
			}
		}
		ImGui::End();

#undef LOG_CATEGORY
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY