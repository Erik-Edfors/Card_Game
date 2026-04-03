#pragma once
#include "../Objects/Mesh.h"
#include "../Objects/Material.h"

class GraphicsEngine;

class RenderCommand
{
public:
	RenderCommand(GraphicsEngine* aEngine, Mesh aMesh, Material aMaterial);

	void Execute();
private:
	GraphicsEngine* myEngine;
	Mesh myMesh;
	Material myMaterial;
};