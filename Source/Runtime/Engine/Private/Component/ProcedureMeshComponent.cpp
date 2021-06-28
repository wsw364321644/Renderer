
#include "Component/ProcedureMeshComponent.h"
#include <DirectXMath.h>
#include "VertexTypes.h"
#include "CommandList.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Material.h"
#include "Mesh.h"
#include "Misc/MathHelper.h"
using namespace DirectX;
ProcedureMeshComponent::ProcedureMeshComponent() {

}

ProcedureMeshComponent::~ProcedureMeshComponent() {

}

void ProcedureMeshComponent::CreateCube(float size, bool reverseWinding)
{
    // Cube is centered at 0,0,0.
    float s = size * 0.5f;

    // 8 edges of cube.
    DirectX::XMFLOAT3 p[8] = { { s, s, -s }, { s, s, s },   { s, -s, s },   { s, -s, -s },
                      { -s, s, s }, { -s, s, -s }, { -s, -s, -s }, { -s, -s, s } };
    // 6 face normals
    DirectX::XMFLOAT3 n[6] = { { 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 }, { 0, 0, 1 }, { 0, 0, -1 } };
    // 4 unique texture coordinates
    DirectX::XMFLOAT3 t[4] = { { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 } };

    // Indices for the vertex positions.
    uint16_t i[24] = {
        0, 1, 2, 3,  // +X
        4, 5, 6, 7,  // -X
        4, 1, 0, 5,  // +Y
        2, 7, 6, 3,  // -Y
        1, 4, 7, 2,  // +Z
        5, 0, 3, 6   // -Z
    };

    std::vector<VertexAttributes> Vertices;
    std::vector<uint16_t> Indices;

    for (uint16_t f = 0; f < 6; ++f)  // For each face of the cube.
    {
        // Four Vertices per face.
        Vertices.emplace_back(p[i[f * 4 + 0]], n[f], t[0]);
        Vertices.emplace_back(p[i[f * 4 + 1]], n[f], t[1]);
        Vertices.emplace_back(p[i[f * 4 + 2]], n[f], t[2]);
        Vertices.emplace_back(p[i[f * 4 + 3]], n[f], t[3]);

        // First triangle.
        Indices.emplace_back(f * 4 + 0);
        Indices.emplace_back(f * 4 + 1);
        Indices.emplace_back(f * 4 + 2);

        // Second triangle
        Indices.emplace_back(f * 4 + 2);
        Indices.emplace_back(f * 4 + 3);
        Indices.emplace_back(f * 4 + 0);
    }

    if (reverseWinding)
    {
        ReverseWinding(Indices, Vertices);
    }

    Mesh=ResourceManager::GetResourceManager()->MakeMesh(L"Cube",Vertices, Indices);
}


void ProcedureMeshComponent::CreateSphere(float radius, uint32_t tessellation, bool reversWinding)
{

    if (tessellation < 3)
        throw std::out_of_range("tessellation parameter out of range");

    size_t verticalSegments = tessellation;
    size_t horizontalSegments = tessellation * 2;

    std::vector<VertexAttributes> Vertices;
    std::vector<uint16_t> Indices;
    // Create rings of Vertices at progressively higher latitudes.
    for (size_t i = 0; i <= verticalSegments; i++)
    {
        float v = 1 - (float)i / verticalSegments;

        float latitude = (i * DirectX::XM_PI / verticalSegments) - DirectX::XM_PIDIV2;
        float dy, dxz;

        DirectX::XMScalarSinCos(&dy, &dxz, latitude);

        // Create a single ring of Vertices at this latitude.
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            float u = (float)j / horizontalSegments;

            float longitude = j * DirectX::XM_2PI / horizontalSegments;
            float dx, dz;

            DirectX::XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            auto normal = DirectX::XMVectorSet(dx, dy, dz, 0);
            auto textureCoordinate = DirectX::XMVectorSet(u, v, 0, 0);
            auto position = normal * radius;

            Vertices.emplace_back(position, normal, textureCoordinate);
        }
    }

    // Fill the index buffer with triangles joining each pair of latitude rings.
    size_t stride = horizontalSegments + 1;

    for (size_t i = 0; i < verticalSegments; i++)
    {
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            size_t nextI = i + 1;
            size_t nextJ = (j + 1) % stride;

            Indices.push_back(i * stride + nextJ);
            Indices.push_back(nextI * stride + j);
            Indices.push_back(i * stride + j);

            Indices.push_back(nextI * stride + nextJ);
            Indices.push_back(nextI * stride + j);
            Indices.push_back(i * stride + nextJ);
        }
    }

    if (reversWinding)
    {
        ReverseWinding(Indices, Vertices);
    }
    Mesh= ResourceManager::GetResourceManager()->MakeMesh(L"Sphere", Vertices, Indices);
}


