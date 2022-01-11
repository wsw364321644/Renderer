#pragma once

#include "Camera.h"
#include "Light.h"
#include"World.h"
#include <GenericPlatform/GameFramework.h>

#include <RenderTarget.h>

#include <DirectXMath.h>

#include <string>

namespace dx12lib
{
class CommandList;
class Device;
class GUI;
class Mesh;
class PipelineStateObject;
class RootSignature;
class Scene;
class ShaderResourceView;
class SwapChain;
class Texture;

}  // namespace dx12lib

class GenericWindow;  // From GameFramework.

class Editor
{
public:
    Editor( const std::string& name, int width, int height, bool vSync = false );
    virtual ~Editor();

    /**
     * Start the main game loop.
     */
    uint32_t Run();

    /**
     *  Load content required for the demo.
     */
    bool LoadContent();

    /**
     *  Unload demo specific content that was loaded in LoadContent.
     */
    void UnloadContent();

protected:
    /**
     *  Update the game logic.
     */
    void OnUpdate( UpdateEventArgs& e );

    /**
     *  Render stuff.
     */
    void OnRender();

    /**
     * Invoked by the registered window when a key is pressed
     * while the window has focus.
     */
    void OnKeyPressed( KeyEventArgs& e );

    /**
     * Invoked when a key on the keyboard is released.
     */
    void OnKeyReleased( KeyEventArgs& e );

    /**
     * Invoked when the mouse is moved over the registered window.
     */
    void OnMouseMoved( MouseMotionEventArgs& e );

    /**
     * Invoked when the mouse wheel is scrolled while the registered window has focus.
     */
    void OnMouseWheel( MouseWheelEventArgs& e );

    void RescaleHDRRenderTarget( float scale );
    void OnResize( ResizeEventArgs& e );

    void OnDPIScaleChanged( DPIScaleEventArgs& e );

    void OnGUI( const std::shared_ptr<dx12lib::CommandList>& commandList, const dx12lib::RenderTarget& renderTarget, std::shared_ptr<dx12lib::Texture> tex );

private:
    std::shared_ptr<dx12lib::Device>    m_Device;
    std::shared_ptr<dx12lib::SwapChain> m_SwapChain;
    std::shared_ptr<dx12lib::GUI>       m_GUI;

    std::shared_ptr<SWindow> m_Window;


    std::shared_ptr<World>       m_World;
    // Some geometry to render.
    std::shared_ptr<dx12lib::Scene> m_Cube;
    std::shared_ptr<dx12lib::Scene> m_Sphere;
    std::shared_ptr<dx12lib::Scene> m_Cone;
    std::shared_ptr<dx12lib::Scene> m_Cylinder;
    std::shared_ptr<dx12lib::Scene> m_Torus;
    std::shared_ptr<dx12lib::Scene> m_Plane;

    std::shared_ptr<dx12lib::Scene> m_Skybox;

    std::shared_ptr<dx12lib::Texture> m_DefaultTexture;
    std::shared_ptr<dx12lib::Texture> m_DirectXTexture;
    std::shared_ptr<dx12lib::Texture> m_EarthTexture;
    std::shared_ptr<dx12lib::Texture> m_MonaLisaTexture;
    std::shared_ptr<dx12lib::Texture> m_GraceCathedralTexture;
    std::shared_ptr<dx12lib::Texture> m_GraceCathedralCubemap;
    std::shared_ptr<dx12lib::ShaderResourceView> m_GraceCathedralCubemapSRV;

    // HDR Render target
    dx12lib::RenderTarget m_HDRRenderTarget;
    std::shared_ptr<dx12lib::Texture> m_HDRTexture;

    dx12lib::RenderTarget m_IntermediateRenderTarget;
    std::shared_ptr<dx12lib::Texture> m_IntermediateTexture;

    // Root signatures
    std::shared_ptr<dx12lib::RootSignature> m_SkyboxSignature;
    std::shared_ptr<dx12lib::RootSignature> m_HDRRootSignature;
    std::shared_ptr<dx12lib::RootSignature> m_SDRRootSignature;

    // Pipeline state object.
    // Skybox PSO
    std::shared_ptr<dx12lib::PipelineStateObject> m_SkyboxPipelineState;
    std::shared_ptr<dx12lib::PipelineStateObject> m_HDRPipelineState;
    // HDR -> SDR tone mapping PSO.
    std::shared_ptr<dx12lib::PipelineStateObject> m_SDRPipelineState;
    // Unlit pixel shader (for rendering the light sources)
    std::shared_ptr<dx12lib::PipelineStateObject> m_UnlitPipelineState;

    D3D12_RECT m_ScissorRect;

    Camera m_Camera;
    struct alignas( 16 ) CameraData
    {
        DirectX::XMVECTOR m_InitialCamPos;
        DirectX::XMVECTOR m_InitialCamRot;
        float             m_InitialFov;
    };
    CameraData* m_pAlignedCameraData;

    // Camera controller
    float m_Forward;
    float m_Backward;
    float m_Left;
    float m_Right;
    float m_Up;
    float m_Down;

    float m_Pitch;
    float m_Yaw;

    // Rotate the lights in a circle.
    bool m_AnimateLights;
    // Set to true if the Shift key is pressed.
    bool m_Shift;

    int  m_Width;
    int  m_Height;
    bool m_VSync;
    bool m_Fullscreen;

    // Scale the HDR render target to a fraction of the window size.
    float m_RenderScale;

    bool m_SceneSelected;
    // Define some lights.
    std::vector<PointLight> m_PointLights;
    std::vector<SpotLight>  m_SpotLights;

    Logger m_Logger;
};