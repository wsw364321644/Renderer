#pragma once

#include <map>
#include <memory>
#include "GameFramework/Actor.h"
#include <type_traits>
class World
{
public:

    World();
    virtual ~World();

    void Render();

    std::shared_ptr<Actor> CreateActor() {
        auto actor = new Actor();
        Actors.emplace(actor->GetID(),actor);
        return Actors[actor->GetID()];
    }
private:
    static int64_t ID_Counter;
    std::map<uuids::uuid, std::shared_ptr<Actor>> Actors;
};