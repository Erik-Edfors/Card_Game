#pragma once
#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

#include <Core/CommonUtilities/InputManager.h>

class SpriteComponent;

namespace client
{
	class ButtonComponent : public cs::Component
	{
	public:
		struct CallbackData
		{
			cs::ComponentRef<ButtonComponent> Button;
			void* UserData;
		};
		using Callback = void(*)(const CallbackData& aData);

		ButtonComponent(const CommonUtilities::InputManager* aInput, SpriteComponent* aSprite);
		~ButtonComponent();

		void SetButtonEnabled(bool aEnabled) noexcept;
		bool IsButtonEnabled() const noexcept;

		void SetCallback(Callback aCallback, void* aUserData) noexcept;

		void SetSprite(SpriteComponent* aSprite) noexcept;
		SpriteComponent* GetSprite() const noexcept;

	protected:
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		const CommonUtilities::InputManager* myInput;

		cs::ComponentRef<SpriteComponent> mySprite;

		bool myButtonEnabled;
		bool myPressed;

		Callback myCallback;
		void* myUserData;
	};
}