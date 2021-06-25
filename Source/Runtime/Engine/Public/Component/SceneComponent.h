#pragma once

#include <DirectXMath.h>
#include "Component/ActorComponent.h"


class SceneComponent:public ActorComponent
{
public:

    SceneComponent();
    virtual ~SceneComponent();

private:
    DirectX::XMFLOAT3 Location;
    DirectX::XMFLOAT3 Rotation;
    DirectX::XMFLOAT3 Scale;
};