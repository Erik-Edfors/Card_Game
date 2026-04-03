#include <Windows.h>
#include <exception>
#include <iostream>

#include <Implementations/Server/Server.h>

static LRESULT CALLBACK locWinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	server::Server* client = reinterpret_cast<server::Server*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (client != nullptr)
	{
		return client->ProcessWindowsMessage(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static int GuardedMain(const wchar_t* const* aArgs, size_t aNumArgs)
{
	constexpr LPCWSTR windowClassName = L"Card_Game_Server_Window";

	server::Server server;

	const HMODULE executableModule = GetModuleHandle(NULL);

	WNDCLASSW wndClass = {};
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = &locWinProc;
	wndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wndClass.lpszClassName = windowClassName;
	wndClass.hIcon = NULL;
	wndClass.hInstance = executableModule;
	if (RegisterClassW(&wndClass) == NULL)
	{
		//TODO: Log failure.
		return -1;
	}

	HWND window = CreateWindowW(
		windowClassName,
		L"Card Game Server",
		WS_OVERLAPPEDWINDOW | WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		450,
		NULL,
		NULL,
		executableModule,
		NULL
	);
	if (window == NULL)
	{
		//TODO: Log failure.
		return -1;
	}

	SetLastError(NOERROR);
	if (SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&server)) == NULL)
	{
		DWORD error = GetLastError();
		if (error != NOERROR)
		{
			//TODO: Log failure.
			return -1;
		}
	}

	ShowWindow(window, SW_SHOW);

	if (not server.Initialize(window, aArgs, aNumArgs))
	{
		//TODO: Log failed initialize.
		return -1;
	}

	server.Run();

	if (DestroyWindow(window) == FALSE)
	{
		//TODO: Log failure.
	}

	if (UnregisterClassW(windowClassName, executableModule) == FALSE)
	{
		//TODO: Log failure.
	}

	return 0;
}

extern int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	int result = 0;

	LPWSTR commandLine = GetCommandLineW();

	int numArgs = 0;
	LPWSTR* args = CommandLineToArgvW(commandLine, &numArgs);

#ifdef _RELEASE
	try
	{
		result = GuardedMain(args, static_cast<size_t>(numArgs));
	}
	catch (std::exception e)
	{
		result = -1;
	}
#else
	if (AllocConsole())
	{
		FILE* consoleOut = nullptr;
		FILE* consoleErr = nullptr;
		freopen_s(&consoleOut, "CONOUT$", "w", stdout);  // NOLINT(cert-err33-c)
		if (consoleOut)
		{
			setvbuf(consoleOut, nullptr, _IONBF, 1024);  // NOLINT(cert-err33-c)
		}
		freopen_s(&consoleErr, "CONOUT$", "w", stderr);  // NOLINT(cert-err33-c)
		if (consoleErr)
		{
			setvbuf(consoleErr, nullptr, _IONBF, 1024);  // NOLINT(cert-err33-c)
		}
	}
	else
	{
		//TODO: Log console allocation failed.
	}
	result = GuardedMain(args, static_cast<size_t>(numArgs));
	system("Pause");
#endif

	LocalFree(args);

	return result;
}