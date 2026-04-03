#include "GraphicsEngine.pch.h"
#include "UpdateConstantBufferCommand.h"
#include "../GraphicsEngine.h"

UpdateConstantBufferCommand::UpdateConstantBufferCommand(GraphicsEngine* aGraphicsEngine, CONSTANT_BUFFER aConstantBuffer, void* aData, size_t aSize) :
	myGraphicsEngine(aGraphicsEngine),
	myConstantBuffer(aConstantBuffer),
	myData(malloc(aSize)),
	mySize(aSize)
{
	memcpy(myData, aData, aSize);
}

UpdateConstantBufferCommand::~UpdateConstantBufferCommand()
{
	if (myData)
	{
		free(myData);
	}
}

UpdateConstantBufferCommand::UpdateConstantBufferCommand(UpdateConstantBufferCommand&& aUpdateConstantBufferCommand) :
	myGraphicsEngine(std::move(aUpdateConstantBufferCommand.myGraphicsEngine)),
	myConstantBuffer(std::move(aUpdateConstantBufferCommand.myConstantBuffer)),
	myData(std::move(aUpdateConstantBufferCommand.myData)),
	mySize(std::move(aUpdateConstantBufferCommand.mySize))
{
	aUpdateConstantBufferCommand.myData = nullptr;
	aUpdateConstantBufferCommand.mySize = 0;
}

UpdateConstantBufferCommand::UpdateConstantBufferCommand(const UpdateConstantBufferCommand& aUpdateConstantBufferCommand) :
	myGraphicsEngine(aUpdateConstantBufferCommand.myGraphicsEngine),
	myConstantBuffer(aUpdateConstantBufferCommand.myConstantBuffer),
	myData(malloc(aUpdateConstantBufferCommand.mySize)),
	mySize(aUpdateConstantBufferCommand.mySize)
{
	memcpy(myData, aUpdateConstantBufferCommand.myData, aUpdateConstantBufferCommand.mySize);
}

void UpdateConstantBufferCommand::Execute()
{
	myGraphicsEngine->UpdateConstantBuffer(myConstantBuffer, myData, mySize);
}
