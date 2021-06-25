
#include "Component/StaticMeshComponent.h"
#include <DirectXMath.h>
#include "VertexTypes.h"
#include "Misc/MathHelper.h"
#include "GameFramework/ResourceManager.h"
using namespace DirectX;
StaticMeshComponent::StaticMeshComponent() {

}

StaticMeshComponent::~StaticMeshComponent() {

}

void StaticMeshComponent::LoadModelFile(const std::wstring& fileName, const std::function<bool(float)>& loadingProgress)
{
	ModelFile=ResourceManager::GetResourceManager()->LoadModelFile(fileName, loadingProgress);
}

