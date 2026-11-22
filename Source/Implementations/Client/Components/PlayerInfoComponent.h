#pragma once
#include <Implementations/CardGame/Defines.h>
#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>
#include <Core/CommonUtilities/Events.hpp>

class SpriteComponent;
class TextComponent;

namespace cardgame
{
	class Game;
}

namespace client
{
	class PlayerComponent;

	class PlayerInfoComponent : public cs::Component
	{
	public:
		PlayerInfoComponent(cardgame::Game* aGame, cardgame::PlayerID aPlayer, SpriteComponent* aSprite, TextComponent* aText);
		~PlayerInfoComponent();

	protected:
		void OnInit() override;
		void OnStart() override;
		void OnUpdate(float aDeltaTime) override;

	private:
		void UpdateInfo();

		cardgame::Game* myGame;
		cardgame::PlayerID myPlayer;

		CommonUtilities::EventListener myGameListener;

		cs::ComponentRef<SpriteComponent> mySprite;
		cs::ComponentRef<TextComponent> myText;
	};
}