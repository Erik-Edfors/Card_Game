#include "Shared.pch.h"
#include "TransformComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "ComponentSystem/Actor.h"

namespace shared
{
	TransformComponent::TransformComponent()
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
		aTranslation;
	}
	CommonUtilities::Vector3f TransformComponent::GetTranslationLocal() const noexcept
	{
		return CommonUtilities::Vector3f();
	}
	CommonUtilities::Vector3f TransformComponent::GetTranslationGlobal() noexcept
	{
		return CommonUtilities::Vector3f();
	}
	void TransformComponent::SetScaleLocal(const CommonUtilities::Vector3f& aScale) noexcept
	{
		FlagDirty();
		myScale = aScale;
	}
	void TransformComponent::SetScaleGlobal(const CommonUtilities::Vector3f& aScale) noexcept
	{
		aScale;
	}
	CommonUtilities::Vector3f TransformComponent::GetScaleLocal() const noexcept
	{
		return CommonUtilities::Vector3f();
	}
	CommonUtilities::Vector3f TransformComponent::GetScaleGlobal() noexcept
	{
		return CommonUtilities::Vector3f();
	}
	void TransformComponent::SetRotationLocal(const CommonUtilities::Quaternionf& aRotation) noexcept
	{
		FlagDirty();
		myRotation = aRotation;
	}
	void TransformComponent::SetRotationGlobal(const CommonUtilities::Quaternionf& aRotation) noexcept
	{
		aRotation;
	}
	CommonUtilities::Quaternionf TransformComponent::GetRotationLocal() const noexcept
	{
		return CommonUtilities::Quaternionf();
	}
	CommonUtilities::Quaternionf TransformComponent::GetRotationGlobal() noexcept
	{
		return CommonUtilities::Quaternionf();
	}
	void TransformComponent::SetMatrixLocal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
	{
		FlagDirty();
		myMatrix = aMatrix;
		myMatrix.Decompose(myTranslation, myRotation, myScale);
	}
	void TransformComponent::SetMatrixGlobal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
	{
		aMatrix;
	}
	CommonUtilities::Matrix4x4f TransformComponent::GetMatrixLocal() noexcept
	{
		return CommonUtilities::Matrix4x4f();
	}
	CommonUtilities::Matrix4x4f TransformComponent::GetMatrixGlobal() noexcept
	{
		return CommonUtilities::Matrix4x4f();
	}
	const std::vector<ComponentRef<TransformComponent>>& shared::TransformComponent::GetChildren() noexcept
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

	void shared::TransformComponent::FlagDirty() noexcept
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
}