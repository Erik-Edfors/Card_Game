#pragma once
#include "Defines.h"
#include "GameObject.h"
#include <Core/CommonUtilities/Events.hpp>
#include <unordered_set>

namespace cardgame
{
	class Game;

	class Card : public GameObject
	{
		friend class Game;
	public:
		virtual ResourceCount GetPlayCost() const = 0;
		virtual bool CanBePlayed() const = 0;
		virtual const char* GetName() const = 0;
		virtual const char* GetDescription() const = 0;

		Card();
		virtual ~Card();

		void Init(Game* aGame, CardID aID, PlayerID aOwner, LocationID aLocation);
		bool Update();
		void ResetLocks() noexcept;

		CardID GetID() const noexcept;

		void SetOwner(PlayerID aOwner) noexcept;
		PlayerID GetOwner() const noexcept;

		void SetLocation(LocationID aLocation);
		LocationID GetLocation() const noexcept;

		Game& GetGame() const noexcept;

		bool IsInHand() const noexcept;
		bool IsInPlay() const noexcept;
		bool IsInDeck() const noexcept;
		bool IsInDiscard() const noexcept;

	protected:
		virtual void OnInit() {};
		virtual bool OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData) = 0;
		virtual void OnDestroyed() {};

		bool RepeatLock(CommonUtilities::StringHash aLock) noexcept;

		void BeginListen(CommonUtilities::StringHash aEvent) noexcept;
		void StopListen(CommonUtilities::StringHash aEvent) noexcept;
		void StopListen() noexcept;

	private:
		std::unordered_set<CommonUtilities::StringHash> myRepetitionLocks;

		Game* myGame;
		CardID myID;
		LocationID myLocation;
		PlayerID myOwner;
		CommonUtilities::EventListener myEventListener;
	};
}