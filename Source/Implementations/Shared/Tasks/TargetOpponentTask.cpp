#include "Shared.pch.h"
#include "TargetOpponentTask.h"

namespace shared
{
	TargetOpponentTask::TargetOpponentTask() noexcept : 
		myOwner(cardgame::INVALID_PLAYER_ID),
		myTarget(cardgame::INVALID_PLAYER_ID)
	{
	}
	TargetOpponentTask::~TargetOpponentTask() noexcept
	{
	}

	void TargetOpponentTask::SetOwner(cardgame::PlayerID aOwner) noexcept
	{
		myOwner = aOwner;
	}

	cardgame::PlayerID TargetOpponentTask::GetTarget() const noexcept
	{
		return myTarget;
	}

	cardgame::TASK_TYPE TargetOpponentTask::GetType() const
	{
		return cardgame::TASK_TARGET_PLAYERS;
	}

	const char* TargetOpponentTask::GetDescriptor() const
	{
		return "Choose an Opponent";
	}

	bool TargetOpponentTask::IsFinished() const
	{
		return myTarget != cardgame::INVALID_PLAYER_ID;
	}

	unsigned int TargetOpponentTask::GetMinTargets() const
	{
		return 1;
	}
	unsigned int TargetOpponentTask::GetMaxTargets() const
	{
		return 1;
	}
	bool TargetOpponentTask::IsValidTarget(cardgame::PlayerID aID) const
	{
		return aID != cardgame::INVALID_PLAYER_ID && aID != myOwner;
	}

	bool TargetOpponentTask::Submit(cardgame::PlayerID* aTargets, size_t aNumTargets)
	{
		if (aNumTargets != 1)
		{
			return false;
		}

		cardgame::PlayerID target = *aTargets;
		if (not IsValidTarget(target))
		{
			return false;
		}
		
		myTarget = target;

		return true;
	}

}