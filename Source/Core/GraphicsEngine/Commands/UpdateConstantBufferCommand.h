#pragma once
#include <Core/GraphicsEngine/GraphicsEngineDefs.h>

class GraphicsEngine;

class UpdateConstantBufferCommand
{
public:
	UpdateConstantBufferCommand(GraphicsEngine* aGraphicsEngine, CONSTANT_BUFFER aConstantBuffer, void* aData, size_t aSize);
	~UpdateConstantBufferCommand();
	UpdateConstantBufferCommand(UpdateConstantBufferCommand&& aUpdateConstantBufferCommand);
	UpdateConstantBufferCommand(const UpdateConstantBufferCommand& aUpdateConstantBufferCommand);

	void Execute();

private:
	GraphicsEngine* myGraphicsEngine;
	CONSTANT_BUFFER myConstantBuffer;
	void* myData;
	size_t mySize;
};