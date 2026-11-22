#include "Location.h"

namespace cardgame
{
	Location::Location(LocationID aID) noexcept :
		myID(aID)
	{
	}
	Location::~Location() noexcept
	{
	}

	LocationID Location::GetID() const noexcept
	{
		return myID;
	}

	bool Location::Contains(CardID aID) const noexcept
	{
		return myCardIndices.contains(aID);
	}

	void Location::AddCard(CardID aID) noexcept
	{
		const size_t index = myCards.size();
		myCards.push_back(aID);
		myCardIndices[aID] = index;
	}

	bool Location::RemoveCard(CardID aID) noexcept
	{
		auto it = myCardIndices.find(aID);
		if (it == myCardIndices.end())
		{
			return false;
		}
		const size_t index = it->second;

		// If card is not the last card in the list. Switch it with the last card.
		const size_t switchIndex = myCards.size() - 1;
		if (index != switchIndex)
		{
			const CardID switchCard = myCards.back();

			myCards[index] = switchCard;
			myCardIndices[switchCard] = index;
		}

		myCards.pop_back();
		myCardIndices.erase(it);
		return true;
	}

	size_t Location::GetNumCards() const noexcept
	{
		return myCards.size();
	}

	size_t Location::GetCards(CardID* outCards, size_t aStart, size_t aCount) const noexcept
	{
		const size_t total = myCards.size();
		if (aStart > total)
		{
			return 0;
		}

		const size_t count = std::min(aCount, total - aStart);

		memcpy(outCards, myCards.data() + aStart, sizeof(CardID) * count);

		return count;
	}

	Location::iterator Location::begin() noexcept
	{
		return Location::iterator(myCards.data());
	}
	Location::const_iterator Location::begin() const noexcept
	{
		return Location::const_iterator(myCards.data());
	}
	Location::const_iterator Location::cbegin() const noexcept
	{
		return Location::const_iterator(myCards.data());
	}

	Location::iterator Location::end() noexcept
	{
		return Location::iterator(myCards.data() + myCards.size());
	}
	Location::const_iterator Location::end() const noexcept
	{
		return Location::const_iterator(myCards.data() + myCards.size());
	}
	Location::const_iterator Location::cend() const noexcept
	{
		return Location::const_iterator(myCards.data() + myCards.size());
	}
}
