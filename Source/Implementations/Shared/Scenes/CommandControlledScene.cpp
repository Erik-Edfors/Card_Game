#include "Shared.pch.h"
#include "CommandControlledScene.h"

namespace shared
{
	CommandControlledScene::CommandControlledScene()
	{
	}
	CommandControlledScene::~CommandControlledScene()
	{
	}
	CommandList& CommandControlledScene::GetCommandList() noexcept
	{
		return myCommandList;
	}
	const CommandList& CommandControlledScene::GetCommandList() const noexcept
	{
		return myCommandList;
	}
}
