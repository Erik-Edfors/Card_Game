#include "Client.pch.h"
#include "ButtonComponent.h"

#include <Core/Engine/Components/SpriteComponent.h>

namespace client
{
	ButtonComponent::ButtonComponent(const CommonUtilities::InputManager* aInput, SpriteComponent* aSprite) :
		myInput(aInput),
		mySprite(aSprite),
		myCallback(nullptr),
		myUserData(nullptr),
		myPressed(false),
		myButtonEnabled(true)
	{
	}
	ButtonComponent::~ButtonComponent()
	{
	}
	void ButtonComponent::SetButtonEnabled(bool aEnabled) noexcept
	{
		myButtonEnabled = aEnabled;
		if (not myButtonEnabled)
		{
			myPressed = false;
		}
	}
	bool ButtonComponent::IsButtonEnabled() const noexcept
	{
		return myButtonEnabled;
	}
	void ButtonComponent::SetCallback(Callback aCallback, void* aUserData) noexcept
	{
		myCallback = aCallback;
		myUserData = aUserData;
	}
	void ButtonComponent::SetSprite(SpriteComponent* aSprite) noexcept
	{
		mySprite = aSprite;
	}
	SpriteComponent* ButtonComponent::GetSprite() const noexcept
	{
		return mySprite.Get();
	}
	void ButtonComponent::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;
		if (not mySprite.Expired())
		{
			CommonUtilities::Vector4f tint = { 1.f, 1.f, 1.f, 1.f };

			if (myButtonEnabled)
			{
				if (myPressed)
				{
					if (mySprite->IsHovered())
					{
						tint = { 0.5f, 0.5f, 0.5f, 1.f };

						if (myCallback != nullptr)
						{
							if (myInput->GetKeyUp(CommonUtilities::Keys::MOUSELBUTTON))
							{
								myCallback(
									CallbackData{
										.Button = this,
										.UserData = myUserData
									}
								);

								myPressed = false;
							}
						}
					}
					else
					{
						myPressed = false;
					}
				}
				else
				{
					if (mySprite->IsHovered())
					{
						tint = { 0.75f, 0.75f, 0.75f, 1.f };

						if (myInput->GetKeyDown(CommonUtilities::Keys::MOUSELBUTTON))
						{
							myPressed = true;
						}
					}
				}
			}
			else
			{
				tint = { 0.25f, 0.25f, 0.25f, 1.f };
			}

			mySprite->SetTint(tint);
		}
	}
}
