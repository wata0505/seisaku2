#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <cstdint>
class Shadowbuffer
{
public:
    Shadowbuffer(ID3D11Device* device, uint32_t width, uint32_t height);
    virtual ~Shadowbuffer() = default;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RtvShadow;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SrvShadow;
    D3D11_VIEWPORT viewport;

    void clear(ID3D11DeviceContext* immediate_context,
        float r = 0, float g = 0, float b = 0, float a = 1, float depth = 1);
    void activate(ID3D11DeviceContext* immediate_context);
    void deactivate(ID3D11DeviceContext* immediate_context);

private:
    UINT viewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
    D3D11_VIEWPORT cachedViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cachedRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cachedDepthStencilView;

};