#include "Framebuffer.h"
#include "Buffer.h"
#include "misc.h"
#include"Graphics.h"
#include"Shader.h"

Framebuffer::Framebuffer(ID3D11Device* device, uint32_t width, uint32_t height) 
{
    //カラーバファ作成
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Color],      &renderTargetView[(int)Buffer::Color],     &shaderResourceViews[(int)Buffer::Color], DXGI_FORMAT_R16G16B16A16_FLOAT);
    //デプスバッファ作成                                                                                                       
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Depth],      &renderTargetView[(int)Buffer::Depth],     &shaderResourceViews[(int)Buffer::Depth], DXGI_FORMAT_R32_FLOAT);
    //法線バッファ作成                                                                                                         
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Nomal],      &renderTargetView[(int)Buffer::Nomal],     &shaderResourceViews[(int)Buffer::Nomal], DXGI_FORMAT_R8G8B8A8_UNORM);
    //3D座標バッファ作成                                                                                                       
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Pos],        &renderTargetView[(int)Buffer::Pos],       &shaderResourceViews[(int)Buffer::Pos], DXGI_FORMAT_R32G32B32A32_FLOAT);
    // メタリックバッファ作成                                                                                                       
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::MetalSmoothness], &renderTargetView[(int)Buffer::MetalSmoothness], &shaderResourceViews[(int)Buffer::MetalSmoothness], DXGI_FORMAT_R32_FLOAT);
    // 粗さバッファ作成                                                                                                       
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::AmbientOcclusion], &renderTargetView[(int)Buffer::AmbientOcclusion], &shaderResourceViews[(int)Buffer::AmbientOcclusion], DXGI_FORMAT_R32_FLOAT);
    // エミッシブバッファ作成                                                                                                       
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Emission], &renderTargetView[(int)Buffer::Emission], &shaderResourceViews[(int)Buffer::Emission], DXGI_FORMAT_R32G32B32A32_FLOAT);
    //ライトバッファ生成                                                                                                       
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Light],     &renderTargetView[(int)Buffer::Light],     &shaderResourceViews[(int)Buffer::Light], DXGI_FORMAT_R16G16B16A16_FLOAT);
    //ライトとカラー合成用
    MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Synthesis], &renderTargetView[(int)Buffer::Synthesis], &shaderResourceViews[(int)Buffer::Synthesis], DXGI_FORMAT_R16G16B16A16_FLOAT);
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;
    DsvBuffer(device, width, height, &depth_stencil_buffer, &depthStencilView, &shaderResourceViews[1]);
    
    Light* light = new Light({ 1.0f, -1.0f, 1.0f }, 0.0f, 1.0f, 1.0f, 1.0f);
    LightManager::Instance().Register(light);
    light = new Light({ -1.0f, -1.0f, 1.0f }, 0.0f, 1.5f, 1.5f, 1.5f);
    LightManager::Instance().Register(light);
    light = new Light({ 1.0f, -1.0f, -1.0f }, 0.0f, 2.0f, 2.0f, 2.0f);
    LightManager::Instance().Register(light);
    light = new Light({ -1.0f, -1.0f, -1.0f }, 0.0f, 1.5f, 1.5f, 1.5f);
    LightManager::Instance().Register(light);
    light = new Light({ 1.0f, -1.0f, 0.0f }, 0.0f, 1.0f, 1.0f, 1.0f);
    LightManager::Instance().Register(light);
    light = new Light({ -1.0f, -1.0f, 0.0f }, 0.0f, 1.5f, 1.5f, 1.5f);
    LightManager::Instance().Register(light);
    light = new Light({ 0.0f, 1.0f, 0.0f }, 0.0f, 0.5f, 0.5f, 0.5f);
    //light->AddDirLight({ 1,-1,0 }, 2.0, 2.0, 2.0);
    //light->AddDirLight({ 1,1,1 }, 0.2, 0.2, 0.2);
    //for (int i = 0; i < 5; i++) {
    //    light->AddPointLight(
    //        DirectX::XMFLOAT3{ (float)(0),
    //             5,
    //             (float)(10 * i)},
    //        100,
    //        rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f);
    //}
   
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
}

void Framebuffer::Clear(ID3D11DeviceContext* immediate_context, float r, float g, float b, float a, float depth)
{
    float color[4]{ r,g,b,a };
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::Color ].Get(), color);
    float cleardepth[4] = { 5000, 1.0f, 1.0f, 1.0f };
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::Depth].Get(), cleardepth);
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::Nomal].Get(), color);
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::Pos].Get(), color);
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::MetalSmoothness].Get(), color);
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::AmbientOcclusion].Get(), color);
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::Emission].Get(), color);
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::Light].Get(), color);
    immediate_context->ClearRenderTargetView(renderTargetView[(int)Buffer::Synthesis].Get(), color);
    immediate_context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, depth, 0);
}

