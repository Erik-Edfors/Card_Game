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
	if (not myParent.Expired())
	{
		SetTranslationLocal((aTranslation.xyz1() * myParent->GetMatrixGlobal().GetInverse()).xyz());
	}
	else
	{
		SetTranslationLocal(aTranslation);
	}
}
CommonUtilities::Vector3f TransformComponent::GetTranslationLocal() const noexcept
{
	return myTranslation;
}
CommonUtilities::Vector3f TransformComponent::GetTranslationGlobal() noexcept
{
	Clean();
	return myGlobalTranslation;
}
void TransformComponent::SetScaleLocal(const CommonUtilities::Vector3f& aScale) noexcept
{
	FlagDirty();
	myScale = aScale;
}
void TransformComponent::SetScaleGlobal(const CommonUtilities::Vector3f& aScale) noexcept
{
	//HACK: Probably doesn't work.
	if (not myParent.Expired())
	{
		SetScaleLocal((aScale.xyz0() * myParent->GetMatrixGlobal().GetInverse()).xyz());
	}
	else
	{
		SetScaleLocal(aScale);
	}
}
CommonUtilities::Vector3f TransformComponent::GetScaleLocal() const noexcept
{
	return myScale;
}
CommonUtilities::Vector3f TransformComponent::GetScaleGlobal() noexcept
{
	Clean();
	return myGlobalScale;
}
void TransformComponent::SetRotationLocal(const CommonUtilities::Quaternionf& aRotation) noexcept
{
	FlagDirty();
	myRotation = aRotation;
}
void TransformComponent::SetRotationGlobal(const CommonUtilities::Quaternionf& aRotation) noexcept
{
	if (not myParent.Expired())
	{
		SetRotationLocal(aRotation * myParent->GetRotationGlobal().Conjugate());
	}
	else
	{
		SetRotationLocal(aRotation);
	}
}
CommonUtilities::Quaternionf TransformComponent::GetRotationLocal() const noexcept
{
	return myRotation;
}
CommonUtilities::Quaternionf TransformComponent::GetRotationGlobal() noexcept
{
	Clean();
	return myGlobalRotation;
}
void TransformComponent::SetMatrixLocal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
{
	FlagDirty();
	myMatrix = aMatrix;
	myMatrix.Decompose(&myTranslation, &myRotation, &myScale);
}
void TransformComponent::SetMatrixGlobal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
{
	SetMatrixLocal(aMatrix * GetMatrixGlobal().GetInverse());
}
CommonUtilities::Matrix4x4f TransformComponent::GetMatrixLocal() noexcept
{
	Clean();
	return myMatrix;
}
CommonUtilities::Matrix4x4f TransformComponent::GetMatrixGlobal() noexcept
{
	Clean();
	return myGlobalMatrix;
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

		if (not myParent.Expired())
		{
			myGlobalMatrix = myMatrix * myParent->GetMatrixGlobal();
			myGlobalMatrix.Decompose(&myGlobalTranslation, &myGlobalRotation, &myGlobalScale);
		}
		else
		{
			myGlobalTranslation = myTranslation;
			myGlobalRotation = myRotation;
			myGlobalScale = myScale;
			myGlobalMatrix = myMatrix;
		}
	}
}