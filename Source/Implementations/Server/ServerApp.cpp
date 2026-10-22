#include "Server.pch.h"
#include "ServerApp.h"
#include "Scenes/MainScene.h"
#include "Server.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/Console.h>
#include <Core/Engine/CommandParser.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/RenderHardwareInterface/Device.h>

#include <Core/imgui/imgui.h>

#include <Implementations/Shared/Network/NetworkManager.h>
#include <Implementations/Shared/Network/Messages.h>

#include <Core/Networking/Networking.h>

#define FILE_CATEGORY "ServerApp.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->ServerApp"

namespace server
{
	static void Ping(const CommandParser::CallbackData& aData)
	{
		ServerApp& server = *reinterpret_cast<ServerApp*>(aData.UserData);

		networking::SocketAddress address = {
			.Port = 54321,
			.IPAddress = networking::NETWORKING_IPV4_LOOPBACK
		};

		shared::NM_Ping ping;
		shared::NetworkManager::NetworkMessage message;
		server.GetServer().CreateMessage(&message, ping, false);
		server.GetServer().SendTo(message, address);
	}

	static void Status(const CommandParser::CallbackData& aData)
	{
		ServerApp& server = *reinterpret_cast<ServerApp*>(aData.UserData);
		Console& console = server.GetEngine().GetConsole();

		console.PrintLine(std::format("Allowing Connection: {}", server.GetServer().IsAllowingConnections()));
		console.PrintLine(std::format("Number of Clients: {}", server.GetServer().GetNumClients()));
	}

	static void OnPing(const Server::MessageCallbackData<shared::NM_Ping>& aData)
	{
		ServerApp& server = *reinterpret_cast<ServerApp*>(aData.UserData);

		server.GetEngine().GetConsole().PrintLine("Recieved Ping!");
	}

	ServerApp::ServerApp() : 
		myExiting(false)
	{
	}
	ServerApp::~ServerApp()
	{
	}

	bool ServerApp::ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		if (myEngine && myEngine->ProcessWindowsMessage(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}

		switch (uMsg)
		{
		case WM_CLOSE: case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
		case WM_SIZE:
		{
			myEngine->GetGraphicsEngine().GetDevice()->UpdateBackbufferSize(LOWORD(lParam), HIWORD(lParam));
		}
		break;
		}

		return false;
	}

	bool ServerApp::Initialize(HWND aWindow, const wchar_t* const* aArgs, size_t aNumArgs)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Initialize"

		aArgs;
		aNumArgs;

		myEngine = std::make_unique<Engine>();
		myServer = std::make_unique<Server>();

		if (not networking::Initialize())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize networking!");
			return false;
		}

		if (not myEngine->Initialize(aWindow, L"../../../Assets/"))	//TODO: Get asset path from config file.
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize Engine!");
			return false;
		}

		if (not myServer->Initialize(54322))	// TODO: Allow setting other port.
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize NetworkManager!");
			return false;
		}
		//myServer->SetAllowingConnections(true);	// TODO: Only allow when game is ready.

		myEngine->GetCommandParser().AddCommand(
			"ping",
			&Ping,
			this
		);
		myEngine->GetCommandParser().AddCommand(
			"status",
			&Status,
			this
		);

		myServer->SetMessageCallback<shared::NM_Ping>(&OnPing, this);

		myEngine->GetGraphicsEngine().SetVSyncEnabled(true);

		myEngine->SetConsoleEnabled(true);
		//myEngine->GetConsole().SetOpen(true);

		myEngine->PushScene<MainScene>(this);

		return true;
#undef LOG_CATEGORY
	}

	bool ServerApp::Shutdown()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Shutdown"

		bool result = true;

		myEngine->GetCommandParser().RemoveCommand("ping");
		myEngine->GetCommandParser().RemoveCommand("status");

		if (not myEngine->Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown Engine.");
			result = false;
		}
		if (not myServer->Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown NetworkManager.");
			result = false;
		}
		if (not networking::Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown networking.");
			result = false;
		}
		myEngine.reset();
		myServer.reset();
		myExiting = false;

		return result;

#undef LOG_CATEGORY
	}

	void ServerApp::Run()
	{
		while (myExiting == false)
		{
			MSG msg = {};
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				switch (msg.message)
				{
				case WM_QUIT:
					myExiting = true;
					break;
				}
			}

			if (myEngine->BeginFrame())
			{
				ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode, NULL);

				myEngine->Update();

				myServer->Update();

				myEngine->EndFrame();
			}
		}
	}
	void ServerApp::Exit()
	{
		myExiting = true;
	}

	Engine& ServerApp::GetEngine() const noexcept
	{
		return *myEngine;
	}
	Server& ServerApp::GetServer() const noexcept
	{
		return *myServer;
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY