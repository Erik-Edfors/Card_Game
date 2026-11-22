#include "CardFactory.h"

namespace cardgame
{
	CardFactory::CardFactory()
	{
	}
	CardFactory::~CardFactory()
	{
	}

	std::unique_ptr<Card> CardFactory::CreateCard(CardFactoryID aFactoryID)
	{
		auto it = myConstructors.find(aFactoryID);
		if (it == myConstructors.end())
		{
			//TODO: Log
			return nullptr;
		}
		CardConstructor constructor = it->second;
		return constructor();
	}
}
