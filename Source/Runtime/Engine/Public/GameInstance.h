#pragma once
#include <vector>
#include <memory>
#include <string>

class FLocalPlayer;
class FGameInstance
{
public:

    FGameInstance();
    virtual ~FGameInstance();

    int32_t AddLocalPlayer(std::shared_ptr<FLocalPlayer> NewLocalPlayer);

    std::vector <std::shared_ptr<FLocalPlayer>> LocalPlayers;
};