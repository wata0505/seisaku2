#include "Framebuffer.h"
#include "Buffer.h"
#include "misc.h"
#include "Graphics.h"
#include "Shader.h"

// �R���X�g���N�^
Framebuffer::Framebuffer(ID3D11Device* device, uint32_t width, uint32_t height) 
{
    // �o�b�t�@�쐬
    {
        // �F�o�t�@�쐬
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Color], &renderTargetView[(int)Buffer::Color], &shaderResourceViews[(int)Buffer::Color], DXGI_FORMAT_R16G16B16A16_FLOAT);
        // �[�x�o�b�t�@�쐬                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Depth], &renderTargetView[(int)Buffer::Depth], &shaderResourceViews[(int)Buffer::Depth], DXGI_FORMAT_R32_FLOAT);
        // �@���o�b�t�@�쐬                                                                                                         
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Nomal], &renderTargetView[(int)Buffer::Nomal], &shaderResourceViews[(int)Buffer::Nomal], DXGI_FORMAT_R8G8B8A8_UNORM);
        // 3D���W�o�b�t�@�쐬                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Position], &renderTargetView[(int)Buffer::Position], &shaderResourceViews[(int)Buffer::Position], DXGI_FORMAT_R32G32B32A32_FLOAT);
        // ���^���b�N�o�b�t�@�쐬                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::MetalSmoothness], &renderTargetView[(int)Buffer::MetalSmoothness], &shaderResourceViews[(int)Buffer::MetalSmoothness], DXGI_FORMAT_R32_FLOAT);
        // �e���o�b�t�@�쐬                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::AmbientOcclusion], &renderTargetView[(int)Buffer::AmbientOcclusion], &shaderResourceViews[(int)Buffer::AmbientOcclusion], DXGI_FORMAT_R32_FLOAT);
        // �G�~�b�V�u�o�b�t�@�쐬                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Emission], &renderTargetView[(int)Buffer::Emission], &shaderResourceViews[(int)Buffer::Emission], DXGI_FORMAT_R32G32B32A32_FLOAT);
        // ���C�g�o�b�t�@����                                                                                                       
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Light], &renderTargetView[(int)Buffer::Light], &shaderResourceViews[(int)Buffer::Light], DXGI_FORMAT_R16G16B16A16_FLOAT);
        // ���C�g�ƃJ���[�����p
        MakeBuffer(device, width, height, &renderTargetBuffer[(int)Buffer::Synthesis], &renderTargetView[(int)Buffer::Synthesis], &shaderResourceViews[(int)Buffer::Synthesis], DXGI_FORMAT_R16G16B16A16_FLOAT);
        
        Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
        DsvBuffer(device, width, height, &depthStencilBuffer, &depthStencilView, &shaderResourceViews[(int)Buffer::Depth]);
    }
    
    // ���C�g����
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

    // �r���[�|�[�g�ݒ�
    {
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
    }
}

// MRT�N���A
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
// MRT�ݒ�
void Framebuffer::Activate(ID3D11DeviceContext* deviceContext)
{
    viewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    deviceContext->RSGetViewports(&viewportCount, cachedViewports);
    deviceContext->OMGetRenderTargets(1, cachedRenderTargetView.ReleaseAndGetAddressOf(),
        cachedDepthStencilView.ReleaseAndGetAddressOf());

    deviceContext->RSSetViewports(1, &viewport);
    ID3D11RenderTargetView* targets[] = 
    {
        renderTargetView[(int)Buffer::Color].Get(),             // �F
        renderTargetView[(int)Buffer::Depth].Get(),             // �[�x
        renderTargetView[(int)Buffer::Nomal].Get(),             // �@��
        renderTargetView[(int)Buffer::Position].Get(),          // 3D���W
        renderTargetView[(int)Buffer::MetalSmoothness].Get(),   // ���^���b�N
        renderTargetView[(int)Buffer::AmbientOcclusion].Get(),  // �e��
        renderTargetView[(int)Buffer::Emission].Get(),          // �G�~�b�V�u
    };
    // �����_�[�^�[�Q�b�g�r���[�ݒ�
    deviceContext->OMSetRenderTargets(7, targets, depthStencilView.Get());
}
void Framebuffer::RenderActivate(ID3D11DeviceContext* deviceContext)
{
    ID3D11RenderTargetView* targets[] = 
    {
        renderTargetView[(int)Buffer::Color].Get(),             // �F
        renderTargetView[(int)Buffer::Depth].Get(),             // �[�x
        renderTargetView[(int)Buffer::Nomal].Get(),             // �@��
        renderTargetView[(int)Buffer::Position].Get(),          // 3D���W
        renderTargetView[(int)Buffer::MetalSmoothness].Get(),   // ���^���b�N
        renderTargetView[(int)Buffer::AmbientOcclusion].Get(),  // �e��
        renderTargetView[(int)Buffer::Emission].Get(),          // �G�~�b�V�u
    };
    // �����_�[�^�[�Q�b�g�r���[�ݒ�
    deviceContext->OMSetRenderTargets(7, targets, depthStencilView.Get());
}
void Framebuffer::EffectActivate(ID3D11DeviceContext* deviceContext)
{
    // �����_�[�^�[�Q�b�g�r���[�ݒ�
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
    // �^�[�Q�b�g�����C�g�o�b�t�@
    deviceContext->OMSetRenderTargets(1, renderTargetView[(int)Buffer::Light].GetAddressOf(), depthStencilView.Get());
    deviceContext->PSSetShaderResources(0, 1, shaderResourceViews[(int)Buffer::Nomal].GetAddressOf());
    // Slot2��GBuffer��Position
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