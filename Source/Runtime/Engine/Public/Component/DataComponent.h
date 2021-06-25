#pragma once
#include <string>
#include<map>
#include "Component/ActorComponent.h"

class DataComponent :public ActorComponent
{
public:

    DataComponent();
    virtual ~DataComponent();
    void SetValue(std::string key, std::string value) { Obj.emplace(key, value); }
private:
    std::map<std::string, std::string> Obj;
};