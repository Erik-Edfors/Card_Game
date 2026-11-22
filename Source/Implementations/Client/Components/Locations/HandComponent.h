#pragma once
#include "LocationComponent.h"

namespace client
{
	class HandComponent : public LocationComponent
	{
	public:
		HandComponent(cardgame::LocationID aID);
		~HandComponent();

	protected:
		void OnAdd(CardComponent* aCard) override;
		void OnRemove(CardComponent* aCard) override;

	private:
		void UpdateCardPositions();
	};
}