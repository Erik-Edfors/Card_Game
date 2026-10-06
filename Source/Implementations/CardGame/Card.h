#pragma once
#include <Core/CommonUtilities/Events.hpp>

namespace cardgame
{
	class Game;

	class Card
	{
		friend class Game;
	public:
		Card();
		~Card();

		void Init(Game* aGame);
		void Update();

		Game* GetGame() const noexcept;

	protected:
		virtual void OnEvent(const CommonUtilities::Event& aEvent) { aEvent; }

	private:
		Game* myGame;
		CommonUtilities::EventListener myEventListener;
	};
}