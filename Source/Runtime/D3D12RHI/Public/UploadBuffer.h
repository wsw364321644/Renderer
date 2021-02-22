#pragma once

/**
 *  @file UploadBuffer.h
 *  @date October 24, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief An UploadBuffer provides a convenient method to upload resources to the GPU.
 */

#include "Defines.h"

#include <d3d12.h>
#include <wrl.h>

#include <deque>
#include <memory>

namespace dx12lib
{

class Device;

class UploadBuffer
{
public:
    // Use to upload data to the GPU
    struct Allocation
    {
        void*                     CPU;
        D3D12_GPU_VIRTUAL_ADDRESS GPU;
    };

    /**
     * The maximum size of an allocation is the size of a single page.
     */
    size_t GetPageSize() const
    {
        return m_PageSize;
    }

    /**
     * Allocate memory in an Upload heap.
     * An allocation must not exceed the size of a page.
     * Use a memcpy or similar method to copy the
     * buffer data to CPU pointer in the Allocation structure returned from
     * this function.
     */
    Allocation Allocate( size_t sizeInBytes, size_t alignment );

    /**
     * Release all allocated pages. This should only be done when the command list
     * is finished executing on the CommandQueue.
     */
    void Reset();

protected:
    friend class std::default_delete<UploadBuffer>;

    /**
     * @param pageSize The size to use to allocate new pages in GPU memory.
     */
    explicit UploadBuffer( Device& device, size_t pageSize = _2MB );
    virtual ~UploadBuffer();

private:
    // A single page for the allocator.
    struct Page
    {
        Page( Device& device, size_t sizeInBytes );
        ~Page();

        // Check to see if the page has room to satisfy the requested
        // allocation.
        bool HasSpace( size_t sizeInBytes, size_t alignment ) const;

        // Allocate memory from the page.
        // Throws std::bad_alloc if the the allocation size is larger
        // that the page size or the size of the allocation exceeds the
        // remaining space in the page.
        Allocation Allocate( size_t sizeInBytes, size_t alignment );

        // Reset the page for reuse.
        void Reset();

    private:
        Device&                                m_Device;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_d3d12Resource;

        // Base pointer.
        void*                     m_CPUPtr;
        D3D12_GPU_VIRTUAL_ADDRESS m_GPUPtr;

        // Allocated page size.
        size_t m_PageSize;
        // Current allocation offset in bytes.
        size_t m_Offset;
    };

    // A pool of memory pages.
    using PagePool = std::deque<std::shared_ptr<Page>>;

    // The device that was used to create this upload buffer.
    Device& m_Device;

    // Request a page from the pool of available pages
    // or create a new page if there are no available pages.
    std::shared_ptr<Page> RequestPage();

    PagePool m_PagePool;
    PagePool m_AvailablePages;

    std::shared_ptr<Page> m_CurrentPage;

    // The size of each page of memory.
    size_t m_PageSize;
};
}  // namespace dx12lib