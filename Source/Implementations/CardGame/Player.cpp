#include "Player.h"

namespace cardgame
{
	Player::Player(PlayerID aID, LocationID aHand, LocationID aDeck, LocationID aPlay, LocationID aDiscard, TurnIndex aTurnIndex) :
		myID(aID),
		myHand(aHand),
		myDeck(aDeck),
		myPlay(aPlay),
		myDiscard(aDiscard),
		myTurnIndex(aTurnIndex),
		myHealth(10),
		myResources(0)
	{
	}
	Player::~Player()
	{
	}

	void Player::SetName(std::string_view aName) noexcept
	{
		myName = aName;
	}
	const std::string& Player::GetName() const noexcept
	{
		return myName;
	}

	PlayerID Player::GetID() const noexcept
	{
		return myID;
	}

	LocationID Player::GetHandLocation() const noexcept
	{
		return myHand;
	}
	LocationID Player::GetDeckLocation() const noexcept
	{
		return myDeck;
	}
	LocationID Player::GetPlayLocation() const noexcept
	{
		return myPlay;
	}
	LocationID Player::GetDiscardLocation() const noexcept
	{
		return myDiscard;
	}

	TurnIndex Player::GetTurnIndex() const noexcept
	{
		return myTurnIndex;
	}

	void Player::SetResourceCount(ResourceCount aAmount) noexcept
	{
		myResources = aAmount;
	}
	ResourceCount Player::GetResourceCount() const noexcept
	{
		return myResources;
	}

	void Player::SetHealth(Health aAmount) noexcept
	{
		myHealth = aAmount;
	}
	Health Player::GetHealth() const noexcept
	{
		return myHealth;
	}

	bool Player::IsDead() const noexcept
	{
		return myHealth <= 0;
	}
}