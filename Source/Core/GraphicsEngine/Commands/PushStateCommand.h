#pragma once
#include "../Objects/PipelineState.h"

class GraphicsEngine;

class PushStateCommand
{
public:
	PushStateCommand(GraphicsEngine* aGraphicsEngine, const PipelineStateDesc& aPipelineState);
	~PushStateCommand();

	void Execute();

private:
	GraphicsEngine* myGraphicsEngine;
	PipelineStateDesc myPipelineState;
};