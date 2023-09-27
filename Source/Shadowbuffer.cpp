#include "Shadowbuffer.h"
#include "misc.h"

Shadowbuffer::Shadowbuffer(ID3D11Device* device, uint32_t width, uint32_t height) {
    HRESULT hr{ S_OK };

    Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTargetBuffer;
    D3D11_TEXTURE2D_DESC texture2dDesc{};
    texture2dDesc.Width = width;
    texture2dDesc.Height = height;
    texture2dDesc.MipLevels = 1;
    texture2dDesc.ArraySize = 1;
    texture2dDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
    texture2dDesc.SampleDesc.Count = 1;
    texture2dDesc.SampleDesc.Quality = 0;
    texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
    texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texture2dDesc.CPUAccessFlags = 0;
    texture2dDesc.MiscFlags = 0;
    hr = device->CreateTexture2D(&texture2dDesc, 0, renderTargetBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
    renderTargetViewDesc.Format = texture2dDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    hr = device->CreateRenderTargetView(renderTargetBuffer.Get(), &renderTargetViewDesc,
        RtvShadow.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    shaderResourceViewDesc.Format = texture2dDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(renderTargetBuffer.Get(), &shaderResourceViewDesc,
        SrvShadow.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    hr = device->CreateTexture2D(&texture2dDesc, 0, depthStencilBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
    depthStencilViewDesc.Format = texture2dDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = 0;
    hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc,
        DepthStencilView.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

}
void Shadowbuffer::clear(ID3D11DeviceContext* immediate_context,
    float r, float g, float b, float a, float depth)
{
    float color[4]{ r,g,b,a };
    immediate_context->ClearRenderTargetView(RtvShadow.Get(), color);
    immediate_context->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH, depth, 0);

}
void Shadowbuffer::activate(ID3D11DeviceContext* immediate_context)
{


    viewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    immediate_context->RSGetViewports(&viewportCount, cachedViewports);
    immediate_context->OMGetRenderTargets(1, cachedRenderTargetView.ReleaseAndGetAddressOf(),
        cachedDepthStencilView.ReleaseAndGetAddressOf());
 
    immediate_context->RSSetViewports(1, &viewport);
    immediate_context->OMSetRenderTargets(1, RtvShadow.GetAddressOf(),
        DepthStencilView.Get());

}
void Shadowbuffer::deactivate(ID3D11DeviceContext* immediate_context)
{

    //immediate_context->PSSetShaderResources(10, 1, SrvShadow.GetAddressOf());
    immediate_context->RSSetViewports(viewportCount, cachedViewports);
    immediate_context->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(),
        cachedDepthStencilView.Get());
    //Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cached_ShaderResourceView = nullptr;
    immediate_context->PSSetShaderResources(2, 1, SrvShadow.GetAddressOf());
    
}