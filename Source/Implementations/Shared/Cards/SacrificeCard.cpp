#include "Shared.pch.h"
#include "SacrificeCard.h"

#include <Implementations/CardGame/Game.h>

namespace shared
{
	SacrificeCard::SacrificeCard()
	{
	}
	SacrificeCard::~SacrificeCard()
	{
	}
	cardgame::ResourceCount SacrificeCard::GetPlayCost() const
	{
		return 0;
	}
	bool SacrificeCard::CanBePlayed() const
	{
		return IsInHand() && GetGame().GetLocation(GetGame().GetPlayer(GetOwner())->GetPlayLocation())->GetNumCards() != 0;
	}
	const char* SacrificeCard::GetName() const
	{
		return "Sacrifice";
	}
	const char* SacrificeCard::GetDescription() const
	{
		return "Sacrifice a Card in Play, then Draw 2 Cards and Add 2 RP.";
	}
	void SacrificeCard::OnInit()
	{
		BeginListen(cardgame::Game::EVENT_CARD_PLAYED);
		BeginListen(cardgame::Game::EVENT_TASK_FINISHED);

		mySacrificeTask.Initialize(
			&GetGame(),
			GetOwner(),
			GetOwner(),
			1,
			1
		);
		mySacrificeTask.AddInvalidTarget(GetID());
	}
	bool SacrificeCard::OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData)
	{
		switch (aEvent)
		{
		case cardgame::Game::EVENT_CARD_PLAYED:
		{
			cardgame::Game::EventCardPlayed data = std::any_cast<cardgame::Game::EventCardPlayed>(aData);
			if (GetID() == data.Card)
			{
				mySacrificeTask.Reset();
				if (PushTask(GetGame(), &mySacrificeTask))
				{
					return true;
				}
			}
		}
		break;
		case cardgame::Game::EVENT_TASK_FINISHED:
		{
			if (mySacrificeTask.IsFinished())
			{
				const cardgame::CardID sacrificeID = mySacrificeTask.GetTargets().front();
				mySacrificeTask.Reset();

				const cardgame::Card* sacrificeCard = GetGame().GetCard(sacrificeID);

				if (sacrificeCard == nullptr)
				{
					break;
				}

				const cardgame::Player* owner = GetGame().GetPlayer(GetOwner());
				const cardgame::LocationID deckID = owner->GetDeckLocation();
				const cardgame::LocationID handID = owner->GetHandLocation();
				const cardgame::Player* sacrificeOwner = GetGame().GetPlayer(sacrificeCard->GetOwner());

				if (owner == nullptr || sacrificeOwner == nullptr)
				{
					break;
				}

				const cardgame::MoveCardCommand moveSacrifice = {
					.Card = sacrificeID,
					.Location = sacrificeOwner->GetDiscardLocation()
				};

				const cardgame::MoveCardCommand moveToDiscard = {
					.Card = GetID(),
					.Location = owner->GetDiscardLocation()
				};

				const cardgame::AddPlayerResourcesCommand addResources = {
					.Source = GetID(),
					.Target = GetOwner(),
					.Amount = 2
				};

				if (Do(GetGame(), moveSacrifice))
				{
					for (size_t i = 0; i < 2; i++)
					{
						const cardgame::MoveCardCommand draw = {
							.Card = GetRandomCard(deckID),
							.Location = handID
						};

						Do(GetGame(), draw);
					}

					Do(GetGame(), addResources);
				}

				Do(GetGame(), moveToDiscard);

				return true;
			}
		}
		break;
		}

		return false;
	}
	void SacrificeCard::OnDestroyed()
	{
		RetractTask(GetGame(), &mySacrificeTask);
	}

	cardgame::CardID SacrificeCard::GetRandomCard(cardgame::LocationID aLocation)
	{
		const cardgame::Location* location = GetGame().GetLocation(aLocation);
		if (location == nullptr)
		{
			return cardgame::INVALID_CARD_ID;
		}

		const cardgame::GenerateUniformRandomIntegerCommand randInt = {
			.Min = 0,
			.Max = static_cast<int>(location->GetNumCards()) - 1
		};
		cardgame::GenerateUniformRandomIntegerCommand::Result randIntResult;

		if (Do(GetGame(), randInt, &randIntResult))
		{
			cardgame::CardID cardID;
			location->GetCards(&cardID, randIntResult.Number, 1);

			return cardID;
		}

		return cardgame::INVALID_CARD_ID;
	}
}
