#pragma once
#include "../Components/ButtonComponent.h"

#include <Core/Engine/Scene.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

class TextComponent;

namespace client
{
	class ClientApp;

	class GameOverScene : public Scene
	{
	public:
		GameOverScene(ClientApp* aApp);
		~GameOverScene();

		void SetWinnerName(std::string_view aName);

	protected:
		virtual void OnInit() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		static void OnExitButton(const ButtonComponent::CallbackData& aData);

		cs::ComponentRef<TextComponent> myText;
		ClientApp* myApp;

		float myAspectRatio;
	};
}