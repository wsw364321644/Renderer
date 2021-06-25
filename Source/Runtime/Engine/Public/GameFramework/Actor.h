#pragma once
#include <list>
#include <memory>
#include <string>
#include <uuid.h>
#include "Component/ActorComponent.h"
#include "Component/StaticMeshComponent.h"

class Actor
{
public:

    Actor();
    virtual ~Actor();

    template<typename inclass, typename... ArgsType>
    std::shared_ptr<inclass> AddComponent(ArgsType ...args) {

        ChildComponents.emplace_back(std::make_shared<inclass>( std::forward(args)...));
        ChildComponents.back().get()->OnRegister(this);
        return std::dynamic_pointer_cast<inclass>(ChildComponents.back());
    }

    std::shared_ptr<ActorComponent> AddComponent(std::shared_ptr<ActorComponent> handle) {

        auto itr = std::find(ChildComponents.begin(), ChildComponents.end(), handle);
        if (itr == ChildComponents.end()) {
            ChildComponents.push_back(handle);
            ChildComponents.back().get()->OnRegister(this);
        }
        return handle;
    }

    bool DeleteComponent(std::shared_ptr<ActorComponent> handle) {
        auto itr=std::find(ChildComponents.begin(), ChildComponents.end(), handle);
        if (itr != ChildComponents.end()) {
            (*itr)->OnUnRegister();
            ChildComponents.erase(itr);
        }
    }

    template<typename inclass>
    bool DeleteComponentByClass() {
        ChildComponents.erase(std::remove_if(
            ChildComponents.begin(),
            ChildComponents.end(),
            [](std::shared_ptr<ActorComponent>& p) { return dynamic_cast<inclass>(p) != nullptr; }
        ),
            ChildComponents.end());
    }
    uuids::uuid GetID() {
        return ID;
    }
private:

    uuids::uuid ID;
    std::list<std::shared_ptr<ActorComponent>> ChildComponents;
};