#pragma once
#include "Defines.h"
#include "GameObject.h"

#include <vector>
#include <unordered_map>

#include <Core/CommonUtilities/ContiguousIterator.hpp>

namespace cardgame
{
	class Location : public GameObject
	{
	public:
		using iterator = CommonUtilities::ContiguousIterator<CardID>;
		using const_iterator = CommonUtilities::ContiguousIterator<const CardID>;

		Location(LocationID aID) noexcept;
		~Location() noexcept;

		LocationID GetID() const noexcept;

		bool Contains(CardID aID) const noexcept;

		void AddCard(CardID aID) noexcept;
		bool RemoveCard(CardID aID) noexcept;

		size_t GetNumCards() const noexcept;
		size_t GetCards(CardID* outCards, size_t aStart, size_t aCount) const noexcept;

		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;

		iterator end() noexcept;
		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;

	private:
		LocationID myID;

		std::vector<CardID> myCards;
		std::unordered_map<CardID, size_t> myCardIndices;
	};
}