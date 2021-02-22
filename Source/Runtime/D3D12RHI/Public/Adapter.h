#pragma once

#pragma once

/**
 *  @file Adapter.h
 *  @date October 9, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief Wrapper for IDXGIAdapter
 */

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <memory>
#include <vector>

namespace dx12lib
{

class Adapter;
using AdapterList = std::vector<std::shared_ptr<Adapter>>;

class Adapter
{
public:
    /**
     * Get a list of DX12 compatible hardware adapters sorted by the GPU preference.
     * 
     * @param gpuPreference The GPU preference to sort the returned adapters.
     */
    static AdapterList GetAdapters( DXGI_GPU_PREFERENCE gpuPreference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE );

    /**
     * Create a GPU adapter.
     *
     * @param gpuPreference The GPU preference. By default, a high-performance
     * GPU is preferred.
     * @param useWarp If true, create a WARP adapter.
     *
     * @returns A shared pointer to a GPU adapter or nullptr if the adapter
     * could not be created.
     */
    static std::shared_ptr<Adapter> Create( DXGI_GPU_PREFERENCE gpuPreference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                                            bool                useWarp       = false );

    /**
     * Get the IDXGIAdapter
     */
    Microsoft::WRL::ComPtr<IDXGIAdapter4> GetDXGIAdapter() const
    {
        return m_dxgiAdapter;
    }

    /**
     * Get the description of the adapter.
     */
    const std::wstring GetDescription() const;

protected:
    Adapter( Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgiAdapter );
    virtual ~Adapter() = default;

private:
    Microsoft::WRL::ComPtr<IDXGIAdapter4> m_dxgiAdapter;
    DXGI_ADAPTER_DESC3                    m_Desc;
};
}  // namespace dx12lib
