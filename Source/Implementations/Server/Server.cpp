#include "Server.pch.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Server.h"

namespace server
{
	Server::Server() : 
		myExiting(false)
	{
	}
	Server::~Server()
	{
	}

	LRESULT CALLBACK Server::ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE: case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool Server::Initialize(HWND aWindow, const wchar_t* const* aArgs, size_t aNumArgs)
	{
		aWindow;
		aArgs;
		aNumArgs;

		return true;
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

			Sleep(100);
		}
	}
	void Server::Exit()
	{
		myExiting = true;
	}
}