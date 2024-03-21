#pragma once
#include "PlatformHelper.h"
#include <exception>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // For HRESULT
#include <comdef.h> // For _com_error class (used to decode HR result codes).
// From DXSampleHelper.h 
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
void ThrowIfFailed(uint64_t res)
{
    HRESULT hr = res;
    if (FAILED(hr))
    {
        _com_error err(hr);
        OutputDebugString(err.ErrorMessage());

        throw std::exception(err.ErrorMessage());
    }
}