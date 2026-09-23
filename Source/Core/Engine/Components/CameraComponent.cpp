#include "Engine.pch.h"
#include "CameraComponent.h"
#include <Core/GraphicsEngine/Projection.h>
#include <Core/CommonUtilities/Math/Constants.h>

CameraComponent::CameraComponent() : 
	myProjection(CreatePerspectiveProjection(90.f * CommonUtilities::DegToRad, 16.f / 9.f, 0.1f, 10000.f))
{
}
CameraComponent::~CameraComponent()
{
}

void CameraComponent::SetProjection(const CommonUtilities::Matrix4x4f& aProjection) noexcept
{
	myProjection = aProjection;
}
CommonUtilities::Matrix4x4f CameraComponent::GetProjection() const noexcept
{
	return myProjection;
}

CommonUtilities::Matrix4x4f CameraComponent::GetView() const noexcept
{
	TransformComponent* transform = myTransform;
	if (transform == nullptr)
	{
		return {};
	}
	else return transform->GetGlobalTransform().GetInverseMatrix();
}

void CameraComponent::OnStart()
{
	myTransform = GetSystem().GetComponent<TransformComponent>(GetEntityHandle());
}
