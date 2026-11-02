#pragma once
#include <Implementations/CardGame/Task.h>

namespace shared
{
	class TargetOpponentTask : public cardgame::TargetPlayersTask
	{
	public:
		TargetOpponentTask() noexcept;
		~TargetOpponentTask() noexcept;

		void SetOwner(cardgame::PlayerID aOwner) noexcept;

		cardgame::PlayerID GetTarget() const noexcept;

		virtual cardgame::TASK_TYPE GetType() const override;
		virtual const char* GetDescriptor() const override;
		virtual bool IsFinished() const override;

		virtual unsigned int GetMinTargets() const override;
		virtual unsigned int GetMaxTargets() const override;
		virtual bool IsValidTarget(cardgame::PlayerID aID) const override;
		virtual bool Submit(cardgame::PlayerID* aTargets, size_t aNumTargets) override;

	private:
		cardgame::PlayerID myOwner;
		cardgame::PlayerID myTarget;
	};
}