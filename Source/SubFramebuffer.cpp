#include "SubFramebuffer.h"
#include "Buffer.h"
#include "misc.h"
#include"Graphics.h"
#include"Shader.h"

SubFramebuffer::SubFramebuffer(ID3D11Device* device, uint32_t width, uint32_t height) {
    HRESULT hr{ S_OK };

    // Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;


     //カラーバファ作成
    MakeBuffer(device, width, height, &renderTargetBuffer, &renderTargetView, &shaderResourceViews, DXGI_FORMAT_R16G16B16A16_FLOAT);
    
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;
    //DsvBuffer(device, width, height, &depth_stencil_buffer, &depth_stencil_view, &shaderResourceViews[1]);

    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

}
void SubFramebuffer::Clear(ID3D11DeviceContext* immediate_context,
    float r, float g, float b, float a, float depth)
{
    float color[4]{ r,g,b,a };
    immediate_context->ClearRenderTargetView(renderTargetView.Get(), color);
    
    //immediate_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH, depth, 0);

}
void SubFramebuffer::Activate(ID3D11DeviceContext* immediate_context)
{
    viewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    immediate_context->RSGetViewports(&viewportCount, cachedViewports);
    immediate_context->OMGetRenderTargets(1, cachedRenderTargetView.ReleaseAndGetAddressOf(),
        cachedDepthStencilView.ReleaseAndGetAddressOf());

    immediate_context->RSSetViewports(1, &viewport);
    ID3D11RenderTargetView* targets[] = {
        renderTargetView.Get()
    };
    // レンダーターゲットビュー設定
    immediate_context->OMSetRenderTargets(
        1, targets, depthStencilView.Get());
}
void SubFramebuffer::EffectActivate(ID3D11DeviceContext* immediate_context)
{

    // レンダーターゲットビュー設定
    immediate_context->OMSetRenderTargets(
        1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}
void SubFramebuffer::Deactivate(ID3D11DeviceContext* immediate_context)
{
    immediate_context->RSSetViewports(viewportCount, cachedViewports);
    immediate_context->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(),
        cachedDepthStencilView.Get());
}
