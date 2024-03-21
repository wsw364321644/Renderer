#pragma once
#include "PlayerID.h"

class FLocalPlayer
{
public:

    FLocalPlayer();
    virtual ~FLocalPlayer();
	struct FPlatformUserId PlatformUserId;
};