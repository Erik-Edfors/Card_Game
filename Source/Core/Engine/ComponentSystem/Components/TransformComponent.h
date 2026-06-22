#pragma once
#include "Component.h"

#include <Core/CommonUtilities/Math/Transform.hpp>

#include <vector>

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	void SetParent(TransformComponent* aParent) noexcept;
	ComponentRef<TransformComponent> GetParent() const noexcept;

	void SetTranslationLocal(const CommonUtilities::Vector3f& aTranslation) noexcept;
	void SetTranslationGlobal(const CommonUtilities::Vector3f& aTranslation) noexcept;
	CommonUtilities::Vector3f GetTranslationLocal() const noexcept;
	CommonUtilities::Vector3f GetTranslationGlobal() noexcept;

	void SetScaleLocal(const CommonUtilities::Vector3f& aScale) noexcept;
	void SetScaleGlobal(const CommonUtilities::Vector3f& aScale) noexcept;
	CommonUtilities::Vector3f GetScaleLocal() const noexcept;
	CommonUtilities::Vector3f GetScaleGlobal() noexcept;

	void SetRotationLocal(const CommonUtilities::Quaternionf& aRotation) noexcept;
	void SetRotationGlobal(const CommonUtilities::Quaternionf& aRotation) noexcept;
	CommonUtilities::Quaternionf GetRotationLocal() const noexcept;
	CommonUtilities::Quaternionf GetRotationGlobal() noexcept;

	void SetMatrixLocal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept;
	void SetMatrixGlobal(const CommonUtilities::Matrix4x4f& aMatrix) noexcept;
	CommonUtilities::Matrix4x4f GetMatrixLocal() noexcept;
	CommonUtilities::Matrix4x4f GetMatrixGlobal() noexcept;

	CommonUtilities::Vector3f GetRight() noexcept;
	CommonUtilities::Vector3f GetUp() noexcept;
	CommonUtilities::Vector3f GetForward() noexcept;

	const std::vector<ComponentRef<TransformComponent>>& GetChildren() noexcept;

protected:
	void OnDestroy() override;

private:
	void FlagDirty() noexcept;
	void Clean() noexcept;

	bool myIsDirty;

	ComponentRef<TransformComponent> myParent;
	std::vector<ComponentRef<TransformComponent>> myChildren;

	CommonUtilities::Vector3f myTranslation;
	CommonUtilities::Vector3f myScale;
	CommonUtilities::Quaternionf myRotation;
	CommonUtilities::Matrix4x4f myMatrix;

	CommonUtilities::Vector3f myGlobalTranslation;
	CommonUtilities::Vector3f myGlobalScale;
	CommonUtilities::Quaternionf myGlobalRotation;
	CommonUtilities::Matrix4x4f myGlobalMatrix;
};