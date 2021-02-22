#pragma once

/**
 *  @file Buffer.h
 *  @date October 22, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief Abstract base class for buffer resources.
 */

#include "D3D12Resource.h"

namespace dx12lib
{

//class Device;

class Buffer : public Resource
{
public:
protected:
    Buffer( Device& device, const D3D12_RESOURCE_DESC& resDesc );
    Buffer( Device& device, Microsoft::WRL::ComPtr<ID3D12Resource> resource );
};
}  // namespace dx12lib