void ProcedureMeshComponent::CreateCylinder(float radius, float height, uint32_t tessellation,
    bool reverseWinding)
{
    if (tessellation < 3)
        throw std::out_of_range("tessellation parameter out of range");

    height /= 2;

    DirectX::XMVECTOR topOffset = DirectX::XMVectorScale(DirectX::g_XMIdentityR1, height);

    size_t stride = tessellation + 1;

    std::vector<VertexAttributes> Vertices;
    std::vector<uint16_t> Indices;

    // Create a ring of triangles around the outside of the cylinder.
    for (size_t i = 0; i <= tessellation; i++)
    {
        DirectX::XMVECTOR normal = MathHelper::GetCircleVector(i, tessellation);

        DirectX::XMVECTOR sideOffset = DirectX::XMVectorScale(normal, radius);

        float u = float(i) / float(tessellation);

        DirectX::XMVECTOR textureCoordinate = DirectX::XMLoadFloat(&u);

        Vertices.emplace_back(DirectX::XMVectorAdd(sideOffset, topOffset), normal, textureCoordinate);
        Vertices.emplace_back(DirectX::XMVectorSubtract(sideOffset, topOffset), normal,
            DirectX::XMVectorAdd(textureCoordinate, DirectX::g_XMIdentityR1));

        Indices.push_back(i * 2 + 1);
        Indices.push_back((i * 2 + 2) % (stride * 2));
        Indices.push_back(i * 2);

        Indices.push_back((i * 2 + 3) % (stride * 2));
        Indices.push_back((i * 2 + 2) % (stride * 2));
        Indices.push_back(i * 2 + 1);
    }

    // Create flat triangle fan caps to seal the top and bottom.
    CreateCylinderCap(Vertices, Indices, tessellation, height, radius, true);
    CreateCylinderCap(Vertices, Indices, tessellation, height, radius, false);

    // Build RH above
    if (reverseWinding)
    {
        ReverseWinding(Indices, Vertices);
    }
    Mesh=ResourceManager::GetResourceManager()->MakeMesh(L"Cylinder", Vertices, Indices);
}

void ProcedureMeshComponent::CreateCone(float radius, float height, uint32_t tessellation, bool reverseWinding)
{
    if (tessellation < 3)
        throw std::out_of_range("tessellation parameter out of range");

    height /= 2;

    DirectX::XMVECTOR topOffset = DirectX::XMVectorScale(DirectX::g_XMIdentityR1, height);

    size_t stride = tessellation + 1;

    std::vector<VertexAttributes> Vertices;
    std::vector<uint16_t> Indices;

    // Create a ring of triangles around the outside of the cone.
    for (size_t i = 0; i <= tessellation; i++)
    {
        DirectX::XMVECTOR circlevec = MathHelper::GetCircleVector(i, tessellation);

        DirectX::XMVECTOR sideOffset = DirectX::XMVectorScale(circlevec, radius);

        float u = float(i) / float(tessellation);

        DirectX::XMVECTOR textureCoordinate = DirectX::XMLoadFloat(&u);

        DirectX::XMVECTOR pt = DirectX::XMVectorSubtract(sideOffset, topOffset);

        DirectX::XMVECTOR normal = DirectX::XMVector3Cross(MathHelper::GetCircleTangent(i, tessellation), DirectX::XMVectorSubtract(topOffset, pt));
        normal = DirectX::XMVector3Normalize(normal);

        // Duplicate the top vertex for distinct normals
        Vertices.emplace_back(topOffset, normal, DirectX::g_XMZero);
        Vertices.emplace_back(pt, normal, DirectX::XMVectorAdd(textureCoordinate, DirectX::g_XMIdentityR1));

        Indices.push_back((i * 2 + 1) % (stride * 2));
        Indices.push_back((i * 2 + 3) % (stride * 2));
        Indices.push_back(i * 2);
    }

    // Create flat triangle fan caps to seal the bottom.
    CreateCylinderCap(Vertices, Indices, tessellation, height, radius, false);

    // Build RH above
    if (reverseWinding)
    {
        ReverseWinding(Indices, Vertices);
    }
    Mesh = ResourceManager::GetResourceManager()->MakeMesh(L"Cone", Vertices, Indices);
}

