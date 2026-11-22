#pragma once
#include <Core/Engine/Scene.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

#include "../Client.h"

class CameraComponent;

namespace client
{
	class Client;
	class ClientApp;
	class GameScene;

	class LobbyScene : public Scene
	{
	public:
		LobbyScene(ClientApp* aApp);

		Client& GetClient() const noexcept;

	protected:
		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		//static void OnPlayerInfo(const Client::MessageCallbackData<shared::NM_PlayerInfo>& aData);
		static void OnStartGame(const Client::MessageCallbackData<shared::NM_StartGame>& aData);

		std::unique_ptr<GameScene> myGameScene;

		std::string myNameInput;

		ClientApp* myApp;
		cs::ComponentRef<CameraComponent> myCamera;
	};
}