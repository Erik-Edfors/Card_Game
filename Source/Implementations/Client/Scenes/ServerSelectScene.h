#pragma once
#include <Core/Engine/Scene.h>

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
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		ClientApp* myClient;

		std::string myHostInput;
		std::string myServiceInput;

		struct _ServerEntry
		{
			std::string HostName;
			std::string ServiceName;
		};

		std::vector<_ServerEntry> myServerEntries;
	};
}