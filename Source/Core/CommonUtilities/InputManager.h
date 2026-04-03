#pragma once
#include <Windows.h>
#include <array>
#include "EnumKeys.h"
#include "Math\Vector2.hpp"

namespace CommonUtilities
{
	class InputManager
	{
	public:
		struct KeyState
		{
		public:
			bool Down;
			bool Changed;
		};

		InputManager();
		~InputManager();

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

		[[nodiscard]] bool GetKeyDown(const Keys aKey) const;

		[[nodiscard]] bool GetKeyPressed(const Keys aKey) const;

		[[nodiscard]] bool GetKeyUp(const Keys aKey) const;

		[[nodiscard]] bool GetKeyReleased(const Keys aKey) const;

		[[nodiscard]] const Vector2<float>& GetMouseDelta() const;
		[[nodiscard]] const Vector2<float>& GetMousePosition() const;
		[[nodiscard]] int GetMouseScrollDelta() const;

		[[nodiscard]] const std::array<KeyState, 256> GetStates() const;

		void UpdateInput();

	private:
		void _setState(int keyCode, bool aDown);

		std::array<KeyState, 256> myKeyStates;
		std::array<KeyState, 256> myPendingKeyStates;

		Vector2<float> myPendingMousePos;
		Vector2<float> myMousePos;
		Vector2<float> myPendingMouseDelta;
		Vector2<float> myMouseDelta;
		int myPendingMouseScrollDelta;
		int myMouseScrollDelta;
	};
}

