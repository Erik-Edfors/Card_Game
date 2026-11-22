#pragma once
#include "Defines.h"
#include "GameObject.h"

#include <string>

namespace cardgame
{
	class Player : public GameObject
	{
	public:
		Player(PlayerID aID, LocationID aHand, LocationID aDeck, LocationID aPlay, LocationID aDiscard, TurnIndex aTurnIndex);
		~Player();

		void SetName(std::string_view aName) noexcept;
		const std::string& GetName() const noexcept;

		PlayerID GetID() const noexcept;
		LocationID GetHandLocation() const noexcept;
		LocationID GetDeckLocation() const noexcept;
		LocationID GetPlayLocation() const noexcept;
		LocationID GetDiscardLocation() const noexcept;
		TurnIndex GetTurnIndex() const noexcept;

		void SetTurnIndex(TurnIndex aTurnIndex) noexcept;

		void SetResourceCount(ResourceCount aAmount) noexcept;
		ResourceCount GetResourceCount() const noexcept;

		void SetHealth(Health aAmount) noexcept;
		Health GetHealth() const noexcept;

		bool IsDead() const noexcept;

	private:
		PlayerID myID;

		std::string myName;

		LocationID myHand;
		LocationID myDeck;
		LocationID myPlay;
		LocationID myDiscard;
		TurnIndex myTurnIndex;

		ResourceCount myResources;
		Health myHealth;
	};
}