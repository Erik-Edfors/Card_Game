#pragma once
#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>
#include <Core/CommonUtilities/Math/Transform.hpp>

class TransformComponent : public cs::Component
{
public:
	TransformComponent();
	~TransformComponent();

	void SetParent(TransformComponent* aTransform) noexcept;
	void SetParentKeepGlobals(TransformComponent* aTransform) noexcept;
	TransformComponent* GetParent() const noexcept;

	void OrphanAllChildren() noexcept;



	void SetTranslation(CommonUtilities::Vector3f aTranslation) noexcept;
	CommonUtilities::Vector3f GetTranslation() const noexcept;

	void SetRotation(CommonUtilities::Quaternionf aRotation) noexcept;
	CommonUtilities::Quaternionf GetRotation() const noexcept;
	
	void SetScale(CommonUtilities::Vector3f aScale) noexcept;
	CommonUtilities::Vector3f GetScale() const noexcept;



	void SetGlobalTranslation(CommonUtilities::Vector3f aTranslation) noexcept;
	CommonUtilities::Vector3f GetGlobalTranslation() const noexcept;

	void SetGlobalRotation(CommonUtilities::Quaternionf aRotation) noexcept;
	CommonUtilities::Quaternionf GetGlobalRotation() const noexcept;

	// Do these even work?
	//void SetGlobalScale(CommonUtilities::Vector3f aTranslation) noexcept;
	//CommonUtilities::Vector3f GetGlobalScale() const noexcept;



	void SetMatrix(const CommonUtilities::Matrix4x4f& aMatrix) noexcept;
	const CommonUtilities::Matrix4x4f& GetMatrix() const noexcept;

	void SetGlobalMatrix(const CommonUtilities::Matrix4x4f& aMatrix) noexcept;
	const CommonUtilities::Matrix4x4f& GetGlobalMatrix() const noexcept;



	const CommonUtilities::Transformf& GetTransform() const noexcept;

	const CommonUtilities::Transformf& GetGlobalTransform() const noexcept;

protected:
	virtual void OnDestroy() override;

private:
	void AddChild(TransformComponent* aTransform);
	void RemoveChild(TransformComponent* aTransform);
	void SilentRemoveParent();

	void SetDirty() const;
	void Clean() const;
	void CleanChildren() const;

	cs::ComponentRef<TransformComponent> myParent;
	mutable std::vector<cs::ComponentRef<TransformComponent>> myChildren;

	CommonUtilities::Transformf myTransform;
	mutable CommonUtilities::Transformf myGlobalTransformCache;
	mutable bool myIsDirty;
};