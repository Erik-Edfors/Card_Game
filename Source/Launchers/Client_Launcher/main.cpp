#include <Windows.h>
#include <exception>
#include <iostream>

#include <Implementations/Client/ClientApp.h>
#include <Core/Logger/Logger.h>

#include <Core/Logger/Logger.h>

#define FILE_CATEGORY "main.cpp"

static LRESULT CALLBACK locWinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	client::ClientApp* client = reinterpret_cast<client::ClientApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (client != nullptr && client->ProcessWindowsMessage(hWnd, uMsg, wParam, lParam))
	{
		return TRUE;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static int GuardedMain(const wchar_t* const* aArgs, size_t aNumArgs)
{
#define LOG_CATEGORY FILE_CATEGORY"->GuardedMain"

	constexpr LPCWSTR windowClassName = L"Card_Game_Client_Window";

	client::ClientApp client;

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
		LOG_ERROR(LOG_CATEGORY, "Failed to register window class!");
		return -1;
	}

	HWND window = CreateWindowW(
		windowClassName,
		L"Card Game",
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
		LOG_ERROR(LOG_CATEGORY, "Failed to create window!");
		return -1;
	}

	SetLastError(NOERROR);
	if (SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&client)) == NULL)
	{
		DWORD error = GetLastError();
		if (error != NOERROR)
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to set window user data!");
			return -1;
		}
	}

	ShowWindow(window, SW_SHOW);

	if (not client.Initialize(window, aArgs, aNumArgs))
	{
		LOG_ERROR(LOG_CATEGORY, "Failed to initialize Client!");
		return -1;
	}

	client.Run();

	if (not client.Shutdown())
	{
		LOG_WARNING(LOG_CATEGORY, "Failed to shutdown Client!");
	}

	if (IsWindow(window))
	{
		SetLastError(S_OK);
		if (DestroyWindow(window) == FALSE)
		{
			DWORD error = GetLastError();
			if (error != S_OK)
			{
				LOG_WARNING(LOG_CATEGORY, "Failed to destroy window!");
			}
		}
	}

	if (UnregisterClassW(windowClassName, executableModule) == FALSE)
	{
		LOG_WARNING(LOG_CATEGORY, "Failed to unregister window class!");
	}

	return 0;

#undef LOG_CATEGORY
}

extern int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
)
{
#define LOG_CATEGORY FILE_CATEGORY"->wWinMain"

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
		LOG_FATAL(LOG_CATEGORY, "GuardedMain threw exception -> {}", e.what());
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

		logger::Logger::SetOutputStream(&std::cout);
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

#undef LOG_CATEGORY
}

#undef FILE_CATEGORY