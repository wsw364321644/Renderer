#pragma once

/**
 *  @file GenerateMipsPSO.h
 *  @date October 24, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief Pipeline state object for generating mip maps.
 */

#include "DescriptorAllocation.h"

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

namespace dx12lib
{

class Device;
class PipelineStateObject;
class RootSignature;

struct alignas( 16 ) GenerateMipsCB
{
    uint32_t          SrcMipLevel;   // Texture level of source mip
    uint32_t          NumMipLevels;  // Number of OutMips to write: [1-4]
    uint32_t          SrcDimension;  // Width and height of the source texture are even or odd.
    uint32_t          IsSRGB;        // Must apply gamma correction to sRGB textures.
    DirectX::XMFLOAT2 TexelSize;     // 1.0 / OutMip1.Dimensions
};

// I don't use scoped enums (C++11) to avoid the explicit cast that is required to
// treat these as root indices.
namespace GenerateMips
{
enum
{
    GenerateMipsCB,
    SrcMip,
    OutMip,
    NumRootParameters
};
}

class GenerateMipsPSO
{
public:
    GenerateMipsPSO( Device& device );

    std::shared_ptr<RootSignature> GetRootSignature() const
    {
        return m_RootSignature;
    }

    std::shared_ptr<PipelineStateObject> GetPipelineState() const
    {
        return m_PipelineState;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE GetDefaultUAV() const
    {
        return m_DefaultUAV.GetDescriptorHandle();
    }

private:
    std::shared_ptr<RootSignature>       m_RootSignature;
    std::shared_ptr<PipelineStateObject> m_PipelineState;
    // Default (no resource) UAV's to pad the unused UAV descriptors.
    // If generating less than 4 mip map levels, the unused mip maps
    // need to be padded with default UAVs (to keep the DX12 runtime happy).
    DescriptorAllocation m_DefaultUAV;
};
}  // namespace dx12lib