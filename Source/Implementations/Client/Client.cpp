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
#include <Core/RenderHardwareInterface/Graphics.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/DDSLoad.h>
#include <Core/RenderHardwareInterface/DX11/Window_DX11Impl.h>
#include <Core/Engine/ComponentSystem/ComponentSystem.h>
#include <Core/Engine/ComponentSystem/Components/TransformComponent.h>
#include <Core/Engine/ComponentSystem/Components/SpriteComponent.h>
#include <fstream>

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

		myEngine = std::make_unique<Engine>();
		myEngine->Initialize(aWindow);


		//TEMPORARY:
		//locSpriteRenderLayer = myEngine->GetGraphicsEngine().AddRenderLayer<SpriteRenderLayer>();
		SpriteRenderLayer* spriteRenderlayer = myEngine->GetGraphicsEngine().AddRenderLayer<SpriteRenderLayer>();
		graphics::PImpl<graphics::Device> device = myEngine->GetGraphicsEngine().GetDevice();

		graphics::PImpl<graphics::Texture2D> backBufferTexture;
		device->GetBuffer(&backBufferTexture);

		graphics::RenderTargetDesc rtDesc = {
			.Name = "Backbuffer RenderTarget",
			.Texture = backBufferTexture.Get(),
			.Format = graphics::GRAPHICS_FORMAT_UNKNOWN,
			.ViewDimension = graphics::GRAPHICS_RENDER_TARGET_VIEW_DIMENSION_TEXTURE2D,
			.FirstSlice = 0,
			.NumSlices = 1,
			.MipSlice = 0
		};

		graphics::PImpl<graphics::RenderTarget> renderTarget;
		device->CreateRenderTarget(&renderTarget, &rtDesc);

		std::ifstream file("../../Assets/Textures/Atlas.dds");

		graphics::PImpl<graphics::Texture2D> atlas;
		graphics::CreateTexture2DFromFile(
			device.Get(),
			"Sprite Atlas",
			"../../Assets/Textures/Atlas.dds",
			&atlas,
			graphics::GRAPHICS_BIND_SHADER_RESOURCE,
			graphics::GRAPHICS_RESOURCE_NONE,
			graphics::GRAPHICS_USAGE_DEFAULT,
			graphics::GRAPHICS_CPU_ACCESS_NONE
		);

		graphics::ShaderResourceDesc srDesc = {
			.Name = "Sprite Atlas ShaderResource",
			.Texture = atlas.Get(),
			.Format = graphics::GRAPHICS_FORMAT_UNKNOWN,
			.ViewDimension = graphics::GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION_TEXTURE2D,
			.FirstSlice = 0,
			.NumSlices = 1,
			.MipLevels = 1,
			.MostDetailedMip = 0
		};

		graphics::PImpl<graphics::ShaderResource> atlasResource;
		device->CreateShaderResource(&atlasResource, &srDesc);

		spriteRenderlayer->SetAtlas(atlasResource.Get());
		spriteRenderlayer->SetTarget(renderTarget.Get());
		spriteRenderlayer->SetView(CommonUtilities::Matrix4x4f());
		spriteRenderlayer->SetProjection(CreateOrthographicProjection(0.f, 1.f, 0.f, 1.f, 0.f, 1.f));

		myEngine->PushScene<client::LobbyScene>();

		ComponentSystem& compSys = myEngine->GetScene()->GetComponentSystem();
		ActorRef actor = compSys.CreateActor();
		ComponentRef<TransformComponent> transform = compSys.AddComponent<TransformComponent>(actor);
		ComponentRef<SpriteComponent> sprite = compSys.AddComponent<SpriteComponent>(actor);

		transform->SetScaleLocal({ 0.1f, 0.2f, 1.f });
		transform->SetTranslationLocal({ 0.5f, 0.5f, 0.f });

		sprite->SetTextureRect({ { 0.003173828125f, 0.001953125f }, { 0.003173828125f + 0.01318359375f, 0.001953125f + 0.025390625f } });

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
