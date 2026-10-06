#pragma once
#include <Core/Engine/Scene.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

class CameraComponent;

namespace client
{
	class ClientApp;

	class LobbyScene : public Scene
	{
	public:
		LobbyScene(ClientApp* aClient);

	protected:
		virtual void OnInit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		ClientApp* myClient;
		cs::ComponentRef<CameraComponent> myCamera;
	};
}