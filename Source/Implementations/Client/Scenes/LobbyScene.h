#pragma once
#include <Core/Engine/Scene.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

class CameraComponent;

namespace client
{
	class Client;

	class LobbyScene : public Scene
	{
	public:
		LobbyScene(Client* aClient);

	protected:
		virtual void OnInit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		Client* myClient;
		cs::ComponentRef<CameraComponent> myCamera;
	};
}