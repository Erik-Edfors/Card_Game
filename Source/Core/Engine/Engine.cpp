#include "Engine.pch.h"
#include "Engine.h"
#include "Scene.h"

#include <Core/RenderHardwareInterface/Graphics.h>
#include <Core/RenderHardwareInterface/DX11/Window_DX11Impl.h>
#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/CommonUtilities/InputManager.h>
#include <Core/CommonUtilities/TimeManager.h>
#include <Core/Assets/AssetManager.h>

#define FILE_CATEGORY "Engine.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Engine"

Engine::Engine()
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
	std::unique_ptr<GraphicsEngine> graphicsEngine = std::make_unique<GraphicsEngine>();
	std::unique_ptr<CommonUtilities::InputManager> inputManager = std::make_unique<CommonUtilities::InputManager>();
	std::unique_ptr<CommonUtilities::TimeManager> timeManager = std::make_unique<CommonUtilities::TimeManager>();

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

bool Engine::BeginFrame()
{
	myGraphicsEngine->BeginFrame();

	return true;
}

void Engine::Update()
{
	myInputManager->UpdateInput();

	if (not mySceneStack.empty())
	{
		mySceneStack.back()->Update(myTimeManager->GetDelta());
	}
}

void Engine::EndFrame()
{
	myGraphicsEngine->EndFrame();
	myTimeManager->Update();
}

const CommonUtilities::InputManager& Engine::GetInputManager() const noexcept
{
	return *myInputManager;
}

GraphicsEngine& Engine::GetGraphicsEngine() noexcept
{
	return *myGraphicsEngine;
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
		mySceneStack.pop_back();
	}
}

#undef FILE_CATEGORY 
#undef CLASS_CATEGORY