#include "Server.pch.h"
#include "Server.h"
#include "Scenes/MainScene.h"

#include <Core/Engine/Engine.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/RenderHardwareInterface/Device.h>

#include <Core/imgui/imgui.h>

#define FILE_CATEGORY "Server.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Server"

namespace server
{
	Server::Server() : 
		myExiting(false)
	{
	}
	Server::~Server()
	{
	}

	bool Server::ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
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

	bool Server::Initialize(HWND aWindow, const wchar_t* const* aArgs, size_t aNumArgs)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Initialize"

		aArgs;
		aNumArgs;

		myEngine = std::make_unique<Engine>();

		if (not myEngine->Initialize(aWindow, L"../../../Assets/"))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize Engine!");
			return false;
		}

		myEngine->PushScene<MainScene>(this);

		return true;
#undef LOG_CATEGORY
	}

	bool Server::Shutdown()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Shutdown"

		bool result = true;
		if (not myEngine->Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown Engine.");
			result = false;
		}
		myEngine.reset();
		myExiting = false;

		return result;

#undef LOG_CATEGORY
	}

	void Server::Run()
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

				myEngine->EndFrame();
			}
		}
	}
	void Server::Exit()
	{
		myExiting = true;
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY