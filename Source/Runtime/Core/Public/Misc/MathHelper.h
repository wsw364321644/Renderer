#pragma once

#include <DirectXMath.h>
namespace MathHelper {
	// Helper function to compute a point on a unit circle aligned to the x,z plane and centered at the origin.
	inline DirectX::XMVECTOR GetCircleVector(size_t i, size_t tessellation) noexcept {
		float angle = float(i) * DirectX::XM_2PI / float(tessellation);
		float dx, dz;

		DirectX::XMScalarSinCos(&dx, &dz, angle);

		DirectX::XMVECTORF32 v = { { { dx, 0, dz, 0 } } };
		return v;
	}
	// Helper function to compute a tangent vector at a point on a unit sphere aligned to the x,z plane.
	inline DirectX::XMVECTOR GetCircleTangent(size_t i, size_t tessellation) noexcept {
		float angle = (float(i) * DirectX::XM_2PI / float(tessellation)) + DirectX::XM_PIDIV2;
		float dx, dz;

		DirectX::XMScalarSinCos(&dx, &dz, angle);

		DirectX::XMVECTORF32 v = { { { dx, 0, dz, 0 } } };
		return v;
	}
}


struct VertexAttributes
{
    VertexAttributes() = default;

    explicit VertexAttributes(const DirectX::XMFLOAT3& position,
        const DirectX::XMFLOAT3& normal,
        const DirectX::XMFLOAT3& texCoord,
        const DirectX::XMFLOAT3& tangent = { 0, 0, 0 },
        const DirectX::XMFLOAT3& bitangent = { 0, 0, 0 })
        : Position(position)
        , Normal(normal)
        , Tangent(tangent)
        , Bitangent(bitangent)
        , TexCoord(texCoord)
    {}

    explicit VertexAttributes(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal,
        DirectX::FXMVECTOR texCoord,
        DirectX::GXMVECTOR tangent = { 0, 0, 0, 0 },
        DirectX::HXMVECTOR bitangent = { 0, 0, 0, 0 })
    {
        DirectX::XMStoreFloat3(&(this->Position), position);
        DirectX::XMStoreFloat3(&(this->Normal), normal);
        DirectX::XMStoreFloat3(&(this->Tangent), tangent);
        DirectX::XMStoreFloat3(&(this->Bitangent), bitangent);
        DirectX::XMStoreFloat3(&(this->TexCoord), texCoord);
    }

    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT3 Tangent;
    DirectX::XMFLOAT3 Bitangent;
    DirectX::XMFLOAT3 TexCoord;

};