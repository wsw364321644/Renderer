#include "GameInstance.h"
#include "LocalPlayer.h"
FGameInstance::FGameInstance()
{
}

FGameInstance::~FGameInstance()
{
}

int32_t FGameInstance::AddLocalPlayer(std::shared_ptr<FLocalPlayer> NewLocalPlayer)
{
    LocalPlayers.push_back(NewLocalPlayer);
    NewLocalPlayer->PlatformUserId = LocalPlayers.size() - 1;
    return LocalPlayers.size()-1;
}
