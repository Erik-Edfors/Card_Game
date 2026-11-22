#include "Shared.pch.h"
#include "TargetPlayCardsTask.h"

namespace shared
{
	TargetPlayCardsTask::TargetPlayCardsTask() :
		myAuthority(cardgame::INVALID_PLAYER_ID),
		myTarget(cardgame::INVALID_CARD_ID),
		myGame(nullptr),
		myFinished(false),
		myMin(0),
		myMax(0)
	{
	}
	TargetPlayCardsTask::~TargetPlayCardsTask()
	{
	}
	void TargetPlayCardsTask::Initialize(cardgame::Game* aGame, cardgame::PlayerID aAuthority, unsigned int aMinTargets, unsigned int aMaxTargets)
	{
		Reset();

		myGame = aGame;

		SetMinTargets(aMinTargets);
		SetMaxTargets(aMaxTargets);
		SetAuthority(aAuthority);
	}
	void TargetPlayCardsTask::Reset()
	{
		myTargets.clear();
		myFinished = false;
	}
	void TargetPlayCardsTask::SetMinTargets(unsigned int aAmount) noexcept
	{
		myMin = aAmount;
		UpdateDescriptor();
	}
	void TargetPlayCardsTask::SetMaxTargets(unsigned int aAmount) noexcept
	{
		myMax = aAmount;
		UpdateDescriptor();
	}
	void TargetPlayCardsTask::AddInvalidTarget(cardgame::CardID aID) noexcept
	{
		myInvalidTargets.insert(aID);
	}
	void TargetPlayCardsTask::RemoveInvalidTarget(cardgame::CardID aID) noexcept
	{
		myInvalidTargets.erase(aID);
	}
	void TargetPlayCardsTask::ClearInvalidTargets() noexcept
	{
		myInvalidTargets.clear();
	}
	const std::vector<cardgame::CardID>& TargetPlayCardsTask::GetTargets() const noexcept
	{
		return myTargets;
	}
	void TargetPlayCardsTask::SetAuthority(cardgame::PlayerID aPlayer) noexcept
	{
		myAuthority = aPlayer;
	}
	cardgame::TASK_TYPE TargetPlayCardsTask::GetType() const
	{
		return cardgame::TASK_TARGET_CARDS;
	}
	const char* TargetPlayCardsTask::GetDescriptor() const
	{
		return myDescriptor.c_str();
	}
	bool TargetPlayCardsTask::IsFinished() const
	{
		return myFinished;
	}
	cardgame::PlayerID TargetPlayCardsTask::GetAuthority() const
	{
		return myAuthority;
	}
	unsigned int TargetPlayCardsTask::GetMinTargets() const
	{
		return myMin;
	}
	unsigned int TargetPlayCardsTask::GetMaxTargets() const
	{
		return myMax;
	}
	bool TargetPlayCardsTask::IsValidTarget(TargetID aID) const
	{
		if (myGame == nullptr || myInvalidTargets.contains(aID))
		{
			return false;
		}
		const cardgame::Card* card = myGame->GetCard(aID);
		if (card == nullptr)
		{
			return false;
		}
		return myGame->GetCard(aID)->IsInPlay();
	}
	bool TargetPlayCardsTask::Submit(TargetID* aTargets, size_t aNumTargets)
	{
		if (aNumTargets < GetMinTargets() || aNumTargets > GetMaxTargets())
		{
			return false;
		}

		myTargets.clear();
		for (size_t i = 0; i < aNumTargets; i++)
		{
			const cardgame::CardID card = aTargets[i];
			if (not IsValidTarget(card))
			{
				myTargets.clear();
				return false;
			}
			myTargets.push_back(card);
		}

		myFinished = true;

		return true;
	}
	void TargetPlayCardsTask::UpdateDescriptor()
	{
		if (GetMinTargets() == 1 && GetMaxTargets() == 1)
		{
			myDescriptor = "Choose a Target Card";
		}
		else if (GetMinTargets() == GetMaxTargets())
		{
			myDescriptor = std::format("Choose {} Target Cards", GetMinTargets());
		}
		else if (GetMinTargets() == 0)
		{
			myDescriptor = std::format("Choose up to {} Target Card(s)", GetMaxTargets());
		}
		else
		{
			myDescriptor = std::format("Choose between {} to {} Target Cards", GetMinTargets(), GetMaxTargets());
		}
	}
}