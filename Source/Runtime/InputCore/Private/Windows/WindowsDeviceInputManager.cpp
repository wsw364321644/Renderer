#include "Windows/WindowsDeviceInputManager.h"
#include "GenericPlatform/GameFramework.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"
#include "SlateManager.h"
#include <typeinfo>
#include <LoggerHelper.h>



WindowsDeviceInputManager::~WindowsDeviceInputManager()
{
}

WindowsDeviceInputManager::WindowsDeviceInputManager() :FDeviceInputManager()
{
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	Rid[0].usUsage = 0x02;              // HID_USAGE_GENERIC_MOUSE
	Rid[0].dwFlags = RIDEV_NOLEGACY;    // adds mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = 0;

	Rid[1].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	Rid[1].usUsage = 0x06;              // HID_USAGE_GENERIC_KEYBOARD
	Rid[1].dwFlags = RIDEV_NOLEGACY;    // adds keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = 0;

	//if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
	//{
	//	//registration failed. Call GetLastError for the cause of the error
	//	GameFramework::Get().CreateLogger(ENGINE_LOG_NAME)->critical("Can not RegisterRawInputDevices with error code:", GetLastError());
	//}
}

bool WindowsDeviceInputManager::IsInputMessage(uint32_t msg)
{
	if (IsKeyboardInputMessage(msg) || IsMouseInputMessage(msg))
	{
		return true;
	}

	switch (msg)
	{
		// Raw input notification messages...
	case WM_INPUT:
	case WM_INPUT_DEVICE_CHANGE:
		return true;
	}
	return false;
}

bool WindowsDeviceInputManager::IsKeyboardInputMessage(uint32_t msg)
{
	switch (msg)
	{
		// Keyboard input notification messages...
	case WM_CHAR:
	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYUP:
	case WM_SYSCOMMAND:
		return true;
	}
	return false;
}

bool WindowsDeviceInputManager::IsMouseInputMessage(uint32_t msg)
{
	switch (msg)
	{
		// Mouse input notification messages...
	case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHOVER:
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_NCMOUSEHOVER:
	case WM_NCMOUSELEAVE:
	case WM_NCMOUSEMOVE:
	case WM_NCMBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCXBUTTONDBLCLK:
	case WM_NCXBUTTONDOWN:
	case WM_NCXBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		return true;
	}
	return false;
}

