#pragma once

#include "GenericPlatform/GenericPlatformApplicationMisc.h"
static int32_t bEnableHighDPIAwareness = 1;
bool FGenericPlatformApplicationMisc::IsHighDPIAwarenessEnabled()
{
    return bEnableHighDPIAwareness != 0;
}
