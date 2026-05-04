#pragma once
#include "CommandControlledScene.h"

namespace shared
{
	class TwoPlayerMatchScene : public CommandControlledScene
	{
	public:
		virtual ~TwoPlayerMatchScene();

	protected:
		TwoPlayerMatchScene();
		
		virtual void OnInit() override;

	private:

	};
}