void ProcedureMeshComponent::CreateTorus(float radius, float thickness, uint32_t tessellation,
    bool reverseWinding)
{
    assert(tessellation > 3);

    std::vector<VertexAttributes> Vertices;
    std::vector<uint16_t> Indices;

    size_t stride = tessellation + 1;

    // First we loop around the main ring of the torus.
    for (size_t i = 0; i <= tessellation; i++)
    {
        float u = (float)i / tessellation;

        float outerAngle = i * DirectX::XM_2PI / tessellation - DirectX::XM_PIDIV2;

        // Create a transform matrix that will align geometry to
        // slice perpendicularly though the current ring position.
        DirectX::XMMATRIX transform = DirectX::XMMatrixTranslation(radius, 0, 0) * DirectX::XMMatrixRotationY(outerAngle);

        // Now we loop along the other axis, around the side of the tube.
        for (size_t j = 0; j <= tessellation; j++)
        {
            float v = 1 - (float)j / tessellation;

            float innerAngle = j * DirectX::XM_2PI / tessellation + DirectX::XM_PI;
            float dx, dy;

            DirectX::XMScalarSinCos(&dy, &dx, innerAngle);

            // Create a vertex.
            auto normal = DirectX::XMVectorSet(dx, dy, 0, 0);
            auto position = normal * thickness / 2;
            auto textureCoordinate = DirectX::XMVectorSet(u, v, 0, 0);

            position = DirectX::XMVector3Transform(position, transform);
            normal = DirectX::XMVector3TransformNormal(normal, transform);

            Vertices.emplace_back(position, normal, textureCoordinate);

            // And create Indices for two triangles.
            size_t nextI = (i + 1) % stride;
            size_t nextJ = (j + 1) % stride;

            Indices.push_back(nextI * stride + j);
            Indices.push_back(i * stride + nextJ);
            Indices.push_back(i * stride + j);

            Indices.push_back(nextI * stride + j);
            Indices.push_back(nextI * stride + nextJ);
            Indices.push_back(i * stride + nextJ);
        }
    }

    if (reverseWinding)
    {
        ReverseWinding(Indices, Vertices);
    }

    Mesh = ResourceManager::GetResourceManager()->MakeMesh(L"Torus", Vertices, Indices);
}

void ProcedureMeshComponent::CreatePlane(float width, float height, bool reverseWinding)
{

    std::vector<VertexAttributes> Vertices;
    std::vector<uint16_t> Indices;
    // clang-format off
    // Define a plane that is aligned with the X-Z plane and the normal is facing up in the Y-axis.
    Vertices = {
        VertexAttributes(DirectX::XMFLOAT3(-0.5f * width, 0.0f, 0.5f * height), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),  // 0
        VertexAttributes(DirectX::XMFLOAT3(0.5f * width, 0.0f, 0.5f * height), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),   // 1
        VertexAttributes(DirectX::XMFLOAT3(0.5f * width, 0.0f, -0.5f * height), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)),  // 2
        VertexAttributes(DirectX::XMFLOAT3(-0.5f * width, 0.0f, -0.5f * height), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f))  // 3
    };
    // clang-format on
    Indices = { 1, 3, 0, 2, 3, 1 };

    if (reverseWinding)
    {
        ReverseWinding(Indices, Vertices);
    }
    Mesh = ResourceManager::GetResourceManager()->MakeMesh(L"Plane", Vertices, Indices);
}

std::shared_ptr<dx12lib::Scene> ProcedureMeshComponent::UploadResource(std::shared_ptr<dx12lib::CommandList> commandlist)
{
    
    if (Mesh->Vertices.empty())
    {
        return nullptr;
    }

    auto vertexBuffer = commandlist->CopyVertexBuffer(Mesh->Vertices);
    auto indexBuffer = commandlist->CopyIndexBuffer(Mesh->Indices);

    auto mesh = std::make_shared<dx12lib::Mesh>();
    // Create a default white material for new meshes.
    auto material = std::make_shared<dx12lib::Material>(dx12lib::Material::White);

    mesh->SetVertexBuffer(0, vertexBuffer);
    mesh->SetIndexBuffer(indexBuffer);
    mesh->SetMaterial(material);

    auto node = std::make_shared<dx12lib::SceneNode>();
    node->AddMesh(mesh);

    Scene = std::make_shared<dx12lib::Scene>();
    Scene->SetRootNode(node);

    return Scene;
}
