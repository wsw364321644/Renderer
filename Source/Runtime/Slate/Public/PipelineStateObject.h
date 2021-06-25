#pragma once

/**
 *  @file PipelineStateObject.h
 *  @date October 18, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief Wrapper for a ID3D12PipelineState object.
 */

#include <d3d12.h>       // For D3D12_PIPELINE_STATE_STREAM_DESC, and ID3D12PipelineState
#include <wrl/client.h>  // For Microsoft::WRL::ComPtr

namespace dx12lib
{

class Device;

class PipelineStateObject
{
public:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> GetD3D12PipelineState() const
    {
        return m_d3d12PipelineState;
    }

protected:
    PipelineStateObject( Device& device, const D3D12_PIPELINE_STATE_STREAM_DESC& desc );
    virtual ~PipelineStateObject() = default;

private:
    Device&                                     m_Device;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_d3d12PipelineState;
};
}  // namespace dx12lib
