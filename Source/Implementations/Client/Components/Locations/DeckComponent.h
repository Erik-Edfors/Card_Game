#pragma once
#include "LocationComponent.h"

namespace client
{
	class DeckComponent : public LocationComponent
	{
	public:
		DeckComponent(cardgame::LocationID aID);
		~DeckComponent();

	protected:
		void OnAdd(CardComponent* aCard) override;
		void OnRemove(CardComponent* aCard) override;

	private:
		void UpdateCardPositions();
	};
}