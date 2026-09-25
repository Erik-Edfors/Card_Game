#include "Server.pch.h"
#include "MainScene.h"
#include "../Server.h"

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

#include <Core/CommonUtilities/ArgsParser.hpp>
#include <Core/CommonUtilities/StringHash.hpp>

#define FILE_CATEGORY "MainScene.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->MainScene"

namespace server
{
	static void test(const char* const* aArguments, size_t aNumArguments, void* aUserData)
	{
		aArguments;
		aNumArguments;

		MainScene& scene = *reinterpret_cast<MainScene*>(aUserData);

		CommonUtilities::ArgsParser<char> parser;
		parser.Parse(aArguments, aNumArguments);

		size_t numArguments = 0, numOptions = 0, numFlags = 0;

		const CommonUtilities::Argument<char>* arguments = nullptr;
		const CommonUtilities::Option<char>* options = nullptr;
		const CommonUtilities::Flag<char>* flags = nullptr;

		parser.GetArguments(&arguments, &numArguments);
		parser.GetOptions(&options, &numOptions);
		parser.GetFlags(&flags, &numFlags);

		scene.GetConsole().PrintLine("Test Succeeded!");

		scene.GetConsole().PrintLine("# Arguments:");
		for (size_t i = 0; i < numArguments; i++)
		{
			scene.GetConsole().PrintLine(std::format("|\t{}", arguments[i].GetValue()));
		}
		scene.GetConsole().PrintLine("# Options:");
		for (size_t i = 0; i < numOptions; i++)
		{
			scene.GetConsole().PrintLine(std::format("|\t{}={}", options[i].GetIdentity(), options[i].GetValue()));
		}
		scene.GetConsole().PrintLine("# Flags:");
		for (size_t i = 0; i < numFlags; i++)
		{
			scene.GetConsole().PrintLine(std::format("|\t/{}", flags[i].GetValue()));
		}
	}

	static void InputCallback(shared::Console& aConsole, std::string_view aInput, void* aUserData)
	{
		MainScene& scene = *reinterpret_cast<MainScene*>(aUserData);

		aConsole.PrintLine(std::format(">>{}", aInput));
		if (not scene.GetCommandParser().Call(aInput.data()))
		{
			aConsole.PrintLine("Unknown Command!");
		}
	}

	MainScene::MainScene(server::Server* aServer) : 
		myServer(aServer)
	{
	}
	MainScene::~MainScene()
	{
	}
	
	shared::Console& MainScene::GetConsole() noexcept
	{
		return myConsole;
	}
	shared::CommandParser& MainScene::GetCommandParser() noexcept
	{
		return myCommandParser;
	}

	void MainScene::OnInit()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::OnInit"

		myConsole.SetInputCallback(&InputCallback, this);

		myCommandParser.AddCommand("test", &test, this);

#undef LOG_CATEGORY
	}

	void MainScene::OnUpdate(float aDeltaTime)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::OnUpdate"

		aDeltaTime;

		ImGui::ShowDemoWindow();

		myConsole.Update();

#undef LOG_CATEGORY
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY