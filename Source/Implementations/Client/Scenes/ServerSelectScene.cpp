#include "Client.pch.h"
#include "ServerSelectScene.h"
#include "LobbyScene.h"
#include "../Client.h"
#include "../ClientApp.h"

#include <Core/Engine/Engine.h>

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
#include <External/rapidjson/ostreamwrapper.h>
#include <External/rapidjson/prettywriter.h>
#pragma warning(pop)

#define FILE_CATEGORY "ServerSelectScene.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->ServerSelectScene"

namespace client
{
	static constexpr const wchar_t* ServersPath = L"Cache/Servers.json";

	ServerSelectScene::ServerSelectScene(ClientApp* aClient) :
		myClient(aClient)
	{
	}
	ServerSelectScene::~ServerSelectScene()
	{
	}
	void ServerSelectScene::OnInit()
	{
		LoadServerList();
		//myClient->GetClient().SetMessageCallback<shared::NM_ServerInfo>(&OnServerInfo, this, true);
	}
	void ServerSelectScene::OnShutdown()
	{
		myClient->GetClient().SetMessageCallback<shared::NM_ServerInfo>(nullptr, nullptr);
	}
	void ServerSelectScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		const ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::SetNextWindowSize(ImGui::GetWindowViewport()->Size);
		ImGui::SetNextWindowPos({ 0.f, 0.f });
		//ImGui::SetNextWindowDockID(1);
		if (ImGui::Begin("##Server Select", nullptr, flags))
		{
			Client& client = myClient->GetClient();

			const float availWidth = ImGui::GetContentRegionAvail().x;
			const ImGuiStyle& style = ImGui::GetStyle();

			switch (client.GetConnectionStatus())
			{
			case CLIENT_CONNECTION_DISCONNECTED:
			{
				ImGui::Text("Direct Connect");

				ImGui::BeginChild("Direct Connect", { -1.f, 0.f }, ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
				{
					const float itemWidth = (availWidth - 2.f * style.ItemSpacing.x) * 0.3f;
					ImGui::SetNextItemWidth(itemWidth);
					ImGui::InputText("##Host", &myHostInput, ImGuiInputTextFlags_AutoSelectAll);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(itemWidth);
					ImGui::InputText("##Service", &myServiceInput, ImGuiInputTextFlags_AutoSelectAll);

					ImGui::SameLine();

					const float remainingWidth = availWidth - ImGui::GetCursorPosX();
					const float buttonWidth = (remainingWidth - style.ItemSpacing.x) * 0.5f;

					if (ImGui::Button("Connect", { buttonWidth, 0.f }))
					{
						Connect(myHostInput, myServiceInput);
					}
					ImGui::SameLine();
					if (ImGui::Button("Save", { -1.f, 0.f }))
					{
						AddServerEntry(myHostInput, myServiceInput);

						myHostInput.clear();
						myServiceInput.clear();
						ExportServerList();
					}
				}
				ImGui::EndChild();


				ImGui::Text("Server List");
				ImGui::BeginChild("Server List", { -1.f, -1.f }, ImGuiChildFlags_Borders);

				int id = 0;
				auto it = myServerEntries.begin();
				while (it != myServerEntries.end())
				{
					_ServerEntry& entry = *it;

					if (not entry.ContractFinished)
					{
						CommonUtilities::CONTRACT_STATUS status = entry.InfoContract.GetStatus();

						switch (status)
						{
						case CommonUtilities::CONTRACT_STATUS_WAITING:
							break;
						case CommonUtilities::CONTRACT_STATUS_CANCELLED:
							entry.InfoContract = myClient->GetClient().RequestServerInfo(entry.Address, 5.f);
							break;
						case CommonUtilities::CONTRACT_STATUS_FINISHED:
							entry.InfoContract.Wait(&entry.Info);
							entry.ContractFinished = true;
							break;
						}
					}

					ImGui::PushID(id++);

					ImGui::BeginChild("##Server Desc", { -1.f, 0.f }, ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);

					const float itemWidth = (availWidth - 2.f * style.ItemSpacing.x) * 0.3f;

					const ImGuiChildFlags textChildFlags = ImGuiChildFlags_FrameStyle
						| ImGuiChildFlags_AutoResizeY;

					ImGui::BeginChild("##Host Name Box", { itemWidth, 0.f }, textChildFlags);
					ImGui::Text(entry.HostName.c_str());
					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginChild("##Service Name Box", { itemWidth, 0.f }, textChildFlags);
					ImGui::Text(entry.ServiceName.c_str());
					ImGui::EndChild();
					ImGui::SameLine();

					//ImGui::PopStyleVar();

					const float remainingWidth = availWidth - ImGui::GetCursorPosX();
					const float buttonWidth = (remainingWidth - style.ItemSpacing.x) * 0.5f;
					if (ImGui::Button("Connect", { buttonWidth, 0.f }))
					{
						Connect(entry.HostName, entry.ServiceName);
					}
					ImGui::SameLine();
					if (ImGui::Button("Remove", { -1.f, 0.f }))
					{
						it = RemoveServerEntry(it);
						ExportServerList();
					}
					else
					{
						it = std::next(it);
					}

					ImGui::Text(entry.Info.Name.c_str());
					ImGui::Text(entry.Info.Description.c_str());
					ImGui::EndChild();

					ImGui::PopID();
				}

				ImGui::EndChild();
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
				GetEngine().ChangeScene<LobbyScene>(myClient);
			}
			break;
			}
		}
		ImGui::End();

	}
	//void ServerSelectScene::OnServerInfo(const Client::MessageCallbackData<shared::NM_ServerInfo>& aData)
	//{
	//	ServerSelectScene& scene = *reinterpret_cast<ServerSelectScene*>(aData.UserData);

