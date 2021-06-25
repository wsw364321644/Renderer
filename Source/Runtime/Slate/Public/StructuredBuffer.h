#pragma once

/**
 *  @file StructuredBuffer.h
 *  @date October 24, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief Structured buffer resource.
 */

#include "Buffer.h"

#include "ByteAddressBuffer.h"

namespace dx12lib
{

class Device;

class StructuredBuffer : public Buffer
{

public:
    /**
     * Get the number of elements contained in this buffer.
     */
    virtual size_t GetNumElements() const
    {
        return m_NumElements;
    }

    /**
     * Get the size in bytes of each element in this buffer.
     */
    virtual size_t GetElementSize() const
    {
        return m_ElementSize;
    }

    std::shared_ptr<ByteAddressBuffer> GetCounterBuffer() const
    {
        return m_CounterBuffer;
    }

protected:
    StructuredBuffer( Device& device, size_t numElements,
                      size_t elementSize );
    StructuredBuffer( Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource,
                      size_t numElements, size_t elementSize );

    virtual ~StructuredBuffer() = default;

private:
    size_t m_NumElements;
    size_t m_ElementSize;

    // A buffer to store the internal counter for the structured buffer.
    std::shared_ptr<ByteAddressBuffer> m_CounterBuffer;
};
}  // namespace dx12lib