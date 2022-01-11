#pragma once

#include <stdint.h>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "GenericPlatform/Events.h"
class DeviceInputManager{
public:
	virtual ~DeviceInputManager();

	DeviceInputManager();

private:

	std::unordered_map<std::type_index, std::vector<std::shared_ptr<EventArgs>>> EventQueues;
};