#pragma once
#include "TransformComponent.h"

class TransformComponent;

namespace CommonUtilities
{
	class InputManager;
}

class CameraControllerComponent : public cs::Component
{
public:
	CameraControllerComponent(const CommonUtilities::InputManager* aInputManager) noexcept;
	~CameraControllerComponent() noexcept;

	void SetSpeed(float aSpeed) noexcept;
	float GetSpeed() const noexcept;

	void SetMouseSensitivity(float aSensitivity) noexcept;
	float GetMouseSensitivity() const noexcept;

protected:
	virtual void OnStart() override;
	virtual void OnLateUpdate(float aDeltaTime) override;

private:
	cs::ComponentRef<TransformComponent> myTransform;
	const CommonUtilities::InputManager* myInput;
	float mySpeed;
	float myMouseSensitivity;
};