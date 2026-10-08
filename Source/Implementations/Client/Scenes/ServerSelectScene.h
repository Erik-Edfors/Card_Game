#pragma once
#include "../Client.h"
#include <Core/Engine/Scene.h>
#include <Core/Networking/SocketAddress.h>

namespace client
{
	class ClientApp;

	class ServerSelectScene : public Scene
	{
	public:
		ServerSelectScene(ClientApp* aClient);
		~ServerSelectScene();

	protected:
		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		struct _ServerEntry
		{
			std::string HostName;
			std::string ServiceName;

			networking::SocketAddress Address;

			CommonUtilities::Contract<Client::ServerInfo> InfoContract;
			bool ContractFinished;
			Client::ServerInfo Info;
		};

		static void OnServerInfo(const Client::MessageCallbackData<shared::NM_ServerInfo>& aData);

		bool ExportServerList();
		bool LoadServerList();
		void ClearServerList();
		void AddServerEntry(std::string_view aHostName, std::string_view aServiceName);
		std::vector<_ServerEntry>::iterator RemoveServerEntry(const std::vector<_ServerEntry>::iterator& aWhere);
		void Connect(std::string_view aHostName, std::string_view aServiceName);

		ClientApp* myClient;

		std::string myHostInput;
		std::string myServiceInput;

		std::vector<_ServerEntry> myServerEntries;
		//std::unordered_map<networking::SocketAddress, size_t> myAddressServerMap;
	};
}