#pragma once
#include "CommandControlledScene.h"
#include "../PlayerManager.h"

namespace shared
{
	class MatchScene : public CommandControlledScene
	{
	public:

	protected:
		void AddPlayers(size_t aCount);

	private:
		std::vector<PlayerManager> myPlayers
	};
}