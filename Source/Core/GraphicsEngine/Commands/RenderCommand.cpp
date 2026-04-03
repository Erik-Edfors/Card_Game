#include "GraphicsEngine.pch.h"
#include "RenderCommand.h"
#include "../GraphicsEngine.h"

#include "../ConstantBuffers/MaterialBuffer.h"

#include <Core/RenderHardwareInterface/Device.h>

RenderCommand::RenderCommand(GraphicsEngine* aEngine, Mesh aMesh, Material aMaterial) :
	myEngine(aEngine),
	myMesh(aMesh),
	myMaterial(aMaterial)
{
}

void RenderCommand::Execute()
{
	graphics::Device* device = myEngine->GetDevice();
	device->BindVertexBuffers(&myMesh.VertexBuffer, 1, 0, &myMesh.VertexStride, nullptr);
	device->BindIndexBuffer(myMesh.IndexBuffer.Get());

	MaterialBuffer mat = {};
	for (size_t i = 0; i < 4; i++)
	{
		mat.AlbedoTint[i] = myMaterial.AlbedoTint[i];
	}
	mat.Roughness = myMaterial.Roughness;
	mat.Metalness = myMaterial.Metalness;

	myEngine->UpdateConstantBuffer(CONSTANT_BUFFER_MATERIAL, &mat, sizeof(mat));

	for (const MaterialResource& resource : myMaterial.Resources)
	{
		device->BindShaderResources(&resource.Resource, 1u, resource.Slot, resource.Stages);
	}

	myEngine->GetDevice()->DrawIndexed(static_cast<unsigned int>(myMesh.myNumIndices), 0, 0);
}
