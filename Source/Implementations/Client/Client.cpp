#include "Client.pch.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Client.h"
#include "Scenes/LobbyScene.h"


//TEMPORARY:
#include <Core/GraphicsEngine/GraphicsEngine.h>
#include <Core/GraphicsEngine/Projection.h>
#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>
#include <Core/RenderHardwareInterface/Graphics.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/DDSLoad.h>
#include <Core/RenderHardwareInterface/DX11/Window_DX11Impl.h>
#include <fstream>

static graphics::PImpl<graphics::Window_DX11Impl> locWindow = nullptr;
static GraphicsEngine* locGraphicsEngine = nullptr;
static SpriteRenderLayer* locSpriteRenderLayer = nullptr;
static graphics::PImpl<graphics::RenderTarget> locRenderTarget = nullptr;

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

		//TEMPORARY:
		locWindow = new graphics::Window_DX11Impl(aWindow);
		graphics::PImpl<graphics::Device> device;
		graphics::CreateDevice(&device, locWindow.Get(), graphics::GRAPHICS_DEVICE_ALLOW_TEARING | graphics::GRAPHICS_DEVICE_DEBUG | graphics::GRAPHICS_DEVICE_VIDEO_SUPPORT | graphics::GRAPHICS_DEVICE_BGRA_SUPPORT);
		locGraphicsEngine = new GraphicsEngine(device.Get());
		locGraphicsEngine->Initialize();

		locSpriteRenderLayer = locGraphicsEngine->AddRenderLayer<SpriteRenderLayer>();

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

		device->CreateRenderTarget(&locRenderTarget, &rtDesc);

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

		locSpriteRenderLayer->SetAtlas(atlasResource.Get());
		locSpriteRenderLayer->SetTarget(locRenderTarget.Get());
		locSpriteRenderLayer->SetView(CommonUtilities::Matrix4x4f());
		locSpriteRenderLayer->SetProjection(CreateOrthographicProjection(0.f, 1.f, 0.f, 1.f, 0.f, 1.f));

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

			}

			static CommonUtilities::Vector2f pos = { 0.f, 0.f };
			pos += { 0.00001f, 0.00001f };

			const Sprite sprite = {
				.Position = pos,
				.Size = { 0.1f, 0.2f },
				.Pivot = { 0.5f, 0.5f },
				.Rotation = 0.f,

				.Tint = { 1.f, 1.f, 1.f, 1.f },
				.Roughness = 1.f,
				.Metallicity = 0.f,

				.TextureRect = {
					{ 0.003173828125f, 0.001953125f },
					{ 0.003173828125f + 0.01318359375f, 0.001953125f + 0.025390625f }
				},
				.Depth = 0.5f
			};
			locSpriteRenderLayer->Draw(sprite);

			myScene->Update();
			locGraphicsEngine->Render();
		}
	}
	void Client::Exit()
	{
		myExiting = true;
	}
}
