#pragma once

namespace cardgame
{
	class Effect;

	class Ability
	{
	public:

		void SetEffect();

	private:
		Effect* myEffect;
	};
}