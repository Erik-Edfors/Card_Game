#pragma once
#include <Core/CommonUtilities/Events.hpp>

namespace cardgame
{
	class Card;

	class Effect
	{
		friend class Card;
	public:
		Effect();
		~Effect();

		Card& GetCard() const noexcept;

	protected:
		virtual void OnInit() {};
		virtual void OnEvent(const CommonUtilities::Event& aEvent) { aEvent; }

	private:
		void Init(Card* aCard);

		Card* myCard;
	};
}