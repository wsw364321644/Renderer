#pragma once

/**
 *  @file DescriptorAllocator.h
 *  @date October 22, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief This is an allocator for CPU visible descriptors.
 *  CPU visible descriptors must be copied to a GPU visible descriptor heap before
 *  being used in a shader. The DynamicDescriptorHeap class is used to upload
 *  CPU visible descriptors to a GPU visible descriptor heap.
 *
 *  Variable sized memory allocation strategy based on:
 *  http://diligentgraphics.com/diligent-engine/architecture/d3d12/variable-size-memory-allocations-manager/
 *  Date Accessed: May 9, 2018
 */

#include "DescriptorAllocation.h"

#include "d3dx12.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

namespace dx12lib
{

    class DescriptorAllocatorPage;
class Device;

class DescriptorAllocator
{
public:
    /**
     * Allocate a number of contiguous descriptors from a CPU visible descriptor heap.
     *
     * @param numDescriptors The number of contiguous descriptors to allocate.
     * Cannot be more than the number of descriptors per descriptor heap.
     */
    dx12lib::DescriptorAllocation Allocate( uint32_t numDescriptors = 1 );

    /**
     * When the frame has completed, the stale descriptors can be released.
     */
    void ReleaseStaleDescriptors();

protected:
    friend class std::default_delete<DescriptorAllocator>;

    // Can only be created by the Device.
    DescriptorAllocator( Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptorsPerHeap = 256 );
    virtual ~DescriptorAllocator();

private:
    using DescriptorHeapPool = std::vector<std::shared_ptr<DescriptorAllocatorPage>>;

    // Create a new heap with a specific number of descriptors.
    std::shared_ptr<DescriptorAllocatorPage> CreateAllocatorPage();

    // The device that was use to create this DescriptorAllocator.
    Device&                    m_Device;
    D3D12_DESCRIPTOR_HEAP_TYPE m_HeapType;
    uint32_t                   m_NumDescriptorsPerHeap;

    DescriptorHeapPool m_HeapPool;
    // Indices of available heaps in the heap pool.
    std::set<size_t> m_AvailableHeaps;

    std::mutex m_AllocationMutex;
};
}  // namespace dx12lib