
#include "SImguiContent.h"
#include <imgui.h>
#include <imgui_impl_win32.h>

#include <map>

struct test {
    int a;
};
SImguiContent::SImguiContent()
{

    m_pImGuiCtx = ImGui::CreateContext();
    //ImGui::SetCurrentContext(m_pImGuiCtx);

    ////ImGui_ImplWin32_Init/////
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl";
    //io.ImeWindowHandle = hwnd;
    std::map< test, int> a;
   
    //todo
    //// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
    //io.KeyMap[ImGuiKey_Tab] = VK_TAB;
    //io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    //io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    //io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    //io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
    //io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
    //io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
    //io.KeyMap[ImGuiKey_Home] = VK_HOME;
    //io.KeyMap[ImGuiKey_End] = VK_END;
    //io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
    //io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    //io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    //io.KeyMap[ImGuiKey_Space] = VK_SPACE;
    //io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    //io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    //io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
    //io.KeyMap[ImGuiKey_A] = 'A';
    //io.KeyMap[ImGuiKey_C] = 'C';
    //io.KeyMap[ImGuiKey_V] = 'V';
    //io.KeyMap[ImGuiKey_X] = 'X';
    //io.KeyMap[ImGuiKey_Y] = 'Y';
    //io.KeyMap[ImGuiKey_Z] = 'Z';
    ////ImGui_ImplWin32_Init/////


    io.ConfigWindowsMoveFromTitleBarOnly = true;

    //todo
    //io.FontGlobalScale = ::GetDpiForWindow(m_hWnd) / 96.0f;

 
    // Allow user UI scaling using CTRL+Mouse Wheel scrolling
    io.FontAllowUserScaling = true;

    // Build texture atlas
    unsigned char* pixelData = nullptr;
    int            width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixelData, &width, &height);

    //auto& commandQueue = m_Device.GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
    //auto  commandList = commandQueue.GetCommandList();

    //auto fontTextureDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

    //m_FontTexture = m_Device.CreateTexture(fontTextureDesc);
    //m_FontTexture->SetName(L"ImGui Font Texture");
    //m_FontSRV = m_Device.CreateShaderResourceView(m_FontTexture);

    //size_t rowPitch, slicePitch;
    //GetSurfaceInfo(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, &slicePitch, &rowPitch, nullptr);

    //D3D12_SUBRESOURCE_DATA subresourceData;
    //subresourceData.pData = pixelData;
    //subresourceData.RowPitch = rowPitch;
    //subresourceData.SlicePitch = slicePitch;

    //commandList->CopyTextureSubresource(m_FontTexture, 0, 1, &subresourceData);
    //commandList->GenerateMips(m_FontTexture);

    //commandQueue.ExecuteCommandList(commandList);

    //auto d3d12Device = m_Device.GetD3D12Device();

    //// Create the root signature for the ImGUI shaders.

    //// Allow input layout and deny unnecessary access to certain pipeline stages.
    //D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
    //    D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
    //    D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
    //    D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

    //CD3DX12_DESCRIPTOR_RANGE1 descriptorRage(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //CD3DX12_ROOT_PARAMETER1 rootParameters[RootParameters::NumRootParameters];
    //rootParameters[RootParameters::MatrixCB].InitAsConstants(
    //    sizeof(DirectX::XMMATRIX) / 4, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
    //rootParameters[RootParameters::FontTexture].InitAsDescriptorTable(1, &descriptorRage,
    //    D3D12_SHADER_VISIBILITY_PIXEL);

    //CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT);
    //linearRepeatSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    //linearRepeatSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    //CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
    //rootSignatureDescription.Init_1_1(RootParameters::NumRootParameters, rootParameters, 1, &linearRepeatSampler,
    //    rootSignatureFlags);

    //m_RootSignature = m_Device.CreateRootSignature(rootSignatureDescription.Desc_1_1);

    //// clang-format off
    //const D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
    //    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    //    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, uv), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    //    { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(ImDrawVert, col), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    //};
    //// clang-format on

    //D3D12_BLEND_DESC blendDesc = {};
    //blendDesc.RenderTarget[0].BlendEnable = true;
    //blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    //blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    //blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    //blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
    //blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    //blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    //blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    //D3D12_RASTERIZER_DESC rasterizerDesc = {};
    //rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    //rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    //rasterizerDesc.FrontCounterClockwise = FALSE;
    //rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    //rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    //rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    //rasterizerDesc.DepthClipEnable = true;
    //rasterizerDesc.MultisampleEnable = FALSE;
    //rasterizerDesc.AntialiasedLineEnable = FALSE;
    //rasterizerDesc.ForcedSampleCount = 0;
    //rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    //D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    //depthStencilDesc.DepthEnable = false;
    //depthStencilDesc.StencilEnable = false;

    //ComPtr<ID3DBlob> vs;
    //ComPtr<ID3DBlob> ps;
    //ThrowIfFailed(D3DReadFileToBlob(L"data/shaders/ImGUI_VS.cso", &vs));
    //ThrowIfFailed(D3DReadFileToBlob(L"data/shaders/ImGUI_PS.cso", &ps));
    //// Setup the pipeline state.
    //struct PipelineStateStream
    //{
    //    CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE        pRootSignature;
    //    CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT          InputLayout;
    //    CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY    PrimitiveTopologyType;
    //    CD3DX12_PIPELINE_STATE_STREAM_VS                    VS;
    //    CD3DX12_PIPELINE_STATE_STREAM_PS                    PS;
    //    CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
    //    CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC           SampleDesc;
    //    CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC            BlendDesc;
    //    CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER            RasterizerState;
    //    CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL         DepthStencilState;
    //} pipelineStateStream;

    //pipelineStateStream.pRootSignature = m_RootSignature->GetD3D12RootSignature().Get();
    //pipelineStateStream.InputLayout = { inputLayout, 3 };
    //pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    //pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(vs.Get());
    //pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(ps.Get());
    //pipelineStateStream.RTVFormats = renderTarget.GetRenderTargetFormats();
    //pipelineStateStream.SampleDesc = renderTarget.GetSampleDesc();
    //pipelineStateStream.BlendDesc = CD3DX12_BLEND_DESC(blendDesc);
    //pipelineStateStream.RasterizerState = CD3DX12_RASTERIZER_DESC(rasterizerDesc);
    //pipelineStateStream.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(depthStencilDesc);

    //m_PipelineState = device.CreatePipelineStateObject(pipelineStateStream);
}

SImguiContent::~SImguiContent()
{
}
