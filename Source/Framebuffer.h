#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <cstdint>
#include "Light.h"
#include "LightManager.h"
#include "FullscreenQuad.h"

class Framebuffer
{
public:
    enum  class Buffer
    {
        Color,              // 色
        Depth,              // 深度
        Nomal,              // 法線
        Position,           // 3D座標
        MetalSmoothness,    // メタリック
        AmbientOcclusion,   // 粗さ
        Emission,           // エミッシブ
        Light,              // ライト
        Synthesis,          // 
        Max
    };
public:
    Framebuffer(ID3D11Device* device, uint32_t width, uint32_t height);
    virtual ~Framebuffer() = default;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTargetBuffer[(int)Buffer::Max];
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView[(int)Buffer::Max];
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceViews[(int)Buffer::Max];
   
    D3D11_VIEWPORT viewport;

    // MRTクリア
    void Clear(ID3D11DeviceContext* deviceContext, float r = 0, float g = 0, float b = 0, float a = 1, float depth = 1);
    // MRT設定
    void Activate(ID3D11DeviceContext* deviceContext);
    void RenderActivate(ID3D11DeviceContext* deviceContext);
    void EffectActivate(ID3D11DeviceContext* deviceContext);
    void Deactivate(ID3D11DeviceContext* deviceContext);

    void Lighting(ID3D11DeviceContext* deviceContext);

private:
    UINT viewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
    D3D11_VIEWPORT cachedViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cachedRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cachedDepthStencilView;
};