void Framebuffer::Activate(ID3D11DeviceContext* immediate_context)
{
    viewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    immediate_context->RSGetViewports(&viewportCount, cachedViewports);
    immediate_context->OMGetRenderTargets(1, cachedRenderTargetView.ReleaseAndGetAddressOf(),
        cachedDepthStencilView.ReleaseAndGetAddressOf());

    immediate_context->RSSetViewports(1, &viewport);
    ID3D11RenderTargetView* targets[] = 
    {
        renderTargetView[(int)Buffer::Color].Get(),
        renderTargetView[(int)Buffer::Depth].Get(),
        renderTargetView[(int)Buffer::Nomal].Get(),
        renderTargetView[(int)Buffer::Pos].Get(),
        renderTargetView[(int)Buffer::MetalSmoothness].Get(),   // メタリック
        renderTargetView[(int)Buffer::AmbientOcclusion].Get(),  // 粗さ
        renderTargetView[(int)Buffer::Emission].Get(),          // エミッシブ
    };
    // レンダーターゲットビュー設定
    immediate_context->OMSetRenderTargets(7, targets, depthStencilView.Get());
}
void Framebuffer::RenderActivate(ID3D11DeviceContext* immediate_context)
{
    ID3D11RenderTargetView* targets[] = 
    {
        renderTargetView[(int)Buffer::Color].Get(),
        renderTargetView[(int)Buffer::Depth].Get(),
        renderTargetView[(int)Buffer::Nomal].Get(),
        renderTargetView[(int)Buffer::Pos].Get(),
        renderTargetView[(int)Buffer::MetalSmoothness].Get(),
        renderTargetView[(int)Buffer::AmbientOcclusion].Get(),
        renderTargetView[(int)Buffer::Emission].Get(),
    };
    // レンダーターゲットビュー設定
    immediate_context->OMSetRenderTargets(7, targets, depthStencilView.Get());
}
void Framebuffer::EffectActivate(ID3D11DeviceContext* immediate_context)
{
    // レンダーターゲットビュー設定
    immediate_context->OMSetRenderTargets(1, renderTargetView[0].GetAddressOf(), depthStencilView.Get());
}
void Framebuffer::Deactivate(ID3D11DeviceContext* immediate_context)
{
    Lighting(immediate_context);
    immediate_context->RSSetViewports(viewportCount, cachedViewports);
    immediate_context->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(),cachedDepthStencilView.Get());
}

void Framebuffer::Lighting(ID3D11DeviceContext* immediate_context) 
{
    float clearColor[4] = { 0, 0, 0, 1 };
    LightManager& lightManager = LightManager::Instance();
    //ターゲット→ライトバッファ
    immediate_context->OMSetRenderTargets(1, renderTargetView[(int)Buffer::Light].GetAddressOf(), depthStencilView.Get());
    immediate_context->PSSetShaderResources(0, 1, shaderResourceViews[(int)Buffer::Nomal].GetAddressOf());
    //0622 Slot2にGBufferのPosition
    immediate_context->PSSetShaderResources(1, 1, shaderResourceViews[(int)Buffer::Pos].GetAddressOf());   
  
    immediate_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediate_context->IASetInputLayout(nullptr);
   
    immediate_context->OMSetBlendState(Graphics::Instance().GetBlendState(BlendState::Add), NULL, 0xFFFFFFFF);
    lightManager.Activate(immediate_context);
    for (int i = 0; i < lightManager.GetNumDirLight(); i++) 
    {
        if (lightManager.ActivateDirLight(immediate_context, i)) 
        {
            immediate_context->Draw(4, 0);
        }
    }
    //immediate_context->ClearRenderTargetView(renderTargetView[5].Get(), clearColor);
    immediate_context->OMSetRenderTargets(
        1, renderTargetView[(int)Buffer::Synthesis].GetAddressOf(), depthStencilView.Get());
    lightManager.CompositActivate(immediate_context);
    immediate_context->PSSetShaderResources(0, 1, shaderResourceViews[(int)Buffer::Color].GetAddressOf());
    immediate_context->PSSetShaderResources(1, 1, shaderResourceViews[(int)Buffer::Nomal].GetAddressOf());
    immediate_context->PSSetShaderResources(2, 1, shaderResourceViews[(int)Buffer::Light].GetAddressOf());
    immediate_context->PSSetShaderResources(3, 1, shaderResourceViews[(int)Buffer::Depth].GetAddressOf());
    //immediate_context->PSSetShaderResources(3, 1, shaderResourceViews[(int)Buffer::MetalSmoothness].GetAddressOf());
    //immediate_context->PSSetShaderResources(4, 1, shaderResourceViews[(int)Buffer::AmbientOcclusion].GetAddressOf());
    //immediate_context->PSSetShaderResources(7, 1, shaderResourceViews[(int)Buffer::Emission].GetAddressOf());
    immediate_context->Draw(4, 0);
    immediate_context->OMSetBlendState(Graphics::Instance().GetBlendState(BlendState::Alpha), NULL, 0xFFFFFFFF);
}