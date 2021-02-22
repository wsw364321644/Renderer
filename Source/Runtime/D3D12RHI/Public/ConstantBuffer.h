#pragma once

/**
 *  @file ConstantBuffer.h
 *  @date October 22, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief Constant buffer resource.
 */

#include "Buffer.h"

#include <d3d12.h> // For ID3D12Resource
#include <wrl/client.h> // For ComPtr

namespace dx12lib
{
class ConstantBuffer : public Buffer
{
public:

    size_t GetSizeInBytes() const
    {
        return m_SizeInBytes;
    }

protected:
    ConstantBuffer( Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource );
    virtual ~ConstantBuffer();

private:
    size_t               m_SizeInBytes;
};
}  // namespace dx12lib