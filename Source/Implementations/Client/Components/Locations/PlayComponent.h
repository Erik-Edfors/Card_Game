#pragma once
#include "LocationComponent.h"

namespace client
{
	class PlayComponent : public LocationComponent
	{
	public:
		PlayComponent(cardgame::LocationID aID);
		~PlayComponent();

		void SetWidth(unsigned int aWidth) noexcept;
		unsigned int GetWidth() const noexcept;

	protected:
		void OnAdd(CardComponent* aCard) override;
		void OnRemove(CardComponent* aCard) override;

	private:
		void UpdateCardPositions();

		unsigned int myWidth;
	};
}