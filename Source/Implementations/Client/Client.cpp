#include "Client.pch.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Client.h"
#include "Scenes/LobbyScene.h"

#include <Core/Engine/Engine.h>

//TEMPORARY:
#include <Core/GraphicsEngine/GraphicsEngine.h>
#include <Core/GraphicsEngine/Projection.h>
#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>
#include <Core/GraphicsEngine/RenderLayers/TextRenderLayer.h>

#include <Core/RenderHardwareInterface/Graphics.h>
#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/ImmediateDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/Objects/RenderTarget.h>
#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>
#include <Core/RenderHardwareInterface/DDSLoad.h>
#include <Core/RenderHardwareInterface/DX11/Window_DX11Impl.h>

#include <Core/ComponentSystem/ComponentSystem.h>

#include <Core/Engine/Components/TransformComponent.h>
#include <Core/Engine/Components/SpriteComponent.h>
#include <Core/Engine/Components/TextComponent.h>

#include <Core/Assets/FontRegistry.h>
#include <Core/Assets/ShaderRegistry.h>
#include <Core/Assets/TextureRegistry.h>

#include <fstream>

#define FILE_CATEGORY "Client.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->Client"

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
#define LOG_CATEGORY CLASS_CATEGORY"::Initialize"

		aArgs;
		aNumArgs;

		myWindow = aWindow;
		myEngine = std::make_unique<Engine>();


		// Initializing Engine
		if (not myEngine->Initialize(myWindow, L"../../../Assets/"))	//TODO: Use another way to get asset path.
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize Engine!");
			return false;
		}

		myEngine->GetGraphicsEngine().SetVSyncEnabled(true);


		// Loading Resources
		myFontRegistry = std::make_unique<assets::FontRegistry>();
		myShaderRegistry = std::make_unique<assets::ShaderRegistry>(myEngine->GetGraphicsEngine().GetDevice());
		myTextureRegistry = std::make_unique<assets::TextureRegistry>(myEngine->GetGraphicsEngine().GetDevice());

		myFontRegistry->Load("core", assets::AssetManager::GetAssetsPath() / "Font");
		myShaderRegistry->Load("core", assets::AssetManager::GetAssetsPath() / "Shader");
		myTextureRegistry->Load("core", assets::AssetManager::GetAssetsPath() / "Texture");

		// Setting up graphics layers
		SpriteRenderLayer* spriteRenderLayer = myEngine->GetGraphicsEngine().AddRenderLayer<SpriteRenderLayer>(myShaderRegistry.get());
		TextRenderLayer* renderLayer = myEngine->GetGraphicsEngine().AddRenderLayer<TextRenderLayer>(myShaderRegistry.get());

		graphics::PImpl<graphics::RenderTarget> renderTarget;
		myEngine->GetGraphicsEngine().GetDevice()->GetBufferRenderTarget(&renderTarget);
		graphics::PImpl<graphics::DepthStencil> depthStencil;
		myEngine->GetGraphicsEngine().GetDevice()->GetBufferDepthStencil(&depthStencil);

		renderLayer->SetRenderTarget(renderTarget.Get());
		spriteRenderLayer->SetRenderTarget(renderTarget.Get());

		renderLayer->SetDepthStencil(depthStencil.Get());
		spriteRenderLayer->SetDepthStencil(depthStencil.Get());

		renderLayer->SetProjection(CreateOrthographicProjection(0.f, 1.f, 0.f, 1.f, 0.f, 1.f));
		spriteRenderLayer->SetProjection(CreateOrthographicProjection(0.f, 1.f, 0.f, 1.f, 0.f, 1.f));

		return true;

#undef LOG_CATEGORY
	}

	bool Client::Shutdown()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Shutdown"

		bool result = true;
		if (not myEngine->Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown Client! Failed to shutdown Engine.");
			result = false;
		}
		myEngine.reset();
		myFontRegistry.reset();
		myShaderRegistry.reset();
		myTextureRegistry.reset();
		myExiting = false;

		return result;

#undef LOG_CATEGORY
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
			}

			if (myEngine->BeginFrame())
			{
				myEngine->EndFrame();
			}
			else
			{
				Exit();
			}
		}
	}

	void Client::Exit()
	{
		myExiting = true;
	}
}

#undef FILE_CATEGORY 
#undef CLASS_CATEGORY