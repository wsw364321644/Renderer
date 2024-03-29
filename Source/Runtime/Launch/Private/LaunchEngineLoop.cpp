#include "LaunchEngineLoop.h"
#include "GenericPlatform/GameFramework.h"
#include "GenericPlatform/PlatformApplicationMisc.h"
FEngineLoop::FEngineLoop()
{
}

bool FEngineLoop::Init()
{
    return false;
}

void FEngineLoop::Tick()
{
    FGameFramework::Get().Tick();
    FPlatformApplicationMisc::PumpMessages(true);
}

void FEngineLoop::Run()
{
    while (!FGameFramework::Get().GetRequestStop()) {
        Tick();
    }
    FPlatformApplicationMisc::RequestExitWithStatus(false, 0);
}
