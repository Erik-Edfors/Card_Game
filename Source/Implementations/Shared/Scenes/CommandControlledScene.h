#pragma once
#include <Core/Engine/Scene.h>
#include "../Commands/CommandList.h"

namespace shared
{
	class CommandControlledScene : public Scene
	{
	public:
		virtual ~CommandControlledScene();

		CommandList& GetCommandList() noexcept;
		const CommandList& GetCommandList() const noexcept;

	protected:
		CommandControlledScene();

	private:
		CommandList myCommandList;
	};
}