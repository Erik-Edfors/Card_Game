#pragma once

namespace shared
{
	class PlayerManager
	{
		friend class MatchScene;
	public:
		using CardID = unsigned int;
		using ActionID = unsigned int;

		static constexpr CardID INVALID_CARD_ID = 0u;
		static constexpr ActionID INVALID_ACTION_ID = 0u;

		PlayerManager();
		~PlayerManager();

		ActionID Play(CardID aCard);

	private:

	};
}