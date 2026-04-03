#pragma once

class GraphicsEngine;

class DrawCommand
{
public:
	DrawCommand(GraphicsEngine* aGraphicsEngine, unsigned int aCount, unsigned int aWhere);
	~DrawCommand();

	void Execute();
private:
	GraphicsEngine* myGraphicsEngine;
	unsigned int myCount;
	unsigned int myWhere;
};