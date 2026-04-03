#include "GraphicsEngine.pch.h"
#include "RenderEntry.h"
#include "../GraphicsEngine.h"

#include "../ConstantBuffers/ObjectBuffer.h"
#include "../ConstantBuffers/MaterialBuffer.h"
#include "../ConstantBuffers/AnimationBuffer.h"

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/CommonUtilities/Math/Interpolation.hpp>

#include <set>

#undef FILE_CATEGORY
#define FILE_CATEGORY "RenderEntry.cpp"
#undef CLASS_CATEGORY
#define CLASS_CATEGORY FILE_CATEGORY"->RenderEntry"

RenderEntry::RenderEntry() :
	myID(0u),
	myFlags(RENDER_ENTRY_FLAGS_NONE),
	myMesh({}),
	myMaterial({}),
	myJoints(nullptr)
{
}
RenderEntry::~RenderEntry() = default;

void RenderEntry::SetID(unsigned int aID) noexcept
{
	myID = aID;
}

unsigned int RenderEntry::GetID() const noexcept
{
	return myID;
}

void RenderEntry::SetFlags(RENDER_ENTRY_FLAGS aFlags) noexcept
{
	myFlags = aFlags;
}

RENDER_ENTRY_FLAGS RenderEntry::GetFlags() const noexcept
{
	return myFlags;
}

void RenderEntry::SetTransform(const CommonUtilities::Matrix4x4<float>& aTransform)
{
	myTransform = aTransform;
}

const CommonUtilities::Matrix4x4<float>& RenderEntry::GetTransform() const
{
	return myTransform;
}

void RenderEntry::SetMesh(const Mesh& aMesh) noexcept
{
	myMesh = aMesh;
}

void RenderEntry::SetMesh(Mesh&& aMesh) noexcept
{
	myMesh = std::move(aMesh);
}

const Mesh& RenderEntry::GetMesh() const noexcept
{
	return myMesh;
}

Mesh& RenderEntry::GetMesh() noexcept
{
	return myMesh;
}

void RenderEntry::SetMaterial(const Material& aMaterial) noexcept
{
	myMaterial = aMaterial;
}

void RenderEntry::SetMaterial(Material&& aMaterial) noexcept
{
	myMaterial = std::move(aMaterial);
}

const Material& RenderEntry::GetMaterial() const noexcept
{
	return myMaterial;
}

Material& RenderEntry::GetMaterial() noexcept
{
	return myMaterial;
}

void RenderEntry::SetJoints(const CommonUtilities::Matrix4x4f* aJoints)
{
	myJoints = aJoints;
}

const CommonUtilities::Matrix4x4f* RenderEntry::GetJoints() const
{
	return myJoints;
}

void RenderEntry::Draw(GraphicsEngine* aGraphicsEngine, bool aEnableBlend)
{
#define LOG_CATEGORY CLASS_CATEGORY"::Draw"

	ObjectBuffer obj = {};
	obj.ID = myID;
	errno_t error = memcpy_s(obj.Transform, sizeof(obj.Transform), &myTransform, sizeof(myTransform));
	if (error != 0)
	{
		LOG_ERROR(LOG_CATEGORY, "Failed to copy transform to ObjectBuffer! Error: '%i'", error);
		return;
	}
	aGraphicsEngine->UpdateConstantBuffer(CONSTANT_BUFFER_OBJECT, &obj, sizeof(obj));

	MaterialBuffer mat = {};

	for (const auto& resource : myMaterial.Resources)
	{
		aGraphicsEngine->GetDevice()->BindShaderResources(&resource.Resource, 1, resource.Slot, resource.Stages);
	}

	std::copy(myMaterial.AlbedoTint, myMaterial.AlbedoTint + 4, mat.AlbedoTint);
	std::copy(myMaterial.EmissiveColor, myMaterial.EmissiveColor + 3, mat.EmissiveColor);
	mat.EmissionIntensity = myMaterial.EmissionIntensity;
	mat.Roughness = myMaterial.Roughness;
	mat.Metalness = myMaterial.Metalness;
	mat.Flags = myMaterial.Flags;
	aGraphicsEngine->UpdateConstantBuffer(CONSTANT_BUFFER_MATERIAL, &mat, sizeof(mat));

	AnimationBuffer animationBuffer = {};
	if (myJoints != nullptr)
	{
		error = memcpy_s(animationBuffer.AB_Joints, sizeof(animationBuffer.AB_Joints), myJoints, ANIMATION_MAX_SKELETON_JOINTS * sizeof(CommonUtilities::Matrix4x4f));
		if (error != 0)
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to copy joints to animation buffer! Error: '%i'", error);
			return;
		}
	}
	else
	{
		for (size_t i = 0; i < ANIMATION_MAX_SKELETON_JOINTS; i++)
		{
			animationBuffer.AB_Joints[i][0][0] = 1.f;
			animationBuffer.AB_Joints[i][1][1] = 1.f;
			animationBuffer.AB_Joints[i][2][2] = 1.f;
			animationBuffer.AB_Joints[i][3][3] = 1.f;
		}
	}

	aGraphicsEngine->UpdateConstantBuffer(CONSTANT_BUFFER_ANIMATION, &animationBuffer, sizeof(animationBuffer));

	aGraphicsEngine->GetDevice()->BindVertexBuffers(&myMesh.VertexBuffer, 1, 0, &myMesh.VertexStride, nullptr);
	aGraphicsEngine->GetDevice()->BindIndexBuffer(myMesh.IndexBuffer.Get());

	//TODO: Add blendfactors and SampleMask.
	if (aEnableBlend)
	{
		aGraphicsEngine->GetDevice()->SetBlendState(myMaterial.BlendState.Get());
	}

	aGraphicsEngine->GetDevice()->DrawIndexed(static_cast<unsigned int>(myMesh.myNumIndices), 0, 0);

	aGraphicsEngine->GetDevice()->SetBlendState(nullptr);

	for (const auto& resource : myMaterial.Resources)
	{
		constexpr graphics::ShaderResource* dummy = nullptr;
		aGraphicsEngine->GetDevice()->BindShaderResources(&dummy, 1, resource.Slot, resource.Stages);
	}

#undef LOG_CATEGORY
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
