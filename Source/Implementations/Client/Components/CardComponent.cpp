#include "Client.pch.h"
#include "CardComponent.h"
#include "Scenes/GameScene.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/Components/SpriteComponent.h>
#include <Core/Engine/Components/TextComponent.h>

#include <Core/CommonUtilities/StringHelper.h>
#include <Core/CommonUtilities/InputManager.h>

namespace client
{
	CardComponent::CardComponent(GameScene* aScene, cardgame::CardID aCard, SpriteComponent* aSpriteFrontComponent, SpriteComponent* aSpriteBackComponent, TextComponent* aTitleTextComponent, TextComponent* aDescTextComponent) :
		myScene(aScene),
		myInputManager(&aScene->GetEngine().GetInputManager()),
		myGame(&aScene->GetGame()),
		myDisplayOnly(false),
		myCard(aCard),
		myLocalPlayer(cardgame::INVALID_PLAYER_ID),
		mySpriteFrontComponent(aSpriteFrontComponent),
		mySpriteBackComponent(aSpriteBackComponent),
		myTitleTextComponent(aTitleTextComponent),
		myDescTextComponent(aDescTextComponent)
	{
	}
	CardComponent::~CardComponent()
	{
	}

	void CardComponent::SetLocalPlayer(cardgame::PlayerID aLocalPlayer) noexcept
	{
		myLocalPlayer = aLocalPlayer;
	}

	const cardgame::Card* CardComponent::GetCard() const noexcept
	{
		return myGame->GetCard(myCard);
	}

	void CardComponent::SetDisplayOnly(bool aDisplayOnly) noexcept
	{
		myDisplayOnly = aDisplayOnly;
	}
	bool CardComponent::IsDisplayOnly() const noexcept
	{
		return myDisplayOnly;
	}

	bool CardComponent::IsHovered() const noexcept
	{
		return mySpriteFrontComponent->IsHovered() || mySpriteBackComponent->IsHovered();
	}


	void CardComponent::OnStart()
	{
		const cardgame::Card* card = GetCard();

		if (card != nullptr)
		{
			std::wstring title = CommonUtilities::ToWide(card->GetName(), CommonUtilities::STRING_ENCODING_UTF8);
			std::wstring desc = CommonUtilities::ToWide(card->GetDescription(), CommonUtilities::STRING_ENCODING_UTF8);

			myTitleTextComponent->SetText(title.c_str());
			myDescTextComponent->SetText(desc.c_str());
		}
	}

	void CardComponent::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		const cardgame::Card* card = GetCard();
		if (card == nullptr)
		{
			GetSystem().Destroy(GetEntityHandle());
			return;
		}

		bool canSee = card->IsInPlay()
			|| card->IsInDiscard()
			|| card->IsInHand() && card->GetOwner() == myLocalPlayer;

		myTitleTextComponent->SetEnabled(canSee);
		myDescTextComponent->SetEnabled(canSee);

		mySpriteFrontComponent->SetEnabled(canSee);
		mySpriteBackComponent->SetEnabled(not canSee);

		if (not IsDisplayOnly())
		{
			const bool isHovered = IsHovered();

			CommonUtilities::Vector4f frontTint = { 1.f, 1.f, 1.f, 1.f };
			CommonUtilities::Vector4f backTint = { 1.f, 1.f, 1.f, 1.f };

			if (isHovered)
			{
				frontTint = { 0.75f, 0.75f, 0.75f, 1.f };
				backTint = { 0.75f, 0.75f, 0.75f, 1.f };
			}

			if (card->CanBePlayed() && isHovered && myInputManager->GetKeyDown(CommonUtilities::Keys::MOUSELBUTTON))
			{
				myScene->PlayCard(myCard);
			}

			if (isHovered && myInputManager->GetKeyDown(CommonUtilities::Keys::MOUSERBUTTON))
			{
				myScene->InspectCard(myCard);
			}

			const bool selected = myScene->IsCardSelected(myCard);

			if (selected)
			{
				if (isHovered)
				{
					const CommonUtilities::InputManager& input = myScene->GetEngine().GetInputManager();
					if (input.GetKeyDown(CommonUtilities::Keys::MOUSELBUTTON))
					{
						myScene->UnselectCard(myCard);
					}

					frontTint = { 0.5f, 0.75f, 0.5f, 1.f };
					backTint = { 0.5f, 0.75f, 0.5f, 1.f };
				}
				else
				{
					frontTint = { 0.75f, 1.f, 0.75f, 1.f };
					backTint = { 0.75f, 1.f, 0.75f, 1.f };
				}
			}
			else
			{
				const bool validForSelection = myScene->CanCardBeSelected(myCard);
				if (validForSelection)
				{
					if (isHovered)
					{
						frontTint = { 1.f, 1.f, 0.5f, 1.f };
						backTint = { 1.f, 1.f, 0.5f, 1.f };
						const CommonUtilities::InputManager& input = myScene->GetEngine().GetInputManager();
						if (input.GetKeyDown(CommonUtilities::Keys::MOUSELBUTTON))
						{
							myScene->SelectCard(myCard);
						}
					}
					else
					{
						frontTint = { 1.f, 1.f, 0.75f, 1.f };
						backTint = { 1.f, 1.f, 0.75f, 1.f };
					}
				}
			}

			mySpriteFrontComponent->SetTint(frontTint);
			mySpriteBackComponent->SetTint(backTint);
		}
	}
}
