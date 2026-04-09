#include "Engine.pch.h"
#include "Engine.h"
#include "Scene.h"

#include <Core/RenderHardwareInterface/Graphics.h>
#include <Core/RenderHardwareInterface/DX11/Window_DX11Impl.h>
#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/CommonUtilities/InputManager.h>

Engine::Engine()
{
}

Engine::~Engine()
{
	// Makes sure scenes are destroyed before any other thing in Engine, as scenes may attempt to access them.
	mySceneStack.clear();
}

void Engine::Initialize(HWND aWindow)
{
	graphics::PImpl<graphics::Window_DX11Impl> window = new graphics::Window_DX11Impl(aWindow);

	graphics::GRAPHICS_DEVICE_FLAGS deviceFlags = graphics::GRAPHICS_DEVICE_ALLOW_TEARING
		| graphics::GRAPHICS_DEVICE_BGRA_SUPPORT
		| graphics::GRAPHICS_DEVICE_VIDEO_SUPPORT;

#ifdef _DEBUG
	deviceFlags |= graphics::GRAPHICS_DEVICE_DEBUG;
#endif

	graphics::PImpl<graphics::Device> device;
	graphics::CreateDevice(&device, window.Get(), deviceFlags);

	myGraphicsEngine = std::make_unique<GraphicsEngine>(device.Get());
	myGraphicsEngine->Initialize();

	myInputManager = std::make_unique<CommonUtilities::InputManager>();
}

bool Engine::ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (myInputManager->UpdateEvents(uMsg, wParam, lParam))
	{
		return true;
	}

	return false;
}

bool Engine::BeginFrame()
{
	if (not mySceneStack.empty())
	{
		mySceneStack.back()->Update();
	}

	return true;
}

void Engine::EndFrame()
{
	myGraphicsEngine->Render();
}

const CommonUtilities::InputManager& Engine::GetInputManager() const noexcept
{
	return *myInputManager;
}

GraphicsEngine& Engine::GetGraphicsEngine() noexcept
{
	return *myGraphicsEngine;
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
