#include <GameFramework/ResourceManager.h>

#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/anim.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#include <DirectXTex.h>
using namespace DirectX;
//todo remove platformspecific
#include <Windows/PlatformHelper.h>
#include <filesystem>
#include<assert.h>


// A progress handler for Assimp
class WrapProgressHandler : public Assimp::ProgressHandler
{
public:
    WrapProgressHandler(const std::function<bool(float)> progressCallback):
         m_ProgressCallback(progressCallback)
    {}

    virtual bool Update(float percentage) override
    {
        // Invoke the progress callback
        if (m_ProgressCallback)
        {
            return m_ProgressCallback(percentage);
        }

        return true;
    }

private:
    std::function<bool(float)> m_ProgressCallback;
};

ResourceManager* g_ResourceManager;
ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
}


ResourceManager* ResourceManager::GetResourceManager()
{
    if (g_ResourceManager == nullptr) {
        g_ResourceManager = new ResourceManager();
    }
    return g_ResourceManager;
}

std::shared_ptr<ModelFileInfo>  ResourceManager::LoadModelFile(const std::wstring& fileName,
    const std::function<bool(float)>& loadingProgress)
{
    std::filesystem::path filePath = fileName;
    std::filesystem::path exportPath = std::filesystem::path(filePath).replace_extension("assbin");

    auto itr = LoadedModelFile.find(filePath.wstring());
    if (itr != LoadedModelFile.end()) {
        return itr->second;
    }
    std::filesystem::path parentPath;
    if (filePath.has_parent_path())
    {
        parentPath = filePath.parent_path();
    }
    else
    {
        parentPath = std::filesystem::current_path();
    }



    std::shared_ptr< Assimp::Importer> importer=std::make_shared<Assimp::Importer>();
    const aiScene* scene;

    importer->SetProgressHandler(new WrapProgressHandler(loadingProgress));

    // Check if a preprocessed file exists.
    if (std::filesystem::exists(exportPath) && std::filesystem::is_regular_file(exportPath))
    {
        scene = importer->ReadFile(exportPath.string(), aiProcess_GenBoundingBoxes);
    }
    else
    {
        // File has not been preprocessed yet. Import and processes the file.
        importer->SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
        importer->SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

        unsigned int preprocessFlags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph |
            aiProcess_ConvertToLeftHanded | aiProcess_GenBoundingBoxes;
        scene = importer->ReadFile(filePath.string(), preprocessFlags);

        //todo manage assbin
        //if (scene)
        //{
        //    // Export the preprocessed scene file for faster loading next time.
        //    Assimp::Exporter exporter;
        //    exporter.Export(scene, "assbin", exportPath.string(), 0);
        //}
    }
    LoadedModelFile.emplace(filePath.wstring(), std::make_shared<ModelFileInfo>(ModelFileInfo{scene, importer }));
    return LoadedModelFile.find(filePath.wstring())->second;
}

std::shared_ptr<TextureInfo> ResourceManager::LoadTextureFile(const std::wstring path, bool sRGB)
{

    TexMetadata  metadata;
    ScratchImage scratchImage;
    std::filesystem::path                 filePath(path);

    if (LoadedTextureFile.find(filePath.wstring())!= LoadedTextureFile.end()) {
        return LoadedTextureFile.find(filePath.wstring())->second;
    }

    if (filePath.extension() == ".dds")
    {
        ThrowIfFailed(LoadFromDDSFile(filePath.c_str(), DDS_FLAGS_FORCE_RGB, &metadata, scratchImage));
    }
    else if (filePath.extension() == ".hdr")
    {
        ThrowIfFailed(LoadFromHDRFile(filePath.c_str(), &metadata, scratchImage));
    }
    else if (filePath.extension() == ".tga")
    {
        ThrowIfFailed(LoadFromTGAFile(filePath.c_str(), &metadata, scratchImage));
    }
    else
    {
        ThrowIfFailed(LoadFromWICFile(filePath.c_str(), WIC_FLAGS_FORCE_RGB, &metadata, scratchImage));
    }

    // Force the texture format to be sRGB to convert to linear when sampling the texture in a shader.
    if (sRGB)
    {
        metadata.format = MakeSRGB(metadata.format);
    }
    LoadedTextureFile.emplace(filePath.wstring(), std::make_shared<TextureInfo>(TextureInfo{ std::move(metadata),std::move(scratchImage) }));
    return LoadedTextureFile.find(filePath.wstring())->second;
}

std::shared_ptr<MeshInfo> ResourceManager::MakeMesh(std::wstring name, std::vector<VertexAttributes> Vertices, std::vector<uint16_t> Indices)
{
    std::shared_ptr<MeshInfo> pMeshInfo;
    if (CachedMesh.find(name) != CachedMesh.end()) {
        pMeshInfo = CachedMesh.find(name)->second;
    }
    else {

        pMeshInfo = std::make_shared<MeshInfo>();
        CachedMesh.emplace(name, pMeshInfo);
    }
    pMeshInfo->Vertices = Vertices;
    pMeshInfo->Indices = Indices;
    return pMeshInfo;
}

std::shared_ptr<MeshInfo> ResourceManager::GetMesh(std::wstring name)
{
    if (CachedMesh.find(name) != CachedMesh.end()) {
        return CachedMesh.find(name)->second;
    }
    return std::shared_ptr<MeshInfo>();
}
