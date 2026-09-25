#pragma once
#include <Core/Engine/Scene.h>
#include <Implementations/Shared/Console.h>
#include <Implementations/Shared/CommandParser.h>

namespace server
{
	class Server;

	class MainScene : public Scene
	{
	public:
		MainScene(server::Server* aServer);
		~MainScene();

		shared::Console& GetConsole() noexcept;
		shared::CommandParser& GetCommandParser() noexcept;

	protected:
		virtual void OnInit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		Server* myServer;
		shared::Console myConsole;
		shared::CommandParser myCommandParser;
	};
}