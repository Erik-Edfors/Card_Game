#include "Client.pch.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "ClientApp.h"
#include "Client.h"
#include "Scenes/LobbyScene.h"
#include "Scenes/ServerSelectScene.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/Console.h>
#include <Core/Engine/CommandParser.h>

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

#include <Core/imgui/imgui.h>

#include <Core/Assets/FontRegistry.h>
#include <Core/Assets/ShaderRegistry.h>
#include <Core/Assets/TextureRegistry.h>

#include <Implementations/Shared/Network/NetworkManager.h>
#include <Implementations/Shared/Network/Messages.h>

#include <Core/Networking/Networking.h>

#include <Core/CommonUtilities/ArgsParser.hpp>

#include <fstream>

#define FILE_CATEGORY "ClientApp.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->ClientApp"

namespace client
{
	static void Ping(const char* const* aArguments, size_t aNumArguments, void* aUserData)
	{
		aArguments;
		aNumArguments;

		ClientApp& client = *reinterpret_cast<ClientApp*>(aUserData);

		networking::SocketAddress address = {
			.Port = 54322,
			.IPAddress = networking::NETWORKING_IPV4_LOOPBACK
		};

		shared::NM_Ping ping;
		shared::NetworkManager::NetworkMessage message;
		client.GetClient().CreateMessage(&message, ping);
		client.GetClient().SendTo(message, address);
	}

	static void Connect(const char* const* aArguments, size_t aNumArguments, void* aUserData)
	{
		aArguments;
		aNumArguments;

		ClientApp& client = *reinterpret_cast<ClientApp*>(aUserData);

		networking::SocketAddress address = {
			.Port = 54322,
			.IPAddress = networking::NETWORKING_IPV4_LOOPBACK
		};

		CommonUtilities::ArgsParser parser;
		parser.Parse(aArguments, aNumArguments);

		const CommonUtilities::Argument* arguments;
		size_t numArguments;
		parser.GetArguments(&arguments, &numArguments);
		if (numArguments >= 3)
		{
			const std::string_view hostName = arguments[1].GetValue();
			const std::string_view serviceName = arguments[2].GetValue();

			const bool suceeded = networking::GetSocketAddress(
				&address,
				hostName,
				serviceName,
				networking::NETWORKING_ADDRESS_FAMILY_UNSPECIFIED,
				networking::NETWORKING_SOCKET_TYPE_DGRAM,
				networking::NETWORKING_INTERNET_PROTOCOL_UDP
			);
			if (not suceeded)
			{
				return;
			}
		}

		client.GetClient().RequestConnection(address);
	}

	static void Status(const char* const* aArguments, size_t aNumArguments, void* aUserData)
	{
		aArguments;
		aNumArguments;

		ClientApp& client = *reinterpret_cast<ClientApp*>(aUserData);
		Console& console = client.GetEngine().GetConsole();

		static const std::unordered_map<unsigned int, std::string> connectionStatusNames = {
			{ CLIENT_CONNECTION_DISCONNECTED, "Disconnected" },
			{ CLIENT_CONNECTION_PENDING, "Pending" },
			{ CLIENT_CONNECTION_CONNECTED, "Connected" }
		};

		console.PrintLine(std::format("Connection Status: {}.", connectionStatusNames.at(client.GetClient().GetConnectionStatus())));
		console.PrintLine(std::format("Remote: {}", networking::to_string(client.GetClient().GetRemote())));
	}

	static void OnPing(const Client::MessageCallbackData<shared::NM_Ping>& aData)
	{
		ClientApp& client = *reinterpret_cast<ClientApp*>(aData.UserData);

		client.GetEngine().GetConsole().PrintLine("Recieved Ping!");
	}

	ClientApp::ClientApp() :
		myExiting(false),
		myWindow(NULL)
	{
	}

	ClientApp::~ClientApp()
	{
	}

	bool ClientApp::ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		if (myEngine && myEngine->ProcessWindowsMessage(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}

		switch (uMsg)
		{
		case WM_CLOSE: case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
		case WM_SIZE:
		{
			myEngine->GetGraphicsEngine().GetDevice()->UpdateBackbufferSize(LOWORD(lParam), HIWORD(lParam));
		}
		break;
		}

		return false;
	}

	bool ClientApp::Initialize(HWND aWindow, const wchar_t* const* aArgs, size_t aNumArgs)
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Initialize"

		aArgs;
		aNumArgs;

		myWindow = aWindow;
		myEngine = std::make_unique<Engine>();
		myClient = std::make_unique<Client>();

		if (not networking::Initialize())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize networking!");
			return false;
		}

		// Initializing Engine
		if (not myEngine->Initialize(myWindow, L"../../../Assets/"))	//TODO: Get asset path from config file.
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize Engine!");
			return false;
		}

		if (not myClient->Initialize(54321)) //TODO: Allow user selected port
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize NetworkManager!");
			return false;
		}

		myEngine->GetGraphicsEngine().SetVSyncEnabled(true);

#ifdef _DEBUG
		myEngine->SetConsoleEnabled(true);
#endif

		myEngine->GetCommandParser().AddCommand(
			"ping",
			&Ping,
			this
		);
		myEngine->GetCommandParser().AddCommand(
			"connect",
			&Connect,
			this
		);
		myEngine->GetCommandParser().AddCommand(
			"status",
			&Status,
			this
		);

		myClient->SetMessageCallback<shared::NM_Ping>(&OnPing, this);

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

		myEngine->PushScene<ServerSelectScene>(this);

		return true;

#undef LOG_CATEGORY
	}

	bool ClientApp::Shutdown()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::Shutdown"

		bool result = true;
		if (not myEngine->Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown Engine.");
			result = false;
		}
		if (not myClient->Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown NetworkManager.");
			result = false;
		}
		if (not networking::Shutdown())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to shutdown networking.");
			result = false;
		}
		myEngine.reset();
		myClient.reset();
		myFontRegistry.reset();
		myShaderRegistry.reset();
		myTextureRegistry.reset();
		myExiting = false;

		return result;

#undef LOG_CATEGORY
	}

	void ClientApp::Run()
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
				ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode, NULL);

				myEngine->Update();

				myClient->Update();

				myEngine->EndFrame();
			}
			else
			{
				Exit();
			}
		}
	}

	void ClientApp::Exit()
	{
		myExiting = true;
	}

	assets::FontRegistry& ClientApp::GetFontRegistry() const noexcept
	{
		return *myFontRegistry;
	}
	assets::ShaderRegistry& ClientApp::GetShaderRegistry() const noexcept
	{
		return *myShaderRegistry;
	}
	assets::TextureRegistry& ClientApp::GetTextureRegistry() const noexcept
	{
		return *myTextureRegistry;
	}
	Engine& ClientApp::GetEngine() const noexcept
	{
		return *myEngine;
	}
	Client& ClientApp::GetClient() const noexcept
	{
		return *myClient;
	}
}

#undef FILE_CATEGORY 
#undef CLASS_CATEGORY