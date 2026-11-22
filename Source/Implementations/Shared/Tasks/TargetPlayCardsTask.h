#pragma once
#include <Implementations/CardGame/Task.h>
#include <Implementations/CardGame/Game.h>

namespace shared
{
	class TargetPlayCardsTask : public cardgame::TargetCardsTask
	{
	public:
		TargetPlayCardsTask();
		~TargetPlayCardsTask();

		void Initialize(
			cardgame::Game* aGame,
			cardgame::PlayerID aAuthority,
			unsigned int aMintargets,
			unsigned int aMaxTargets
		);
		void Reset();

		void SetMinTargets(unsigned int aAmount) noexcept;
		void SetMaxTargets(unsigned int aAmount) noexcept;

		void AddInvalidTarget(cardgame::CardID aID) noexcept;
		void RemoveInvalidTarget(cardgame::CardID aID) noexcept;
		void ClearInvalidTargets() noexcept;

		const std::vector<cardgame::CardID>& GetTargets() const noexcept;

		void SetAuthority(cardgame::PlayerID aPlayer) noexcept;

		virtual cardgame::TASK_TYPE GetType() const override;
		virtual const char* GetDescriptor() const override;
		virtual bool IsFinished() const override;
		virtual cardgame::PlayerID GetAuthority() const override;

		virtual unsigned int GetMinTargets() const override;
		virtual unsigned int GetMaxTargets() const override;
		virtual bool IsValidTarget(TargetID aID) const override;
		virtual bool Submit(TargetID* aTargets, size_t aNumTargets) override;

	private:
		void UpdateDescriptor();

		cardgame::PlayerID myAuthority;
		cardgame::CardID myTarget;
		cardgame::Game* myGame;

		std::unordered_set<cardgame::CardID> myInvalidTargets;

		std::string myDescriptor;

		std::vector<cardgame::CardID> myTargets;
		bool myFinished;

		unsigned int myMin;
		unsigned int myMax;
	};
}