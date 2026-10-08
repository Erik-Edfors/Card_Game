#include "Engine.pch.h"
#include "Engine.h"
#include "Scene.h"
#include "Console.h"
#include "CommandParser.h"

#include <Core/RenderHardwareInterface/Graphics.h>
#include <Core/RenderHardwareInterface/DX11/Window_DX11Impl.h>
#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/CommonUtilities/InputManager.h>
#include <Core/CommonUtilities/TimeManager.h>
#include <Core/Assets/AssetManager.h>

#define FILE_CATEGORY "Engine.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Engine"

static void locConsoleInputCallback(Console& aConsole, std::string_view aInput, void* aUserData)
{
	Engine* engine = reinterpret_cast<Engine*>(aUserData);

	aConsole.PrintLine(std::format(">>{}", aInput));

	if (not engine->GetCommandParser().Call(aInput.data()))
	{
		aConsole.PrintLine("Unknown Command!");
	}
}

Engine::Engine() : 
	myConsoleEnabled(false)
{
}

Engine::~Engine()
{
	// Makes sure scenes are destroyed before any other thing in Engine, as scenes may attempt to access them.
	mySceneStack.clear();
}

bool Engine::Initialize(HWND aWindow, const wchar_t* aAssetPath)
{
#define LOG_CATEGORY CLASS_CATEGORY"::Initialize"
	std::unique_ptr<Console> console = std::make_unique<Console>();
	std::unique_ptr<CommandParser> commandParser = std::make_unique<CommandParser>();
	std::unique_ptr<GraphicsEngine> graphicsEngine = std::make_unique<GraphicsEngine>();
	std::unique_ptr<CommonUtilities::InputManager> inputManager = std::make_unique<CommonUtilities::InputManager>();
	std::unique_ptr<CommonUtilities::TimeManager> timeManager = std::make_unique<CommonUtilities::TimeManager>();

	console->SetInputCallback(&locConsoleInputCallback, this);

	graphics::PImpl<graphics::Window_DX11Impl> window = new graphics::Window_DX11Impl(aWindow);
	myWindow = window.Get();

	graphics::GRAPHICS_DEVICE_FLAGS deviceFlags = graphics::GRAPHICS_DEVICE_ALLOW_TEARING
		| graphics::GRAPHICS_DEVICE_BGRA_SUPPORT
		| graphics::GRAPHICS_DEVICE_VIDEO_SUPPORT;

#ifdef _DEBUG
	deviceFlags |= graphics::GRAPHICS_DEVICE_DEBUG;
#endif

	graphics::PImpl<graphics::Device> device;
	graphics::result::Code result = graphics::CreateDevice(&device, window.Get(), deviceFlags);
	if (GRAPHICS_FAILED(result))
	{
		const graphics::result::Message msg = graphics::result::GetErrorMessage(result);
		LOG_ERROR(LOG_CATEGORY, "Failed to initialize Engine! graphics::CreateDevice returned error. Error: '%s: %s'.", msg.Type, msg.Desc);
		return false;
	}

	if (not graphicsEngine->Initialize(device.Get()))
	{
		LOG_ERROR(LOG_CATEGORY, "Failed to initialize Engine! Failed to initialize GraphicsEngine.");
		return false;
	}

	if (not assets::AssetManager::Initialize(aAssetPath))
	{
		LOG_ERROR(LOG_CATEGORY, "Failed to initialize Engine! Failed to initialize AssetManager.");
		return false;
	}

	myConsole = std::move(console);
	myCommandParser = std::move(commandParser);
	myGraphicsEngine = std::move(graphicsEngine);
	myInputManager = std::move(inputManager);
	myTimeManager = std::move(timeManager);

	return true;

#undef LOG_CATEGORY
}

bool Engine::Shutdown()
{
#define LOG_CATEGORY CLASS_CATEGORY"::Shutdown"

	bool result = true;
	if (not myGraphicsEngine->Shutdown())
	{
		LOG_WARNING(LOG_CATEGORY, "Failed to shutdown Engine! Failed to shutdown GraphicsEngine.");
		result = false;
	}
	myGraphicsEngine.reset();
	myInputManager.reset();
	if (not assets::AssetManager::Shutdown())
	{
		LOG_WARNING(LOG_CATEGORY, "Failed to shutdown Engine! Failed to shutdown AssetManager.");
		result = false;
	}
	return result;

#undef LOG_CATEGORY
}

bool Engine::ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	graphics::Window_DX11Impl* window = static_cast<graphics::Window_DX11Impl*>(myWindow.Get());

	if (window->GetWindowHandle() == hWnd)
	{
		if (window->ProcessMessage(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}
	}

	if (myInputManager->UpdateEvents(uMsg, wParam, lParam))
	{
		return true;
	}

	return false;
}

void Engine::SetConsoleEnabled(bool aEnabled) noexcept
{
	myConsoleEnabled = aEnabled;
	if (not myConsoleEnabled)
	{
		myConsole->SetOpen(false);
	}
}

bool Engine::IsConsoleEnabled() const noexcept
{
	return myConsoleEnabled;
}

bool Engine::BeginFrame()
{
	myGraphicsEngine->BeginFrame();

	return true;
}

void Engine::Update()
{
	myInputManager->UpdateInput();

	if (IsConsoleEnabled() && myInputManager->GetKeyDown(CommonUtilities::Keys::F1))
	{
		myConsole->SetOpen(not myConsole->IsOpen());
	}

	if (not mySceneStack.empty())
	{
		mySceneStack.back()->Update(myTimeManager->GetDelta());
	}
}

void Engine::EndFrame()
{
	myConsole->Update();

	myGraphicsEngine->EndFrame();
	myTimeManager->Update();
}

Console& Engine::GetConsole() noexcept
{
	return *myConsole;
}
CommandParser& Engine::GetCommandParser() noexcept
{
	return *myCommandParser;
}
GraphicsEngine& Engine::GetGraphicsEngine() noexcept
{
	return *myGraphicsEngine;
}

const Console& Engine::GetConsole() const noexcept
{
	return *myConsole;
}
const CommandParser& Engine::GetCommandParser() const noexcept
{
	return *myCommandParser;
}
const CommonUtilities::InputManager& Engine::GetInputManager() const noexcept
{
	return *myInputManager;
}
const CommonUtilities::TimeManager& Engine::GetTimeManager() const noexcept
{
	return *myTimeManager;
}

Scene* Engine::GetScene() noexcept
{
	if (not mySceneStack.empty())
	{
		return mySceneStack.back().get();
	}
	return nullptr;
}

void Engine::PopScene()
{
	if (not mySceneStack.empty())
	{
		mySceneStack.back()->Shutdown();
		mySceneStack.pop_back();
	}
}

#undef FILE_CATEGORY 
#undef CLASS_CATEGORY