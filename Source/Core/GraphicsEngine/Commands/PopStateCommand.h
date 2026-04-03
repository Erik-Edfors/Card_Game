#pragma once

class GraphicsEngine;

class PopStateCommand
{
public:
	PopStateCommand(GraphicsEngine* aGraphicsEngine);
	~PopStateCommand();

	void Execute();

private:
	GraphicsEngine* myGraphicsEngine;
};