#include "Engine.pch.h"
#include "TransformComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "ComponentSystem/Actor.h"

TransformComponent::TransformComponent() :
	myIsDirty(false),
	myTranslation(0.f, 0.f, 0.f),
	myRotation(1.f, 0.f, 0.f, 0.f),
	myScale(1.f, 1.f, 1.f)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetParent(TransformComponent* aParent) noexcept
{
	FlagDirty();
	myParent = aParent;
}

ComponentRef<TransformComponent> TransformComponent::GetParent() const noexcept
{
	return myParent;
}
void TransformComponent::SetTranslationLocal(const CommonUtilities::Vector3f& aTranslation) noexcept
{
	FlagDirty();
	myTranslation = aTranslation;
}
void TransformComponent::SetTranslationGlobal(const CommonUtilities::Vector3f& aTranslation) noexcept
{
	//TODO: Implement.
	SetTranslationLocal(aTranslation);
}
CommonUtilities::Vector3f TransformComponent::GetTranslationLocal() const noexcept
{
	return myTranslation;
}
CommonUtilities::Vector3f TransformComponent::GetTranslationGlobal() noexcept
{
	//TODO: Implement.
	return GetTranslationLocal();
}
void TransformComponent::SetScaleLocal(const CommonUtilities::Vector3f& aScale) noexcept
{
	FlagDirty();
	myScale = aScale;
}
void TransformComponent::SetScaleGlobal(const CommonUtilities::Vector3f& aScale) noexcept
{
	//TODO: Implement.
	SetScaleLocal(aScale);
}
CommonUtilities::Vector3f TransformComponent::GetScaleLocal() const noexcept
{
	return myScale;
}
CommonUtilities::Vector3f TransformComponent::GetScaleGlobal() noexcept
{
	//TODO: Implement.
	return GetScaleLocal();
}
void TransformComponent::SetRotationLocal(const CommonUtilities::Quaternionf& aRotation) noexcept
{
	FlagDirty();
	myRotation = aRotation;
}
void TransformComponent::SetRotationGlobal(const CommonUtilities::Quaternionf& aRotation) noexcept
{
	//TODO: Implement.
	SetRotationLocal(aRotation);
}
CommonUtilities::Quaternionf TransformComponent::GetRotationLocal() const noexcept
{
	return myRotation;
}
CommonUtilities::Quaternionf TransformComponent::GetRotationGlobal() noexcept
{
	//TODO: Implement.
	return GetRotationLocal();
}
void TransformComponent::SetMatrixLocal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
{
	FlagDirty();
	myMatrix = aMatrix;
	myMatrix.Decompose(myTranslation, myRotation, myScale);
}
void TransformComponent::SetMatrixGlobal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
{
	//TODO: Implement.
	SetMatrixLocal(aMatrix);
}
CommonUtilities::Matrix4x4f TransformComponent::GetMatrixLocal() noexcept
{
	Clean();
	return myMatrix;
}
CommonUtilities::Matrix4x4f TransformComponent::GetMatrixGlobal() noexcept
{
	//TODO: Implement.
	return GetMatrixLocal();
}
const std::vector<ComponentRef<TransformComponent>>& TransformComponent::GetChildren() noexcept
{
	return myChildren;
}

void TransformComponent::OnDestroy()
{
	for (const ComponentRef<TransformComponent>& child : GetChildren())
	{
		if (child && child->GetActor())
		{
			child->GetActor()->Destroy();
		}
	}
}

void TransformComponent::FlagDirty() noexcept
{
	myIsDirty = true;
	for (const ComponentRef<TransformComponent>& child : GetChildren())
	{
		if (child)
		{
			child->FlagDirty();
		}
	}
}
void TransformComponent::Clean() noexcept
{
	if (myIsDirty)
	{
		myMatrix.Compose(myTranslation, myRotation, myScale);

		myIsDirty = false;
	}
}