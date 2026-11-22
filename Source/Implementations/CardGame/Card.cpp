#include "Card.h"
#include "Game.h"

namespace cardgame
{
	Card::Card() :
		myGame(nullptr),
		myID(0),
		myLocation(0),
		myOwner(0)
	{
	}

	Card::~Card()
	{
	}

	void Card::Init(Game* aGame, CardID aID, PlayerID aOwner, LocationID aLocation)
	{
		myGame = aGame;
		myID = aID;
		SetOwner(aOwner);
		SetLocation(aLocation);

		myEventListener.SetPusher(&myGame->GetEventPusher());

		OnInit();
	}
	bool Card::Update()
	{
		bool action = false;

		CommonUtilities::Event event;
		while (myEventListener.GetEvent(&event))
		{
			action |= OnEvent(event.ID, event.Data);
		}

		return action;
	}

	void Card::ResetLocks() noexcept
	{
		myRepetitionLocks.clear();
	}

	CardID Card::GetID() const noexcept
	{
		return myID;
	}

	void Card::SetOwner(PlayerID aOwner) noexcept
	{
		myOwner = aOwner;
	}

	PlayerID Card::GetOwner() const noexcept
	{
		return myOwner;
	}

	void Card::SetLocation(LocationID aLocation)
	{
		myLocation = aLocation;
	}

	LocationID Card::GetLocation() const noexcept
	{
		return myLocation;
	}

	Game& Card::GetGame() const noexcept
	{
		return *myGame;
	}

	bool Card::IsInHand() const noexcept
	{
		const Player* player = GetGame().GetPlayer(GetOwner());
		return player != nullptr && player->GetHandLocation() == GetLocation();
	}
	bool Card::IsInPlay() const noexcept
	{
		const Player* player = GetGame().GetPlayer(GetOwner());
		return player != nullptr && player->GetPlayLocation() == GetLocation();
	}
	bool Card::IsInDeck() const noexcept
	{
		const Player* player = GetGame().GetPlayer(GetOwner());
		return player != nullptr && player->GetDeckLocation() == GetLocation();
	}
	bool Card::IsInDiscard() const noexcept
	{
		const Player* player = GetGame().GetPlayer(GetOwner());
		return player != nullptr && player->GetDiscardLocation() == GetLocation();
	}

	bool Card::RepeatLock(CommonUtilities::StringHash aLock) noexcept
	{
		if (myRepetitionLocks.contains(aLock))
		{
			return false;
		}
		myRepetitionLocks.insert(aLock);
		return true;
	}

	void Card::BeginListen(CommonUtilities::StringHash aEvent) noexcept
	{
		myEventListener.BeginListen(aEvent);
	}
	void Card::StopListen(CommonUtilities::StringHash aEvent) noexcept
	{
		myEventListener.StopListen(aEvent);
	}
	void Card::StopListen() noexcept
	{
		myEventListener.StopListen();
	}
}