#pragma once
#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>
#include <Implementations/CardGame/Game.h>

class SpriteComponent;
class TextComponent;
class Engine;

namespace CommonUtilities
{
	class InputManager;
}

namespace client
{
	class GameScene;

	class CardComponent : public cs::Component
	{
	public:
		CardComponent(GameScene* aScene, cardgame::CardID aCard, SpriteComponent* aSpriteFrontComponent, SpriteComponent* aSpriteBackComponent, TextComponent* aTitleTextComponent, TextComponent* aDescTextComponent);
		~CardComponent();

		void SetLocalPlayer(cardgame::PlayerID aLocalPlayer) noexcept;

		const cardgame::Card* GetCard() const noexcept;

		void SetDisplayOnly(bool aDisplayOnly) noexcept;
		bool IsDisplayOnly() const noexcept;

		bool IsHovered() const noexcept;

	protected:
		virtual void OnStart() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		GameScene* myScene;
		const CommonUtilities::InputManager* myInputManager;
		cardgame::Game* myGame;

		bool myDisplayOnly;

		cardgame::CardID myCard;
		cardgame::PlayerID myLocalPlayer;

		cs::ComponentRef<SpriteComponent> mySpriteFrontComponent;
		cs::ComponentRef<SpriteComponent> mySpriteBackComponent;
		cs::ComponentRef<TextComponent> myTitleTextComponent;
		cs::ComponentRef<TextComponent> myDescTextComponent;
	};
}