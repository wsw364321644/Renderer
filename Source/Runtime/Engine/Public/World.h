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

    Actor* CreateActor();

    std::vector<Actor*> GetActors();
    void ItrActors(std::function<void(Actor*)>);

    template<typename inclass>
    std::vector<inclass*> GetComponent();
private:
    void OnAddComponent(Actor* InActor, ActorComponent* Component);
    void OnRemoveComponent(Actor* InActor, ActorComponent* Component);
    static int64_t ID_Counter;
    std::map<uuids::uuid, std::shared_ptr<Actor>> Actors;

    std::list<PrimitiveComponent*> CachedPrimitiveComponents;
};



template<typename inclass>
inline std::vector<inclass*> World::GetComponent()
{
    std::vector<inclass*> result;
    for (auto [id, actor] : Actors) {
        for (auto com : actor->GetComponents()) {
            if (dynamic_cast<inclass*>(com.get())) {
                result.push_back(dynamic_cast<inclass*>(com.get()));
            }
        }
    }
    return result;
}

