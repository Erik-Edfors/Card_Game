#pragma once

class GraphicsEngine;

class DrawIndexedCommand
{
public:
	DrawIndexedCommand(GraphicsEngine* aGraphicsEngine, unsigned int aCount, unsigned int aWhere, unsigned int aVertexWhere);
	~DrawIndexedCommand();

	void Execute();
private:
	GraphicsEngine* myGraphicsEngine;
	unsigned int myCount;
	unsigned int myWhere;
	unsigned int myVertexWhere;
};