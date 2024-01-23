#include "Framebuffer.h"
#include "Buffer.h"
#include "misc.h"
#include "Graphics.h"
#include "Shader.h"

// コンストラクタ
Framebuffer::Framebuffer(ID3D11Device* device, uint32_t width, uint32_t height) 
{
    // バッファ作成
    {
        // 色バファ作成
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Color], &renderTargetView[(int)Buffer::Color], &shaderResourceViews[(int)Buffer::Color], DXGI_FORMAT_R16G16B16A16_FLOAT);
        // 深度バッファ作成                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Depth], &renderTargetView[(int)Buffer::Depth], &shaderResourceViews[(int)Buffer::Depth], DXGI_FORMAT_R32_FLOAT);
        // 法線バッファ作成                                                                                                         
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Nomal], &renderTargetView[(int)Buffer::Nomal], &shaderResourceViews[(int)Buffer::Nomal], DXGI_FORMAT_R8G8B8A8_UNORM);
        // 3D座標バッファ作成                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Position], &renderTargetView[(int)Buffer::Position], &shaderResourceViews[(int)Buffer::Position], DXGI_FORMAT_R32G32B32A32_FLOAT);
        // メタリックバッファ作成                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::MetalSmoothness], &renderTargetView[(int)Buffer::MetalSmoothness], &shaderResourceViews[(int)Buffer::MetalSmoothness], DXGI_FORMAT_R32_FLOAT);
        // 粗さバッファ作成                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::AmbientOcclusion], &renderTargetView[(int)Buffer::AmbientOcclusion], &shaderResourceViews[(int)Buffer::AmbientOcclusion], DXGI_FORMAT_R32_FLOAT);
        // エミッシブバッファ作成                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Emission], &renderTargetView[(int)Buffer::Emission], &shaderResourceViews[(int)Buffer::Emission], DXGI_FORMAT_R32G32B32A32_FLOAT);
        // ライトバッファ生成                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Light], &renderTargetView[(int)Buffer::Light], &shaderResourceViews[(int)Buffer::Light], DXGI_FORMAT_R16G16B16A16_FLOAT);
        // ライトとカラー合成用
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Synthesis], &renderTargetView[(int)Buffer::Synthesis], &shaderResourceViews[(int)Buffer::Synthesis], DXGI_FORMAT_R16G16B16A16_FLOAT);
        
        Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
        DsvBuffer(device, width, height, &depthStencilBuffer, &depthStencilView, &shaderResourceViews[(int)Buffer::Depth]);
    }
    
    // ライト生成
    {
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
    }

    // ビューポート設定
    {
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
    }
}

