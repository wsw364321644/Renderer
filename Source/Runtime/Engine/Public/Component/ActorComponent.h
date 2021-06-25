#pragma once

#include <vector>
#include <memory>

class Actor;
class ActorComponent
{
public:

    ActorComponent();
    virtual ~ActorComponent();

    virtual void OnRegister(Actor* owner) {
        Owner = owner;
    }

    virtual void OnUnRegister() {
        Owner=nullptr;
    }
private:
    Actor* Owner;
    std::vector<std::shared_ptr< ActorComponent>> ChildComponents;
};