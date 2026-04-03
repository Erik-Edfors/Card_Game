#include "GraphicsEngine.pch.h"
#include "PushStateCommand.h"
#include "../GraphicsEngine.h"

PushStateCommand::PushStateCommand(GraphicsEngine* aGraphicsEngine, const PipelineStateDesc& aPipelineState) :
	myGraphicsEngine(aGraphicsEngine),
	myPipelineState(aPipelineState)
{
}
PushStateCommand::~PushStateCommand() = default;

void PushStateCommand::Execute()
{
	myGraphicsEngine->PushState(myPipelineState);
}
