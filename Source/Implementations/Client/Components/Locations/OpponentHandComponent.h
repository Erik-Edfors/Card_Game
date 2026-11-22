#pragma once
#include "LocationComponent.h"

namespace client
{
	class OpponentHandComponent : public LocationComponent
	{
	public:
		OpponentHandComponent(cardgame::LocationID aID);
		~OpponentHandComponent();

	protected:
		void OnAdd(CardComponent* aCard) override;
		void OnRemove(CardComponent* aCard) override;

	private:
		void UpdateCardPositions();
	};
}