#pragma once

/**
 *  @file ByteAddressBuffer.h
 *  @date October 22, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief ByteAddressBuffer.
 *  @see https://msdn.microsoft.com/en-us/library/ff471453(v=vs.85).aspx
 */

#include "Buffer.h"
#include "DescriptorAllocation.h"

#include <d3dx12.h>

namespace dx12lib
{

class Device;

class ByteAddressBuffer : public Buffer
{
public:
    size_t GetBufferSize() const
    {
        return m_BufferSize;
    }

protected:
    ByteAddressBuffer( Device& device, const D3D12_RESOURCE_DESC& resDesc );
    ByteAddressBuffer( Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource );
    virtual ~ByteAddressBuffer() = default;

private:
    size_t m_BufferSize;
};
}  // namespace dx12lib