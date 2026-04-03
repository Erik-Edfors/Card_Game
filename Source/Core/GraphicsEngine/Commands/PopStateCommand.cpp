#include "GraphicsEngine.pch.h"
#include "PopStateCommand.h"
#include "../GraphicsEngine.h"

PopStateCommand::PopStateCommand(GraphicsEngine* aGraphicsEngine) :
	myGraphicsEngine(aGraphicsEngine)
{
}
PopStateCommand::~PopStateCommand() = default;

void PopStateCommand::Execute()
{
	myGraphicsEngine->PopState();
}
