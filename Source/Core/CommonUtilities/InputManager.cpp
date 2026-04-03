#include "InputManager.h"
#include "EnumKeys.h"
#include <iostream>

namespace CommonUtilities
{
	InputManager::InputManager()
	{
		KeyState defaultState = {};
		defaultState.Down = false;
		defaultState.Changed = false;

		myKeyStates.fill(defaultState);
		myPendingKeyStates.fill(defaultState);

		myPendingMouseScrollDelta = 0;
		myMouseScrollDelta = 0;
	}
	InputManager::~InputManager()
	{
	}
	bool InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_MOUSEMOVE:
		{
			POINTS newPos = MAKEPOINTS(lParam);
			myPendingMouseDelta.x = newPos.x - myMousePos.x;
			myPendingMouseDelta.y = newPos.y - myMousePos.y;
			myPendingMousePos.x = newPos.x;
			myPendingMousePos.y = newPos.y;
			return true;
		}
		case WM_MOUSEWHEEL:
		{
			short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			myPendingMouseScrollDelta += static_cast<int>(wheelDelta / WHEEL_DELTA);
			return true;
		}
		case WM_KEYDOWN: case WM_SYSKEYDOWN: case WM_KEYUP: case WM_SYSKEYUP:
		{
			bool keyDown = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;

			WPARAM virtualKey;
			const UINT scancode = (lParam & 0x00ff0000) >> 16;
			const bool extended = (lParam & 0x01000000) != 0;

			switch (wParam)
			{
			case VK_SHIFT:
				virtualKey = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
				break;
			case VK_CONTROL:
				virtualKey = extended ? VK_RCONTROL : VK_LCONTROL;
				break;
			case VK_MENU:
				virtualKey = extended ? VK_RMENU : VK_LMENU;
				break;
			default:
				virtualKey = wParam;
				break;
			}

			_setState(static_cast<int>(wParam), keyDown);
			_setState(static_cast<int>(virtualKey), keyDown);
			return true;
		}
		case WM_LBUTTONDOWN:			_setState(static_cast<int>(Keys::MOUSELBUTTON), true);																return true;
		case WM_LBUTTONUP:				_setState(static_cast<int>(Keys::MOUSELBUTTON), false);																return true;
		case WM_RBUTTONDOWN:			_setState(static_cast<int>(Keys::MOUSERBUTTON), true);																return true;
		case WM_RBUTTONUP:				_setState(static_cast<int>(Keys::MOUSERBUTTON), false);																return true;
		case WM_MBUTTONDOWN:			_setState(static_cast<int>(Keys::MBUTTON), true);																		return true;
		case WM_MBUTTONUP:				_setState(static_cast<int>(Keys::MBUTTON), false);																	return true;
		case WM_XBUTTONDOWN:			_setState(static_cast<int>(GET_KEYSTATE_WPARAM(wParam) == 1 ? Keys::MOUSEXBUTTON1 : Keys::MOUSEXBUTTON2), true);		return true;
		case WM_XBUTTONUP:				_setState(static_cast<int>(GET_KEYSTATE_WPARAM(wParam) == 1 ? Keys::MOUSEXBUTTON1 : Keys::MOUSEXBUTTON2), false);		return true;
		default: return false;
		}
	}
	bool InputManager::GetKeyDown(const Keys aKey) const
	{
		return myKeyStates[static_cast<int>(aKey)].Down && myKeyStates[static_cast<int>(aKey)].Changed;
	}
	bool InputManager::GetKeyPressed(const Keys aKey) const
	{
		return myKeyStates[static_cast<int>(aKey)].Down;
	}
	bool InputManager::GetKeyUp(const Keys aKey) const
	{
		return not myKeyStates[static_cast<int>(aKey)].Down && myKeyStates[static_cast<int>(aKey)].Changed;
	}
	bool InputManager::GetKeyReleased(const Keys aKey) const
	{
		return not myKeyStates[static_cast<int>(aKey)].Down;
	}
	const Vector2<float>& InputManager::GetMouseDelta() const
	{
		return myMouseDelta;
	}
	const Vector2<float>& InputManager::GetMousePosition() const
	{
		return myMousePos;
	}
	int InputManager::GetMouseScrollDelta() const
	{
		return myMouseScrollDelta;
	}
	const std::array<InputManager::KeyState, 256> InputManager::GetStates() const
	{
		return myKeyStates;
	}
	void InputManager::UpdateInput()
	{
		myKeyStates = myPendingKeyStates;
		for (KeyState& keyState : myPendingKeyStates)
		{
			keyState.Changed = false;
		}

		myMouseDelta = myPendingMouseDelta;
		myPendingMouseDelta = Vector2<float>(0.f, 0.f);
		myMousePos = myPendingMousePos;

		myMouseScrollDelta = myPendingMouseScrollDelta;
		myPendingMouseScrollDelta = 0;
	}
	void InputManager::_setState(int keyCode, bool aDown)
	{
		KeyState& state = myPendingKeyStates[keyCode];
		state.Down = aDown;
		state.Changed = true;
	}
}