#include "Server.pch.h"
#include "MainScene.h"
#include "../ServerApp.h"
#include "../Server.h"
#include "LobbyScene.h"

#include <Core/Engine/Engine.h>

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

//#include <Core/CommonUtilities/ArgsParser.hpp>
//#include <Core/CommonUtilities/StringHash.hpp>

#define FILE_CATEGORY "MainScene.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->MainScene"

namespace server
{
	//static void test(const char* const* aArguments, size_t aNumArguments, void* aUserData)
	//{
	//	aArguments;
	//	aNumArguments;

	//	MainScene& scene = *reinterpret_cast<MainScene*>(aUserData);

	//	CommonUtilities::TArgsParser<char> parser;
	//	parser.Parse(aArguments, aNumArguments);

	//	size_t numArguments = 0, numOptions = 0, numFlags = 0;

	//	const CommonUtilities::TArgument<char>* arguments = nullptr;
	//	const CommonUtilities::TOption<char>* options = nullptr;
	//	const CommonUtilities::TFlag<char>* flags = nullptr;

	//	parser.GetArguments(&arguments, &numArguments);
	//	parser.GetOptions(&options, &numOptions);
	//	parser.GetFlags(&flags, &numFlags);

	//	scene.GetConsole().PrintLine("Test Succeeded!");

	//	scene.GetConsole().PrintLine("# Arguments:");
	//	for (size_t i = 0; i < numArguments; i++)
	//	{
	//		scene.GetConsole().PrintLine(std::format("|\t{}", arguments[i].GetValue()));
	//	}
	//	scene.GetConsole().PrintLine("# Options:");
	//	for (size_t i = 0; i < numOptions; i++)
	//	{
	//		scene.GetConsole().PrintLine(std::format("|\t{}={}", options[i].GetIdentity(), options[i].GetValue()));
	//	}
	//	scene.GetConsole().PrintLine("# Flags:");
	//	for (size_t i = 0; i < numFlags; i++)
	//	{
	//		scene.GetConsole().PrintLine(std::format("|\t/{}", flags[i].GetValue()));
	//	}
	//}

	//static void InputCallback(shared::Console& aConsole, std::string_view aInput, void* aUserData)
	//{
	//	MainScene& scene = *reinterpret_cast<MainScene*>(aUserData);

	//	aConsole.PrintLine(std::format(">>{}", aInput));
	//	if (not scene.GetCommandParser().Call(aInput.data()))
	//	{
	//		aConsole.PrintLine("Unknown Command!");
	//	}
	//}

	MainScene::MainScene(server::ServerApp* aServer) :
		myServer(aServer)
	{
	}
	MainScene::~MainScene()
	{
	}

	//shared::Console& MainScene::GetConsole() noexcept
	//{
	//	return myConsole;
	//}
	//shared::CommandParser& MainScene::GetCommandParser() noexcept
	//{
	//	return myCommandParser;
	//}

	void MainScene::OnInit()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::OnInit"

		myServerPortName = "54322";

		//myConsole.SetInputCallback(&InputCallback, this);

		//myCommandParser.AddCommand("test", &test, this);

#undef LOG_CATEGORY
	}

	void MainScene::OnEnter()
	{
		myServer->ResetServer();
	}

	void MainScene::OnUpdate(float aDeltaTime)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::OnUpdate"

		aDeltaTime;

		Server& server = myServer->GetServer();

		const ImGuiWindowFlags flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		if (ImGui::Begin("Main", nullptr, flags))
		{
			ImGui::SeparatorText("Server Setup");

			ImGui::Text("Port");
			ImGui::SetNextItemWidth(-1);
			ImGui::InputText("##Port", &myServerPortName, ImGuiInputTextFlags_CharsDecimal);

			std::string input = server.GetName();
			ImGui::Text("Name");
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputText("##Name", &input, ImGuiInputTextFlags_AutoSelectAll))
			{
				server.SetName(input);
			}

			input = server.GetDescription();
			ImGui::Text("Description");
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputText("##Description", &input, ImGuiInputTextFlags_AutoSelectAll))
			{
				server.SetDescription(input);
			}

			ImGui::NewLine();
			if (ImGui::Button("Host Game", { -1.f, 0.f }))
			{
				if (not server.GetName().empty())
				{
					bool failedToParsePort = false;
					networking::Port port = 54322;
					try
					{
						port = static_cast<networking::Port>(std::stoi(myServerPortName));
					}
					catch (std::exception)
					{
						failedToParsePort = true;
					}

					if (not failedToParsePort && server.Bind(port))
					{
						GetEngine().ChangeScene<LobbyScene>(myServer);
					}
				}
			}
		}
		ImGui::End();

		//ImGui::ShowDemoWindow();

		//myConsole.Update();

#undef LOG_CATEGORY
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY