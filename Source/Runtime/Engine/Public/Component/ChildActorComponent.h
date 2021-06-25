#pragma once
#include <memory>
#include<vector>
#include "Component/ActorComponent.h"

class ChildActorComponent:public ActorComponent
{
public:

    ChildActorComponent();
    virtual ~ChildActorComponent();

private:
    std::vector<std::shared_ptr<Actor>> ChildActors;
};