// MRTクリア
void Framebuffer::Clear(ID3D11DeviceContext* deviceContext, float r, float g, float b, float a, float depth)
{
    float color[4] = { r, g, b, a };
    float cleardepth[4] = { 5000, 1.0f, 1.0f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::Color].Get(), color);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::Depth].Get(), cleardepth);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::Nomal].Get(), color);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::Position].Get(), color);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::MetalSmoothness].Get(), color);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::AmbientOcclusion].Get(), color);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::Emission].Get(), color);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::Light].Get(), color);
    deviceContext->ClearRenderTargetView(renderTargetView[(int)Buffer::Synthesis].Get(), color);
    deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, depth, 0);
}
// MRT設定
void Framebuffer::Activate(ID3D11DeviceContext* deviceContext)
{
    viewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    deviceContext->RSGetViewports(&viewportCount, cachedViewports);
    deviceContext->OMGetRenderTargets(1, cachedRenderTargetView.ReleaseAndGetAddressOf(),
        cachedDepthStencilView.ReleaseAndGetAddressOf());

    deviceContext->RSSetViewports(1, &viewport);
    ID3D11RenderTargetView* targets[] = 
    {
        renderTargetView[(int)Buffer::Color].Get(),             // 色
        renderTargetView[(int)Buffer::Depth].Get(),             // 深度
        renderTargetView[(int)Buffer::Nomal].Get(),             // 法線
        renderTargetView[(int)Buffer::Position].Get(),          // 3D座標
        renderTargetView[(int)Buffer::MetalSmoothness].Get(),   // メタリック
        renderTargetView[(int)Buffer::AmbientOcclusion].Get(),  // 粗さ
        renderTargetView[(int)Buffer::Emission].Get(),          // エミッシブ
    };
    // レンダーターゲットビュー設定
    deviceContext->OMSetRenderTargets(7, targets, depthStencilView.Get());
}
void Framebuffer::RenderActivate(ID3D11DeviceContext* deviceContext)
{
    ID3D11RenderTargetView* targets[] = 
    {
        renderTargetView[(int)Buffer::Color].Get(),             // 色
        renderTargetView[(int)Buffer::Depth].Get(),             // 深度
        renderTargetView[(int)Buffer::Nomal].Get(),             // 法線
        renderTargetView[(int)Buffer::Position].Get(),          // 3D座標
        renderTargetView[(int)Buffer::MetalSmoothness].Get(),   // メタリック
        renderTargetView[(int)Buffer::AmbientOcclusion].Get(),  // 粗さ
        renderTargetView[(int)Buffer::Emission].Get(),          // エミッシブ
    };
    // レンダーターゲットビュー設定
    deviceContext->OMSetRenderTargets(7, targets, depthStencilView.Get());
}
void Framebuffer::EffectActivate(ID3D11DeviceContext* deviceContext)
{
    // レンダーターゲットビュー設定
    deviceContext->OMSetRenderTargets(1, renderTargetView[0].GetAddressOf(), depthStencilView.Get());
}
void Framebuffer::Deactivate(ID3D11DeviceContext* deviceContext)
{
    Lighting(deviceContext);
    deviceContext->RSSetViewports(viewportCount, cachedViewports);
    deviceContext->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(),cachedDepthStencilView.Get());
}

void Framebuffer::Lighting(ID3D11DeviceContext* deviceContext)
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    LightManager& lightManager = LightManager::Instance();
    // ターゲット→ライトバッファ
    deviceContext->OMSetRenderTargets(1, renderTargetView[(int)Buffer::Light].GetAddressOf(), depthStencilView.Get());
    deviceContext->PSSetShaderResources(0, 1, shaderResourceViews[(int)Buffer::Nomal].GetAddressOf());
    // Slot2にGBufferのPosition
    deviceContext->PSSetShaderResources(1, 1, shaderResourceViews[(int)Buffer::Position].GetAddressOf());
  
    deviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    deviceContext->IASetInputLayout(nullptr);
   
    deviceContext->OMSetBlendState(Graphics::Instance().GetBlendState(BlendState::Add), NULL, 0xFFFFFFFF);
    lightManager.Activate(deviceContext);
    for (int i = 0; i < lightManager.GetDirLightCount(); i++) 
    {
        if (lightManager.ActivateDirLight(deviceContext, i))
        {
            deviceContext->Draw(4, 0);
        }
    }
    deviceContext->OMSetRenderTargets(1, renderTargetView[(int)Buffer::Synthesis].GetAddressOf(), depthStencilView.Get());
    lightManager.CompositActivate(deviceContext);
    deviceContext->PSSetShaderResources(0, 1, shaderResourceViews[(int)Buffer::Color].GetAddressOf());
    deviceContext->PSSetShaderResources(1, 1, shaderResourceViews[(int)Buffer::Nomal].GetAddressOf());
    deviceContext->PSSetShaderResources(2, 1, shaderResourceViews[(int)Buffer::Light].GetAddressOf());
    deviceContext->PSSetShaderResources(3, 1, shaderResourceViews[(int)Buffer::Depth].GetAddressOf());    
    deviceContext->Draw(4, 0);
    deviceContext->OMSetBlendState(Graphics::Instance().GetBlendState(BlendState::Alpha), NULL, 0xFFFFFFFF);
}