bool WindowsDeviceInputManager::ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{


	if (!IsInputMessage(msg)) { return false; }

	auto bLShift = (::GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0;
	auto bRShift = (::GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;
	auto bLControl = (::GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0;
	auto bRControl = (::GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0;
	auto bLAlt = (::GetAsyncKeyState(VK_LMENU) & 0x8000) != 0;
	auto bRAlt = (::GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
	auto bCapital = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;

	auto app = dynamic_cast<WindowsApplication*>(FGameFramework::Get().GetSlateManager()->GetGenericApplication());
	auto MessageHandler= app->GetMessageHandler();
	std::shared_ptr<WindowsWindow> pWindow = std::dynamic_pointer_cast<WindowsWindow>(app->GetWindowByHandle(hwnd).lock());


	switch (msg) {
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		bool bIsRepeat = (lParam & 0x40000000) != 0;

		MSG charMsg;

		// Get the Unicode character (UTF-16)
		unsigned int c = 0;
		// For printable characters, the next message will be WM_CHAR.
		// This message contains the character code we need to send the
		// KeyPressed event. Inspired by the SDL 1.2 implementation.
		if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR)
		{
			// GetMessage( &charMsg, hwnd, 0, 0 );
			c = static_cast<unsigned int>(charMsg.wParam);
		}
		MessageHandler->OnKeyDown(wParam, c, bIsRepeat);


		KeyCode      key = (KeyCode)wParam;
		KeyEventArgs keyEventArgs(key, c, KeyState::Pressed, bLShift, bRShift, bLControl, bRControl, bLAlt, bRAlt, bCapital);
		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnKeyPressed(keyEventArgs);
	}
	break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
		bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
		bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

		unsigned int c = 0;
		unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

		// Determine which key was released by converting the key code and
		// the scan code to a printable character (if possible). Inspired by
		// the SDL 1.2 implementation.
		unsigned char keyboardState[256];
		GetKeyboardState(keyboardState);
		wchar_t translatedCharacters[4];
		if (int result =
			ToUnicodeEx((UINT)wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
		{
			c = translatedCharacters[0];
		}
		MessageHandler->OnKeyUp(wParam, c, false);

		KeyCode      key = (KeyCode)wParam;
		KeyEventArgs keyEventArgs(key, c, KeyState::Released, bLShift, bRShift, bLControl, bRControl, bLAlt, bRAlt, bCapital);
		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnKeyReleased(keyEventArgs);
	}
	break;

	case WM_MOUSEMOVE:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		MouseMotionEventArgs mouseMotionEventArgs(lButton, mButton, rButton, control, shift, x, y);
		pWindow->OnMouseMoved(mouseMotionEventArgs);
		MessageHandler->OnMouseMove();

		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnMouseMoved(mouseMotionEventArgs);
	}
	break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(msg, wParam, lParam), ButtonState::Pressed, lButton,
			mButton, rButton, control, shift, x, y);
		pWindow->OnMouseButtonPressed(mouseButtonEventArgs);
		MessageHandler->OnMouseDown(pWindow, DecodeMouseButton(msg, wParam, lParam));

		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnMouseButtonPressed(mouseButtonEventArgs);
	}
	break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));



		MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(msg, wParam, lParam), ButtonState::Released, lButton,
			mButton, rButton, control, shift, x, y);
		pWindow->OnMouseButtonReleased(mouseButtonEventArgs);
		MessageHandler->OnMouseUp( DecodeMouseButton(msg, wParam, lParam));

		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnMouseButtonReleased(mouseButtonEventArgs);
	}
	break;
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(msg, wParam, lParam), ButtonState::Pressed, lButton,
			mButton, rButton, control, shift, x, y);
		MessageHandler->OnMouseDoubleClick(pWindow, DecodeMouseButton(msg, wParam, lParam));

		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnMouseButtonPressed(mouseButtonEventArgs);
	}
	break;
	case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
	{
		// The distance the mouse wheel is rotated.
		// A positive value indicates the wheel was rotated forwards (away
		//  the user). A negative value indicates the wheel was rotated
		//  backwards (toward the user).
		float zDelta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
		short keyStates = (short)LOWORD(wParam);

		bool lButton = (keyStates & MK_LBUTTON) != 0;
		bool rButton = (keyStates & MK_RBUTTON) != 0;
		bool mButton = (keyStates & MK_MBUTTON) != 0;
		bool shift = (keyStates & MK_SHIFT) != 0;
		bool control = (keyStates & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		// Convert the screen coordinates to client coordinates.
		POINT screenToClientPoint;
		screenToClientPoint.x = x;
		screenToClientPoint.y = y;
		::ScreenToClient(hwnd, &screenToClientPoint);

		MouseWheelEventArgs mouseWheelEventArgs(zDelta, lButton, mButton, rButton, control, shift,
			(int)screenToClientPoint.x, (int)screenToClientPoint.y);
		glm::vec2 pos(screenToClientPoint.x, screenToClientPoint.y);
		MessageHandler->OnMouseWheel(zDelta, pos);

		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnMouseWheel(mouseWheelEventArgs);
	}
	break;
	case WM_MOUSELEAVE:
	{
		EventArgs mouseLeaveEventArgs;
		pWindow->OnMouseLeave(mouseLeaveEventArgs);
		FGameFramework::Get().GetSlateManager()->FindWindowByNative(pWindow)->OnMouseLeave(mouseLeaveEventArgs);
	}
	break;
	case WM_INPUT: {
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
		{
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
			GetLogger(ENGINE_LOG_NAME)->warn("GetRawInputData does not return correct size !");
			break;
		}

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			printf(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n", raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.Reserved,
				raw->data.keyboard.ExtraInformation,
				raw->data.keyboard.Message,
				raw->data.keyboard.VKey);
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			printf("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n", raw->data.mouse.usFlags,
				raw->data.mouse.ulButtons,
				raw->data.mouse.usButtonFlags,
				raw->data.mouse.usButtonData,
				raw->data.mouse.ulRawButtons,
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY,
				raw->data.mouse.ulExtraInformation);

		}

		delete[] lpb;
		break;
	}
	}
	return true;
}


// Convert the message ID into a MouseButton ID
EMouseButton WindowsDeviceInputManager::DecodeMouseButton(UINT messageID, WPARAM wParam, LPARAM lParam)
{
	EMouseButton mouseButton = EMouseButton::Invalid;
	switch (messageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	{
		mouseButton = EMouseButton::Left;
	}
	break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	{
		mouseButton = EMouseButton::Right;
	}
	break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	{
		mouseButton = EMouseButton::Middle;
	}
	break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
	{
		mouseButton = (HIWORD(wParam) & XBUTTON1) ? EMouseButton::Thumb01 : EMouseButton::Thumb02;
	}
	break;
	}

	return mouseButton;
}