	//	auto it = scene.myAddressServerMap.find(aData.Address);
	//	if (it != scene.myAddressServerMap.end())
	//	{
	//		const size_t index = it->second;
	//		_ServerEntry& entry = scene.myServerEntries[index];
	//		entry.Name = aData.Message.Name;
	//		entry.Description = aData.Message.Description;
	//	}
	//}
	bool ServerSelectScene::ExportServerList()
	{
#define LOG_CATEGORY CLASS_CATEGORY"::ExportServerList"

		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		rapidjson::Value servers;
		servers.SetArray();
		for (const _ServerEntry& entry : myServerEntries)
		{
			rapidjson::Value server;
			server.SetObject();
			server.AddMember("HostName", rapidjson::StringRef(entry.HostName.c_str()), allocator);
			server.AddMember("ServiceName", rapidjson::StringRef(entry.ServiceName.c_str()), allocator);

			servers.PushBack(server.Move(), allocator);
		}

		doc.SetObject();
		doc.AddMember("Servers", servers.Move(), allocator);

		std::ofstream file(ServersPath);
		if (not file.is_open() || file.fail())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to export server list! Could not write to file.");
			return false;
		}

		rapidjson::OStreamWrapper stream(file);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(stream);

		if (not doc.Accept(writer))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to export server list! Failed to write document.");
			return false;
		}

		return true;

#undef LOG_CATEGORY
	}
	bool ServerSelectScene::LoadServerList()
	{
		ClearServerList();

		if (not std::filesystem::exists(ServersPath))
		{
			std::filesystem::create_directories(ServersPath);
			std::wofstream serverEntriesFile(ServersPath);
			serverEntriesFile << L"{\n}" << std::endl;
			serverEntriesFile.close();
		}
		std::ifstream serverEntriesFile(ServersPath);

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
			doc.SetObject();
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

			std::string rawHostName = value["HostName"].GetString();
			std::string rawServiceName = value["ServiceName"].GetString();

			AddServerEntry(rawHostName, rawServiceName);
		}

		return true;
	}
	void ServerSelectScene::ClearServerList()
	{
		myServerEntries.clear();
		//myAddressServerMap.clear();
	}
	void ServerSelectScene::AddServerEntry(std::string_view aHostName, std::string_view aServiceName)
	{
		const std::wstring hostName = CommonUtilities::ToWide(aHostName, CommonUtilities::STRING_ENCODING_UTF8);
		const std::wstring serviceName = CommonUtilities::ToWide(aServiceName, CommonUtilities::STRING_ENCODING_UTF8);

		networking::SocketAddress address = {};

		bool succeeded = networking::GetSocketAddress(
			&address,
			hostName,
			serviceName,
			networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4,
			networking::NETWORKING_SOCKET_TYPE_DGRAM,
			networking::NETWORKING_INTERNET_PROTOCOL_UDP
		);
		//if (succeeded)
		//{
		//	myAddressServerMap[address] = myServerEntries.size();

		//	client::Client& client = myClient->GetClient();
		//	shared::NetworkManager::NetworkMessage message;
		//	client.CreateMessage(
		//		&message,
		//		shared::NM_ServerInfoRequest{ }
		//	);
		//	client.SendTo(message, address);
		//}
		_ServerEntry entry = {
			.HostName = aHostName.data(),
			.ServiceName = aServiceName.data(),
			.Address = address,
			.InfoContract = {},
			.ContractFinished = false,
			.Info = {
				.Name = "Loading...",
				.Description = "Loading..."
			}
		};

		if (succeeded)
		{
			entry.InfoContract = myClient->GetClient().RequestServerInfo(address, 5.f);
		}

		myServerEntries.push_back(std::move(entry));
	}
	std::vector<ServerSelectScene::_ServerEntry>::iterator ServerSelectScene::RemoveServerEntry(const std::vector<_ServerEntry>::iterator& aWhere)
	{
		//myAddressServerMap.erase(aWhere->Address);
		return myServerEntries.erase(aWhere);
	}
	void ServerSelectScene::Connect(std::string_view aHostName, std::string_view aServiceName)
	{
		std::wstring hostName = CommonUtilities::ToWide(aHostName, CommonUtilities::STRING_ENCODING_UTF8);
		std::wstring serviceName = L"54322";
		if (not aServiceName.empty())
		{
			serviceName = CommonUtilities::ToWide(aServiceName, CommonUtilities::STRING_ENCODING_UTF8);
		}

		networking::SocketAddress address;
		if (networking::GetSocketAddress(&address, hostName, serviceName, networking::NETWORKING_ADDRESS_FAMILY_INTERNET_PROTOCOL_4, networking::NETWORKING_SOCKET_TYPE_DGRAM, networking::NETWORKING_INTERNET_PROTOCOL_UDP))
		{
			myClient->GetClient().RequestConnection(address);
		}
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY