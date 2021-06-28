#pragma once
#include <list>
#include <memory>
#include <string>
#include <uuid.h>
#include <Templates/Event.h>
#include "Component/ActorComponent.h"
#include "Component/StaticMeshComponent.h"


using ComponentChangeEvent = Delegate<void(Actor*, ActorComponent*)>;

class Actor
{
public:

    Actor();
    virtual ~Actor();

    template<typename inclass, typename... ArgsType>
    std::shared_ptr<inclass> AddComponent(ArgsType ...args) {

        ChildComponents.emplace_back(std::make_shared<inclass>( std::forward(args)...));
        ChildComponents.back().get()->OnRegister(this);
        OnAddComponent(this, ChildComponents.back().get());
        return std::dynamic_pointer_cast<inclass>(ChildComponents.back());
    }

    std::shared_ptr<ActorComponent> AddComponent(std::shared_ptr<ActorComponent> handle) {

        auto itr = std::find(ChildComponents.begin(), ChildComponents.end(), handle);
        if (itr == ChildComponents.end()) {
            ChildComponents.push_back(handle);
            ChildComponents.back().get()->OnRegister(this);
        }
        OnAddComponent(this, ChildComponents.back().get());
        return handle;
    }

    bool RemoveComponent(std::shared_ptr<ActorComponent> handle) {
        auto itr=std::find(ChildComponents.begin(), ChildComponents.end(), handle);
        if (itr != ChildComponents.end()) {
            (*itr)->OnUnRegister();
            OnRemoveComponent(this, itr->get());
            ChildComponents.erase(itr);
        }

        
    }

    template<typename inclass>
    bool RemoveComponentByClass() {
        ChildComponents.erase(std::remove_if(
            ChildComponents.begin(),
            ChildComponents.end(),
            [](std::shared_ptr<ActorComponent>& p) { 
                bool b = dynamic_cast<inclass>(p) != nullptr;
                if(b)
                    OnRemoveComponent(this, p->get());
                return b; 
            }
        ),
            ChildComponents.end());
    }
    uuids::uuid GetID() {
        return ID;
    }

    std::list<std::shared_ptr<ActorComponent>> GetComponents() { return ChildComponents; };

    ComponentChangeEvent OnAddComponent;
    ComponentChangeEvent OnRemoveComponent;

private:

    uuids::uuid ID;
    std::list<std::shared_ptr<ActorComponent>> ChildComponents;
};