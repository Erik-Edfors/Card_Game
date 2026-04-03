#include "Client.pch.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Client.h"
#include "Scenes/LobbyScene.h"

namespace client
{
	Client::Client() : 
		myExiting(false)
	{
	}

	Client::~Client()
	{
	}

	LRESULT CALLBACK Client::ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE: case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool Client::Initialize(HWND aWindow, const wchar_t* const* aArgs, size_t aNumArgs)
	{
		aWindow;
		aArgs;
		aNumArgs;

		myScene = std::make_unique<client::LobbyScene>();

		return true;
	}

	void Client::Run()
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

				myScene->Update();
			}

			Sleep(100);
		}
	}
	void Client::Exit()
	{
		myExiting = true;
	}
}
