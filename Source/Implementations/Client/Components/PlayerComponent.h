#pragma once
#include <Implementations/CardGame/Game.h>

#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

class SpriteComponent;

namespace client
{
	class GameScene;

	class DeckComponent;
	class HandComponent;
	class PlayComponent;
	class DiscardComponent;
	class PlayerInfoComponent;

	class PlayerComponent : public cs::Component
	{
	public:
		PlayerComponent(
			GameScene* aScene,
			cardgame::PlayerID aID, 
			DeckComponent* aDeck, 
			HandComponent* aHand, 
			PlayComponent* aPlay, 
			DiscardComponent* aDiscard,
			PlayerInfoComponent* aPlayerInfo,
			SpriteComponent* aSprite
		);

		DeckComponent* GetDeck() const noexcept;
		HandComponent* GetHand() const noexcept;
		PlayComponent* GetPlay() const noexcept;
		DiscardComponent* GetDiscard() const noexcept;
		PlayerInfoComponent* GetPlayerInfo() const noexcept;
		//TextComponent* GetHealthText() const noexcept;
		//TextComponent* GetResourcesText() const noexcept;

		cardgame::PlayerID GetID() const noexcept;
		cardgame::Game& GetGame() const noexcept;

	protected:
		void OnUpdate(float aDeltaTime) override;
		void OnDestroy() override;

	private:
		GameScene* myScene;
		cardgame::PlayerID myID;
		cs::ComponentRef<DeckComponent> myDeck;
		cs::ComponentRef<HandComponent> myHand;
		cs::ComponentRef<PlayComponent> myPlay;
		cs::ComponentRef<DiscardComponent> myDiscard;
		cs::ComponentRef<PlayerInfoComponent> myPlayerInfo;
		cs::ComponentRef<SpriteComponent> mySprite;
	};
}