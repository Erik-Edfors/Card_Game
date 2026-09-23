#include "Engine.pch.h"
#include "CameraControllerComponent.h"
#include <Core/CommonUtilities/InputManager.h>

CameraControllerComponent::CameraControllerComponent(const CommonUtilities::InputManager* aInputManager) noexcept :
	myInput(aInputManager),
	mySpeed(1.f),
	myMouseSensitivity(0.005f)
{
}

CameraControllerComponent::~CameraControllerComponent() noexcept
{
}

void CameraControllerComponent::SetSpeed(float aSpeed) noexcept
{
	mySpeed = aSpeed;
}

float CameraControllerComponent::GetSpeed() const noexcept
{
	return mySpeed;
}

void CameraControllerComponent::SetMouseSensitivity(float aSensitivity) noexcept
{
	myMouseSensitivity = aSensitivity;
}

float CameraControllerComponent::GetMouseSensitivity() const noexcept
{
	return myMouseSensitivity;
}

void CameraControllerComponent::OnStart()
{
	myTransform = GetSystem().GetComponent<TransformComponent>(GetEntityHandle());
}

void CameraControllerComponent::OnLateUpdate(float aDeltaTime)
{
	TransformComponent* transform = myTransform.Get();

	if (myInput->GetKeyPressed(CommonUtilities::Keys::MOUSERBUTTON))
	{
		float speed = mySpeed * aDeltaTime;

		CommonUtilities::Vector3f relativeMove;
		CommonUtilities::Vector2f mouseMove = myInput->GetMouseDelta() * myMouseSensitivity;

		const float scroll = static_cast<float>(myInput->GetMouseScrollDelta());

		if (scroll != 0.f)
		{
			mySpeed *= std::expf(0.1f * scroll);
		}

		if (myInput->GetKeyPressed(CommonUtilities::Keys::LSHIFT))
		{
			speed *= 5.f;
		}


		if (myInput->GetKeyPressed(CommonUtilities::Keys::A))
		{
			relativeMove += CommonUtilities::Vector3f(-speed, 0.f, 0.f);
		}
		if (myInput->GetKeyPressed(CommonUtilities::Keys::D))
		{
			relativeMove += CommonUtilities::Vector3f(speed, 0.f, 0.f);
		}
		if (myInput->GetKeyPressed(CommonUtilities::Keys::W))
		{
			relativeMove += CommonUtilities::Vector3f(0.f, 0.f, speed);
		}
		if (myInput->GetKeyPressed(CommonUtilities::Keys::S))
		{
			relativeMove += CommonUtilities::Vector3f(0.f, 0.f, -speed);
		}
		if (myInput->GetKeyPressed(CommonUtilities::Keys::Q))
		{
			relativeMove += CommonUtilities::Vector3f(0.f, -speed, 0.f);
		}
		if (myInput->GetKeyPressed(CommonUtilities::Keys::E))
		{
			relativeMove += CommonUtilities::Vector3f(0.f, speed, 0.f);
		}

		const CommonUtilities::Vector3f globalMove = (relativeMove.xyz0() * transform->GetGlobalMatrix()).xyz();
		const CommonUtilities::Quaternionf rotationX = CommonUtilities::Quaternionf({ 1.f, 0.f, 0.f }, mouseMove.y);
		const CommonUtilities::Quaternionf rotationY = CommonUtilities::Quaternionf({ 0.f, 1.f, 0.f }, mouseMove.x);

		transform->SetTranslation(transform->GetTranslation() + globalMove);
		transform->SetRotation(rotationY * transform->GetRotation() * rotationX);
	}
}
