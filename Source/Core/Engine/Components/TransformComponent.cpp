#include "Engine.pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent() : 
	myIsDirty(false)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetParent(TransformComponent* aTransform) noexcept
{
	SetDirty();

	TransformComponent* oldParent = myParent;
	if (oldParent != nullptr)
	{
		oldParent->RemoveChild(this);
	}

	if (aTransform == this || aTransform == nullptr)
	{
		myParent = nullptr;
	}
	else
	{
		myParent = aTransform;
		aTransform->AddChild(this);
	}
}

void TransformComponent::SetParentKeepGlobals(TransformComponent* aTransform) noexcept
{
	CommonUtilities::Matrix4x4f globals = GetGlobalMatrix();
	SetParent(aTransform);
	SetGlobalMatrix(globals);
}

TransformComponent* TransformComponent::GetParent() const noexcept
{
	return myParent.Get();
}

void TransformComponent::OrphanAllChildren() noexcept
{
	CleanChildren();
	for (cs::ComponentRef<TransformComponent>& child : myChildren)
	{
		child->SilentRemoveParent();
	}
	myChildren.clear();
}

void TransformComponent::SetTranslation(CommonUtilities::Vector3f aTranslation) noexcept
{
	SetDirty();
	myTransform.SetTranslation(aTranslation);
}
CommonUtilities::Vector3f TransformComponent::GetTranslation() const noexcept
{
	return myTransform.GetTranslation();
}

void TransformComponent::SetRotation(CommonUtilities::Quaternionf aRotation) noexcept
{
	SetDirty();
	myTransform.SetRotation(aRotation);
}
CommonUtilities::Quaternionf TransformComponent::GetRotation() const noexcept
{
	return myTransform.GetRotation();
}

void TransformComponent::SetScale(CommonUtilities::Vector3f aScale) noexcept
{
	SetDirty();
	myTransform.SetScale(aScale);
}
CommonUtilities::Vector3f TransformComponent::GetScale() const noexcept
{
	return myTransform.GetScale();
}

void TransformComponent::SetGlobalTranslation(CommonUtilities::Vector3f aTranslation) noexcept
{
	SetTranslation((CommonUtilities::Vector4f(aTranslation, 1.f) * myParent->GetGlobalTransform().GetInverseMatrix()).xyz());
}
CommonUtilities::Vector3f TransformComponent::GetGlobalTranslation() const noexcept
{
	Clean();
	return myGlobalTransformCache.GetTranslation();
}

void TransformComponent::SetGlobalRotation(CommonUtilities::Quaternionf aRotation) noexcept
{
	SetRotation(myParent->GetGlobalRotation().Conjugate() * aRotation);
}
CommonUtilities::Quaternionf TransformComponent::GetGlobalRotation() const noexcept
{
	Clean();
	return myGlobalTransformCache.GetRotation();
}

void TransformComponent::SetMatrix(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
{
	SetDirty();
	myTransform.SetMatrix(aMatrix);
}
const CommonUtilities::Matrix4x4f& TransformComponent::GetMatrix() const noexcept
{
	return myTransform.GetMatrix();
}

void TransformComponent::SetGlobalMatrix(const CommonUtilities::Matrix4x4f& aMatrix) noexcept
{
	TransformComponent* parent = myParent;
	if (parent != nullptr)
	{
		SetMatrix(parent->GetGlobalTransform().GetInverseMatrix() * aMatrix);
	}
	else
	{
		SetMatrix(aMatrix);
	}
}
const CommonUtilities::Matrix4x4f& TransformComponent::GetGlobalMatrix() const noexcept
{
	Clean();
	return myGlobalTransformCache.GetMatrix();
}

const CommonUtilities::Transformf& TransformComponent::GetTransform() const noexcept
{
	return myTransform;
}

const CommonUtilities::Transformf& TransformComponent::GetGlobalTransform() const noexcept
{
	Clean();
	return myGlobalTransformCache;
}

void TransformComponent::OnDestroy()
{
	std::vector<cs::ComponentRef<TransformComponent>> children = std::move(myChildren);
	myChildren.clear();

	for (TransformComponent* child : children)
	{
		if (child != nullptr)	// Instead of calling CleanChildren, this is more effiscient as all children will be removed anyways.
		{
			GetSystem().Destroy(child->GetEntityHandle());
		}
	}
}

void TransformComponent::AddChild(TransformComponent* aTransform)
{
	if (aTransform != nullptr)
	{
		myChildren.push_back(aTransform);
	}
}

void TransformComponent::RemoveChild(TransformComponent* aTransform)
{
	std::erase(myChildren, aTransform);
}

void TransformComponent::SilentRemoveParent()
{
	SetDirty();
	myParent.Reset();
}

void TransformComponent::SetDirty() const
{
	CleanChildren();
	myIsDirty = true;
	for (TransformComponent* child : myChildren)
	{
		child->SetDirty();
	}
}

void TransformComponent::Clean() const
{
	if (myIsDirty)
	{
		TransformComponent* parent = myParent.Get();
		if (parent == nullptr)
		{
			myGlobalTransformCache = myTransform;
			return;
		}

		const CommonUtilities::Matrix4x4f globalMatrix = GetMatrix() * parent->GetGlobalMatrix();
		myGlobalTransformCache.SetMatrix(globalMatrix);

		myIsDirty = false;
	}
}

void TransformComponent::CleanChildren() const
{
	std::erase_if(myChildren, [](const cs::ComponentRef<TransformComponent>& aChild) { return aChild.Expired(); });
}
