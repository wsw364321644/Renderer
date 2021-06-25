
#include "Component/PrimitiveComponent.h"
#include <DirectXMath.h>

using namespace DirectX;
PrimitiveComponent::PrimitiveComponent() {

}

PrimitiveComponent::~PrimitiveComponent() {

}


void PrimitiveComponent::CreateCylinderCap(std::vector<VertexAttributes>& Vertices, std::vector<uint16_t>& Indices, size_t tessellation,
    float height, float radius, bool isTop)
{
    // Create cap Indices.
    for (size_t i = 0; i < tessellation - 2; i++)
    {
        size_t i1 = (i + 1) % tessellation;
        size_t i2 = (i + 2) % tessellation;

        if (isTop)
        {
            std::swap(i1, i2);
        }

        size_t vbase = Vertices.size();
        Indices.push_back(vbase + i2);
        Indices.push_back(vbase + i1);
        Indices.push_back(vbase);
    }

    // Which end of the cylinder is this?
    DirectX::XMVECTOR normal = DirectX::g_XMIdentityR1;
    DirectX::XMVECTOR textureScale = DirectX::g_XMNegativeOneHalf;

    if (!isTop)
    {
        normal = DirectX::XMVectorNegate(normal);
        textureScale = DirectX::XMVectorMultiply(textureScale, DirectX::g_XMNegateX);
    }

    // Create cap Vertices.
    for (size_t i = 0; i < tessellation; i++)
    {
        DirectX::XMVECTOR circleVector = MathHelper::GetCircleVector(i, tessellation);
        DirectX::XMVECTOR position = DirectX::XMVectorAdd(DirectX::XMVectorScale(circleVector, radius), DirectX::XMVectorScale(normal, height));
        DirectX::XMVECTOR textureCoordinate =
            DirectX::XMVectorMultiplyAdd(DirectX::XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, DirectX::g_XMOneHalf);

        Vertices.emplace_back(position, normal, textureCoordinate);
    }
}

