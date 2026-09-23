#pragma once
#include "TransformComponent.h"

class CameraComponent : public cs::Component
{
public:
	CameraComponent();
	~CameraComponent();

	void SetProjection(const CommonUtilities::Matrix4x4f& aProjection) noexcept;
	CommonUtilities::Matrix4x4f GetProjection() const noexcept;

	CommonUtilities::Matrix4x4f GetView() const noexcept;

protected:
	virtual void OnStart() override;

private:
	CommonUtilities::Matrix4x4f myProjection;
	cs::ComponentRef<TransformComponent> myTransform;
};