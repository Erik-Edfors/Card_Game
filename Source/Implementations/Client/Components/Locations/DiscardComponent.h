#pragma once
#include "LocationComponent.h"

namespace client
{
	class DiscardComponent : public LocationComponent
	{
	public:
		DiscardComponent(cardgame::LocationID aID);
		~DiscardComponent();

	protected:
		void OnAdd(CardComponent* aCard) override;
		void OnRemove(CardComponent* aCard) override;

	private:
		void UpdateCardPositions();
	};
}