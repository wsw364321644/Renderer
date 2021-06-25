#pragma once

/**
 *  @file UnorderedAccessView.h
 *  @date October 17, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief Wrapper for an Unordered Access View (UAV)
 */

#include <DescriptorAllocation.h>

#include <d3d12.h>  // For D3D12_UNORDERED_ACCESS_VIEW_DESC and D3D12_CPU_DESCRIPTOR_HANDLE
#include <memory>   // For std::shared_ptr

namespace dx12lib
{

class Device;
class Resource;

class UnorderedAccessView
{
public:
    std::shared_ptr<Resource> GetResource() const
    {
        return m_Resource;
    }

    std::shared_ptr<Resource> GetCounterResource() const
    {
        return m_CounterResource;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const
    {
        return m_Descriptor.GetDescriptorHandle();
    }

protected:
    UnorderedAccessView( Device& device, const std::shared_ptr<Resource>& resource,
                         const std::shared_ptr<Resource>&        counterResource = nullptr,
                         const D3D12_UNORDERED_ACCESS_VIEW_DESC* uav             = nullptr );
    virtual ~UnorderedAccessView() = default;

private:
    Device&                   m_Device;
    std::shared_ptr<Resource> m_Resource;
    std::shared_ptr<Resource> m_CounterResource;
    DescriptorAllocation      m_Descriptor;
};
}  // namespace dx12lib
