#include "DeviceInputManager.h"
#include "GenericPlatform/GameFramework.h"
#include "GenericPlatform/GenericWindow.h"
#include "Misc/Helpers.h"
#include <memory>

FPlatformUserId FDefaultInputMapper::GetUserIndexForMouse(std::shared_ptr<FGenericWindow>& Window, FInputDeviceId InputDeviceId) const
{
    return FPlatformUserId(0);
}

FPlatformUserId FDefaultInputMapper::GetUserIndexForKeyboard(std::shared_ptr<FGenericWindow>& Window, int32_t KeyCode, FInputDeviceId InputDeviceId) const
{
    return FPlatformUserId(0);
}

FPlatformUserId FDefaultInputMapper::GetUserIndexForController(std::shared_ptr<FGenericWindow>& Window, FInputDeviceId InputDeviceId) const
{
    return FPlatformUserId(0);
}



FDeviceInputManager::~FDeviceInputManager()
{
}

FDeviceInputManager::FDeviceInputManager()
{
}

glm::vec2 FDeviceInputManager::GetCursorPosition()
{
    return InputUser.CursorPosition;
}

