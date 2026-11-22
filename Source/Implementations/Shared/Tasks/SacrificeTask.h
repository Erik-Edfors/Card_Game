#pragma once
#include <Implementations/CardGame/Task.h>

#include <unordered_set>

namespace cardgame
{
	class Game;
}

namespace shared
{
	class SacrificeTask : public cardgame::TargetCardsTask
	{
	public:
		SacrificeTask();
		~SacrificeTask();

		void Initialize(
			cardgame::Game* aGame,
			cardgame::PlayerID aAuthority,
			cardgame::PlayerID aOwner,
			unsigned int aMinTargets,
			unsigned int aMaxTargets
		);

		void Reset();

		void SetAuthority(cardgame::PlayerID aPlayer) noexcept;
		void SetOwner(cardgame::PlayerID aPlayer);

		void SetMinTargets(unsigned int aAmount) noexcept;
		void SetMaxTargets(unsigned int aAmount) noexcept;

		void AddInvalidTarget(cardgame::CardID aID) noexcept;
		void RemoveInvalidTarget(cardgame::CardID aID) noexcept;
		void ClearInvalidTargets() noexcept;

		const std::vector<cardgame::CardID>& GetTargets() const noexcept;

		virtual cardgame::TASK_TYPE GetType() const override;
		virtual const char* GetDescriptor() const override;
		virtual bool IsFinished() const override;
		virtual cardgame::CardID GetAuthority() const override;

		virtual unsigned int GetMinTargets() const override;
		virtual unsigned int GetMaxTargets() const override;
		virtual bool IsValidTarget(cardgame::CardID aID) const override;
		virtual bool Submit(cardgame::CardID* aTargets, size_t aNumTargets) override;

	private:
		void UpdateDescriptor() noexcept;

		cardgame::Game* myGame;
		std::string myDescriptor;

		cardgame::PlayerID myOwner;
		cardgame::PlayerID myAuthority;

		std::unordered_set<cardgame::CardID> myInvalidTargets;

		unsigned int myMin;
		unsigned int myMax;

		std::vector<cardgame::CardID> mySubmittedTargets;
		bool myFinished;
	};
}