#pragma once
#include "Defines.h"
#include "Card.h"

#include <memory>

namespace cardgame
{
	class CardFactory
	{
	public:
		CardFactory();
		~CardFactory();

		std::unique_ptr<Card> CreateCard(CardFactoryID aID);

	private:
	};
}