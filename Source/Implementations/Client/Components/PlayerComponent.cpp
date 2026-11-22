#include "Client.pch.h"
#include "PlayerComponent.h"
#include "Scenes/GameScene.h"
#include "Locations/DeckComponent.h"
#include "Locations/HandComponent.h"
#include "Locations/PlayComponent.h"
#include "Locations/DiscardComponent.h"
#include "PlayerInfoComponent.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/Components/SpriteComponent.h>

#include <Implementations/CardGame/Task.h>

#include <Core/CommonUtilities/InputManager.h>

namespace client
{
	PlayerComponent::PlayerComponent(
		GameScene* aScene,
		cardgame::PlayerID aID,
		DeckComponent* aDeck,
		HandComponent* aHand,
		PlayComponent* aPlay,
		DiscardComponent* aDiscard,
		PlayerInfoComponent* aPlayerInfo,
		SpriteComponent* aSprite
	) :
		myScene(aScene),
		myID(aID),
		myDeck(aDeck),
		myHand(aHand),
		myPlay(aPlay),
		myDiscard(aDiscard),
		myPlayerInfo(aPlayerInfo),
		mySprite(aSprite)
	{
	}
	DeckComponent* PlayerComponent::GetDeck() const noexcept
	{
		return myDeck.Get();
	}
	HandComponent* PlayerComponent::GetHand() const noexcept
	{
		return myHand.Get();
	}
	PlayComponent* PlayerComponent::GetPlay() const noexcept
	{
		return myPlay.Get();
	}
	DiscardComponent* PlayerComponent::GetDiscard() const noexcept
	{
		return myDiscard.Get();
	}

	PlayerInfoComponent* PlayerComponent::GetPlayerInfo() const noexcept
	{
		return myPlayerInfo.Get();
	}

	cardgame::PlayerID PlayerComponent::GetID() const noexcept
	{
		return myID;
	}
	cardgame::Game& PlayerComponent::GetGame() const noexcept
	{
		return myScene->GetGame();
	}

	//TextComponent* PlayerComponent::GetHealthText() const noexcept
	//{
	//	return myHealthText.Get();
	//}
	//TextComponent* PlayerComponent::GetResourcesText() const noexcept
	//{
	//	return myResourcesText.Get();
	//}
	void PlayerComponent::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		const bool selected = myScene->IsPlayerSelected(myID);

		CommonUtilities::Vector4f tint = { 1.f, 1.f, 1.f, 1.f };

		if (selected)
		{
			tint = { 0.75f, 1.f, 0.75f, 1.f };
			if (mySprite->IsHovered())
			{
				tint = { 0.5f, 0.75f, 0.5f, 1.f };
				const CommonUtilities::InputManager& input = myScene->GetEngine().GetInputManager();
				if (input.GetKeyDown(CommonUtilities::Keys::MOUSELBUTTON))
				{
					myScene->UnselectPlayer(myID);
				}
			}
		}
		else
		{
			const bool validForSelection = myScene->CanPlayerBeSelected(myID);
			if (validForSelection)
			{
				if (mySprite->IsHovered())
				{
					tint = { 1.f, 1.f, 0.5f, 1.f };
					const CommonUtilities::InputManager& input = myScene->GetEngine().GetInputManager();
					if (input.GetKeyDown(CommonUtilities::Keys::MOUSELBUTTON))
					{
						myScene->SelectPlayer(myID);
					}
				}
				else
				{
					tint = { 1.f, 1.f, 0.75f, 1.f };
				}
			}
		}

		mySprite->SetTint(tint);

		//const cardgame::Player* player = myGame->GetPlayer(myID);
		//myHealthText->SetText(std::format("HP: {}", player->GetHealth()).c_str());
		//myResourcesText->SetText(std::format("Resources: {}", player->GetResourceCount()).c_str());
	}
	void PlayerComponent::OnDestroy()
	{
		if (myDeck) GetSystem().Destroy(myDeck->GetEntityHandle());
		if (myHand) GetSystem().Destroy(myHand->GetEntityHandle());
		if (myPlay) GetSystem().Destroy(myPlay->GetEntityHandle());
		if (myDiscard) GetSystem().Destroy(myDiscard->GetEntityHandle());
		if (myPlayerInfo) GetSystem().Destroy(myPlayerInfo->GetEntityHandle());
		if (mySprite) GetSystem().Destroy(mySprite->GetEntityHandle());
	}
}