#include "Shared.pch.h"
#include "DestroyCard.h"

namespace shared
{
	DestroyCard::DestroyCard()
	{
	}
	DestroyCard::~DestroyCard()
	{
	}
	cardgame::ResourceCount DestroyCard::GetPlayCost() const
	{
		return 3;
	}
	bool DestroyCard::CanBePlayed() const
	{
		if (not IsInHand())
		{
			return false;
		}

		size_t numPlayers = GetGame().GetNumPlayers();
		const cardgame::Player** players = new const cardgame::Player * [numPlayers];
		numPlayers = GetGame().GetPlayers(players, numPlayers);

		bool anyCardInPlay = false;
		for (size_t p = 0; p < numPlayers; p++)
		{
			const cardgame::Player* player = players[p];
			if (player == nullptr)
			{
				continue;
			}

			const cardgame::Location* location = GetGame().GetLocation(player->GetPlayLocation());
			if (location == nullptr)
			{
				continue;
			}

			constexpr size_t cardBufferSize = 16;
			cardgame::CardID cards[cardBufferSize] = {};
			size_t cardCursor = 0;

			while (size_t numCards = location->GetCards(cards, cardCursor, cardBufferSize))
			{
				for (size_t c = 0; c < numCards; c++)
				{
					cardgame::CardID cardID = cards[c];

					const cardgame::Card* card = GetGame().GetCard(cardID);
					if (card != nullptr && card->IsInPlay())
					{
						anyCardInPlay = true;
						break;
					}
				}

				cardCursor += numCards;

				if (anyCardInPlay)
				{
					break;
				}
			}
		}

		delete[] players;

		return anyCardInPlay;
	}
	const char* DestroyCard::GetName() const
	{
		return "Destroy";
	}
	const char* DestroyCard::GetDescription() const
	{
		return "Cost: 3 RP.\n\nDestroy Target Card.";
	}
	void DestroyCard::OnInit()
	{
		BeginListen(cardgame::Game::EVENT_CARD_PLAYED);
		BeginListen(cardgame::Game::EVENT_TASK_FINISHED);

		myTargetsTask.Initialize(&GetGame(), GetOwner(), 1, 1);
		myTargetsTask.AddInvalidTarget(GetID());
	}
	bool DestroyCard::OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData)
	{
		switch (aEvent)
		{
		case cardgame::Game::EVENT_CARD_PLAYED:
		{
			cardgame::Game::EventCardPlayed data = std::any_cast<cardgame::Game::EventCardPlayed>(aData);
			if (data.Card == GetID())
			{
				myTargetsTask.Reset();
				return PushTask(GetGame(), &myTargetsTask);
			}
		}
		break;
		case cardgame::Game::EVENT_TASK_FINISHED:
		{
			if (myTargetsTask.IsFinished())
			{
				const cardgame::CardID targetID = myTargetsTask.GetTargets().front();
				myTargetsTask.Reset();

				const cardgame::Card* target = GetGame().GetCard(targetID);

				const cardgame::Player* player = GetGame().GetPlayer(GetOwner());
				const cardgame::Player* targetOwner = GetGame().GetPlayer(target->GetOwner());

				const cardgame::MoveCardCommand moveTarget = {
					.Card = targetID,
					.Location = targetOwner->GetDiscardLocation()
				};
				const cardgame::MoveCardCommand moveCard = {
					.Card = GetID(),
					.Location = player->GetDiscardLocation()
				};

				Do(GetGame(), moveTarget);
				Do(GetGame(), moveCard);

				return true;
			}
		}
		break;
		}

		return false;
	}
	void DestroyCard::OnDestroyed()
	{
		RetractTask(GetGame(), &myTargetsTask);
	}
}
