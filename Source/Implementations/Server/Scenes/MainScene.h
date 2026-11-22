#pragma once
#include <Core/Engine/Scene.h>
//#include <Implementations/Shared/Console.h>
//#include <Implementations/Shared/CommandParser.h>

namespace server
{
	class ServerApp;

	class MainScene : public Scene
	{
	public:
		MainScene(server::ServerApp* aServer);
		~MainScene();

		//shared::Console& GetConsole() noexcept;
		//shared::CommandParser& GetCommandParser() noexcept;

	protected:
		virtual void OnInit() override;
		virtual void OnEnter() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		std::string myServerPortName;

		ServerApp* myServer;
		//shared::Console myConsole;
		//shared::CommandParser myCommandParser;
	};
}