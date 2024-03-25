#pragma once

#include <vector>
#include <memory>

class FActor;
class ActorComponent
{
public:

    ActorComponent();
    virtual ~ActorComponent();

    virtual void OnRegister(FActor* owner) {
        Owner = owner;
    }

    virtual void OnUnRegister() {
        Owner=nullptr;
    }
private:
    FActor* Owner;
    std::vector<std::shared_ptr< ActorComponent>> ChildComponents;
};