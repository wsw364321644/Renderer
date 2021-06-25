#pragma once

/**
 *  @file ConstantBufferView.h
 *  @date October 17, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief A wrapper for a Constant buffer view (CBV)
 */

#include <DescriptorAllocation.h>

#include <d3d12.h>  // For D3D12_CONSTANT_BUFFER_VIEW_DESC and D3D12_CPU_DESCRIPTOR_HANDLE
#include <memory> // For std::shared_ptr

namespace dx12lib
{

class ConstantBuffer;
class Device;

class ConstantBufferView
{
public:
    std::shared_ptr<ConstantBuffer> GetConstantBuffer() const
    {
        return m_ConstantBuffer;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle()
    {
        return m_Descriptor.GetDescriptorHandle();
    }

protected:
    ConstantBufferView( Device& device, const std::shared_ptr<ConstantBuffer>& constantBuffer,
                        size_t offset = 0 );
    virtual ~ConstantBufferView() = default;

private:
    Device&                         m_Device;
    std::shared_ptr<ConstantBuffer> m_ConstantBuffer;
    DescriptorAllocation            m_Descriptor;
};

}  // namespace dx12lib
