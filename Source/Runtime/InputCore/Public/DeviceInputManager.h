#pragma once

#include <stdint.h>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <glm/vec2.hpp>
#include "GenericPlatform/Events.h"
#include "PlayerID.h"
#include "DeviceID.h"

class FGenericWindow;
class IInputMapper
{
public:
	virtual FPlatformUserId GetUserIndexForMouse(std::shared_ptr< FGenericWindow >& Window, FInputDeviceId InputDeviceId = INPUTDEVICEID_NONE) const = 0;
	virtual FPlatformUserId GetUserIndexForKeyboard(std::shared_ptr< FGenericWindow >& Window,int32_t KeyCode, FInputDeviceId InputDeviceId = INPUTDEVICEID_NONE) const = 0;
	virtual FPlatformUserId GetUserIndexForController(std::shared_ptr< FGenericWindow >& Window,FInputDeviceId InputDeviceId) const = 0;
};


class FDefaultInputMapper : public IInputMapper
{
public:
	virtual FPlatformUserId GetUserIndexForMouse(std::shared_ptr< FGenericWindow >& Window, FInputDeviceId InputDeviceId = INPUTDEVICEID_NONE) const override;
	virtual FPlatformUserId GetUserIndexForKeyboard(std::shared_ptr< FGenericWindow >& Window, int32_t KeyCode, FInputDeviceId InputDeviceId = INPUTDEVICEID_NONE) const override;
	virtual FPlatformUserId GetUserIndexForController(std::shared_ptr< FGenericWindow >& Window, FInputDeviceId InputDeviceId) const override;
};

class FInputUser
{
public: 
	glm::vec2 CursorPosition;
};


class FDeviceInputManager{
public:
	virtual ~FDeviceInputManager();

	FDeviceInputManager();

	glm::vec2 GetCursorPosition();
private:
	FDefaultInputMapper InputMapper;
	FInputUser  InputUser;
	//std::unordered_map<std::type_index, std::vector<std::shared_ptr<EventArgs>>> EventQueues;
};