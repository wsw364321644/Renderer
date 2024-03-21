#pragma once

/**
 *  @file GUI.h
 *  @date October 24, 2018
 *  @author Jeremiah van Oosten
 *
 *  @brief Wrapper for ImGui. This class is used internally by the Window class.
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>  // For HWND

#include <memory>  // For std::shared_ptr
struct ImGuiContext;
namespace dx12lib
{

class CommandList;
class Device;
class PipelineStateObject;
class RenderTarget;
class RootSignature;
class ShaderResourceView;
class Texture;

class GUI
{
public:
    /**
     * Window message handler. This needs to be called by the application to allow ImGui to handle input messages.
     */
    LRESULT WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

    /**
     * Begin a new ImGui frame. Do this before calling any ImGui functions that modifies ImGui's render context.
     */
    void NewFrame(double delta);

    /**
     * Render ImgGui to the given render target.
     */
    void Render( const std::shared_ptr<CommandList>& commandList, const RenderTarget& renderTarget );

    /**
     * Destroy ImGui context.
     */
    void Destroy();

    /**
     * Set the font scaling for ImGui (this should be called when the window's DPI scaling changes.
     */
    void SetScaling( float scale );

protected:
    GUI( Device& device, HWND hWnd, const RenderTarget& renderTarget );
    virtual ~GUI();

private:

    Device&                              m_Device;
    HWND                                 m_hWnd;
    ImGuiContext*                        m_pImGuiCtx;

    std::shared_ptr<Texture>             m_FontTexture;
    std::shared_ptr<ShaderResourceView>  m_FontSRV;
    std::shared_ptr<RootSignature>       m_RootSignature;
    std::shared_ptr<PipelineStateObject> m_PipelineState;
};
}  // namespace dx12lib