#pragma once

#include <memory>
#include <string>
#include<map>
#include "Misc/MathHelper.h"
#include "DirectXTex.h"

#include "Component/ActorComponent.h"

struct  aiScene;
namespace Assimp {
    class  Importer;
}

struct ModelFileInfo {
    const aiScene* Scene;
    std::shared_ptr<Assimp::Importer> PImporter;
};

struct TextureInfo{
    DirectX::TexMetadata  metadata;
    DirectX::ScratchImage scratchImage;
};

struct MeshInfo {
    std::vector<VertexAttributes> Vertices;
    std::vector<uint16_t> Indices;
};

//todo threadsafe
//todo load thread
class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();
    static ResourceManager* GetResourceManager();
    std::shared_ptr<ModelFileInfo> LoadModelFile(const std::wstring& fileName,const std::function<bool(float)>& loadingProgress);
    std::shared_ptr<TextureInfo> LoadTextureFile(const std::wstring path, bool sRGB);

    std::shared_ptr<MeshInfo> MakeMesh(std::wstring name, std::vector<VertexAttributes> Vertices,std::vector<uint16_t> Indices);
    std::shared_ptr<TextureInfo> MakeTexture(std::wstring name, DirectX::TexMetadata, DirectX::ScratchImage);
private:

    std::map<std::wstring, std::shared_ptr<ModelFileInfo>> LoadedModelFile;
    std::map<std::wstring, std::shared_ptr<TextureInfo>> LoadedTextureFile;

    std::map<std::wstring, std::shared_ptr<MeshInfo>> CachedMesh;
};