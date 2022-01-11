#pragma once


#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // For HRESULT


#include "DeviceInputManager.h"
#include "GenericPlatform/Events.h"



class WindowsDeviceInputManager :public DeviceInputManager {
public:
	virtual ~WindowsDeviceInputManager();

	WindowsDeviceInputManager();

	bool IsInputMessage(uint32_t msg);
	bool IsKeyboardInputMessage(uint32_t msg);
	bool IsMouseInputMessage(uint32_t msg);
	


	bool ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
private:
	EMouseButton DecodeMouseButton(UINT messageID, WPARAM wParam, LPARAM lParam);

};