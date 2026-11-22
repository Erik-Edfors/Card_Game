#include "Shared.pch.h"
#include "SacrificeTask.h"

#include <Implementations/CardGame/Game.h>

namespace shared
{
	SacrificeTask::SacrificeTask() :
		myGame(nullptr),
		myOwner(cardgame::INVALID_PLAYER_ID),
		myAuthority(cardgame::INVALID_PLAYER_ID),
		myMin(1),
		myMax(1),
		myFinished(false)
	{
		UpdateDescriptor();
	}
	SacrificeTask::~SacrificeTask()
	{
	}
	void SacrificeTask::Initialize(cardgame::Game* aGame, cardgame::PlayerID aAuthority, cardgame::PlayerID aOwner, unsigned int aMinTargets, unsigned int aMaxTargets)
	{
		Reset();

		myGame = aGame;
		SetAuthority(aAuthority);
		SetOwner(aOwner);
		SetMinTargets(aMinTargets);
		SetMaxTargets(aMaxTargets);
	}
	void SacrificeTask::Reset()
	{
		myFinished = false;
		mySubmittedTargets.clear();
	}
	void SacrificeTask::SetAuthority(cardgame::PlayerID aPlayer) noexcept
	{
		myAuthority = aPlayer;
	}
	void SacrificeTask::SetOwner(cardgame::PlayerID aPlayer)
	{
		myOwner = aPlayer;
	}
	void SacrificeTask::SetMinTargets(unsigned int aAmount) noexcept
	{
		myMin = aAmount;
		UpdateDescriptor();
	}
	void SacrificeTask::SetMaxTargets(unsigned int aAmount) noexcept
	{
		myMax = aAmount;
		UpdateDescriptor();
	}
	void SacrificeTask::AddInvalidTarget(cardgame::CardID aID) noexcept
	{
		myInvalidTargets.insert(aID);
	}
	void SacrificeTask::RemoveInvalidTarget(cardgame::CardID aID) noexcept
	{
		myInvalidTargets.erase(aID);
	}
	void SacrificeTask::ClearInvalidTargets() noexcept
	{
		myInvalidTargets.clear();
	}
	const std::vector<cardgame::CardID>& SacrificeTask::GetTargets() const noexcept
	{
		return mySubmittedTargets;
	}
	cardgame::TASK_TYPE SacrificeTask::GetType() const
	{
		return cardgame::TASK_TARGET_CARDS;
	}
	const char* SacrificeTask::GetDescriptor() const
	{
		return myDescriptor.c_str();
	}
	bool SacrificeTask::IsFinished() const
	{
		return myFinished;
	}
	cardgame::CardID SacrificeTask::GetAuthority() const
	{
		return myAuthority;
	}

	unsigned int SacrificeTask::GetMinTargets() const
	{
		return myMin;
	}
	unsigned int SacrificeTask::GetMaxTargets() const
	{
		return myMax;
	}
	bool SacrificeTask::IsValidTarget(cardgame::CardID aID) const
	{
		if (myGame == nullptr || myOwner == cardgame::INVALID_PLAYER_ID || myInvalidTargets.contains(aID))
		{
			return false;
		}
		const cardgame::Card* card = myGame->GetCard(aID);
		if (card == nullptr)
		{
			return false;
		}
		return card->GetOwner() == myOwner && card->IsInPlay();
	}
	bool SacrificeTask::Submit(cardgame::CardID* aTargets, size_t aNumTargets)
	{
		if (aNumTargets < GetMinTargets() || aNumTargets > GetMaxTargets())
		{
			return false;
		}

		mySubmittedTargets.clear();
		for (size_t i = 0; i < aNumTargets; i++)
		{
			const cardgame::CardID card = aTargets[i];
			if (not IsValidTarget(card))
			{
				mySubmittedTargets.clear();
				return false;
			}
			mySubmittedTargets.push_back(card);
		}

		myFinished = true;

		return true;
	}

	void SacrificeTask::UpdateDescriptor() noexcept
	{
		if (GetMinTargets() == 1 && GetMaxTargets() == 1)
		{
			myDescriptor = "Choose a Card to Sacrifice";
		}
		else if (GetMinTargets() == GetMaxTargets())
		{
			myDescriptor = std::format("Choose {} Cards to Sacrifice", GetMinTargets());
		}
		else if (GetMinTargets() == 0)
		{
			myDescriptor = std::format("Choose up to {} Card(s) to Sacrifice", GetMaxTargets());
		}
		else
		{
			myDescriptor = std::format("Choose between {} to {} Cards to Sacrifice", GetMinTargets(), GetMaxTargets());
		}
	}
}
