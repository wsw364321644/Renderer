#pragma once
#include <memory>
#include "Scene.h"
#include "VertexTypes.h"
#include "GameFramework/ResourceManager.h"
#include "Component/PrimitiveComponent.h"


class StaticMeshComponent :public PrimitiveComponent
{
public:

    StaticMeshComponent();
    virtual ~StaticMeshComponent();

    void LoadModelFile(const std::wstring& fileName, const std::function<bool(float)>& loadingProgress);
   
private:
    std::shared_ptr<ModelFileInfo> ModelFile;

};
