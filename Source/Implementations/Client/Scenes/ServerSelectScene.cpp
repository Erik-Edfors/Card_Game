#include "Client.pch.h"
#include "ServerSelectScene.h"
#include "../Client.h"
#include "../ClientApp.h"

#include <Core/Assets/AssetManager.h>

#include <Core/imgui/imgui.h>
#include <Core/imgui/imgui_internal.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

#include <Core/CommonUtilities/StringHelper.h>

#define RAPIDJSON_NOMEMBERITERATORCLASS
#pragma warning(push)
#pragma warning(disable : 5054)
#include <External/rapidjson/document.h>
#include <External/rapidjson/istreamwrapper.h>
#pragma warning(pop)

namespace client
{
	ServerSelectScene::ServerSelectScene(ClientApp* aClient) :
		myClient(aClient)
	{
	}
	ServerSelectScene::~ServerSelectScene()
	{
	}
	void ServerSelectScene::OnInit()
	{
		constexpr const wchar_t* serverEntriesPath = L"Cache/Servers.json";

		if (not std::filesystem::exists(serverEntriesPath))
		{
			std::filesystem::create_directories(serverEntriesPath);
			std::wofstream serverEntriesFile(serverEntriesPath);
			serverEntriesFile << L"{\n}" << std::endl;
			serverEntriesFile.close();
		}
		std::ifstream serverEntriesFile(serverEntriesPath);

		rapidjson::IStreamWrapper stream(serverEntriesFile);

		rapidjson::Document doc;
		doc.ParseStream(stream);

		bool reset = false;

		reset |= doc.HasParseError();
		if (not reset)
		{
			reset |= not doc.HasMember("Servers");
		}
		if (not reset)
		{
			reset |= not doc["Servers"].IsArray();
		}

		if (reset)
		{
			doc = {};
			rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
			rapidjson::Value servers;
			servers.SetArray();
			doc.AddMember("Servers", servers, allocator);
		}

		rapidjson::GenericArray<false, rapidjson::Value> array = doc["Servers"].GetArray();
		for (rapidjson::Value& value : array)
		{
			if (not value.HasMember("HostName") || not value.HasMember("ServiceName"))
			{
				continue;
			}
			if (not value["HostName"].IsString() || not value["ServiceName"].IsString())
			{
				continue;
			}

			_ServerEntry entry = {
				.HostName = value["HostName"].GetString(),
				.ServiceName = value["ServiceName"].GetString()
			};
			myServerEntries.push_back(std::move(entry));
		}
	}
	void ServerSelectScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		const ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		ImGui::SetNextWindowPos({ 0.f, 0.f });
		//ImGui::SetNextWindowDockID(0);
		if (ImGui::Begin("Server Select", nullptr, flags))
		{
			Client& client = myClient->GetClient();

			switch (client.GetConnectionStatus())
			{
			case CLIENT_CONNECTION_DISCONNECTED:
			{
				const float width = ImGui::GetWindowWidth();
				ImGui::SetNextItemWidth(width * 0.3f);
				ImGui::InputText("##Host", &myHostInput, ImGuiInputTextFlags_AutoSelectAll);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(width * 0.3f);
				ImGui::InputText("##Service", &myServiceInput, ImGuiInputTextFlags_AutoSelectAll);

				ImGui::SameLine();
				if (ImGui::Button("Connect", { -1.f, 0.f }))
				{
					std::wstring hostName = CommonUtilities::UTF8(myHostInput);
					std::wstring serviceName = L"54322";
					if (not myServiceInput.empty())
					{
						serviceName = CommonUtilities::UTF8(myServiceInput);
					}

					networking::SocketAddress address;
					if (networking::GetSocketAddress(&address, hostName, serviceName, networking::NETWORKING_ADDRESS_FAMILY_UNSPECIFIED, networking::NETWORKING_SOCKET_TYPE_DGRAM, networking::NETWORKING_INTERNET_PROTOCOL_UDP))
					{
						client.RequestConnection(address);
					}
				}

				int id = 0;

				auto it = myServerEntries.begin();
				while (it != myServerEntries.end())
				{
					const _ServerEntry& entry = *it;

					ImGui::PushID(id++);

					const ImGuiStyle& style = ImGui::GetStyle();
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

					ImGui::BeginChild("##Host Name Box", { width * 0.3f, 0.f }, ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY);
					ImGui::Text(entry.HostName.c_str());
					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginChild("##Service Name Box", { width * 0.3f, 0.f }, ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY);
					ImGui::Text(entry.ServiceName.c_str());
					ImGui::EndChild();
					ImGui::SameLine();

					ImGui::PopStyleVar();

					const float remainingWidth = width - ImGui::GetCursorPosX();
					const float padding = style.ItemSpacing.x;
					if (ImGui::Button("Connect", { 0.5f * remainingWidth - 2.f * padding, 0.f }))
					{
						networking::SocketAddress address;
						if (networking::GetSocketAddress(&address, CommonUtilities::UTF8(entry.HostName), CommonUtilities::UTF8(entry.ServiceName), networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4, networking::NETWORKING_SOCKET_TYPE_DGRAM, networking::NETWORKING_INTERNET_PROTOCOL_UDP))
						{
							client.RequestConnection(address);
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("Remove", { -1.f, 0.f }))
					{
						it = myServerEntries.erase(it);
						//TODO: Update cache file.
					}
					else
					{
						it = std::next(it);
					}
					ImGui::PopID();
				}
			}
			break;
			case CLIENT_CONNECTION_PENDING:
			{
				ImGui::BeginDisabled();
				ImGui::Button("Connecting...", { -1.f, 0.f });
				ImGui::EndDisabled();
			}
			break;
			case CLIENT_CONNECTION_CONNECTED:
			{
				ImGui::Button("Connected!", { -1.f, 0.f });
			}
			break;
			}
		}
		ImGui::End();

	}
}
