#include "Buffer.h"
#include "misc.h"

void MakeBuffer(ID3D11Device* device, uint32_t width, uint32_t height, Microsoft::WRL::ComPtr<ID3D11Texture2D>* TEX, Microsoft::WRL::ComPtr <ID3D11RenderTargetView>* RTV, Microsoft::WRL::ComPtr <ID3D11ShaderResourceView>* SRV,DXGI_FORMAT Format) {
    HRESULT hr{ S_OK };

    //Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
    D3D11_TEXTURE2D_DESC texture2dDesc{};
    texture2dDesc.Width = width;
    texture2dDesc.Height = height;
    texture2dDesc.MipLevels = 1;
    texture2dDesc.ArraySize = 1;
    texture2dDesc.Format = Format;
    texture2dDesc.SampleDesc.Count = 1;
    texture2dDesc.SampleDesc.Quality = 0;
    texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
    texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texture2dDesc.CPUAccessFlags = 0;
    texture2dDesc.MiscFlags = 0;
    hr = device->CreateTexture2D(&texture2dDesc, 0, TEX->GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc{};
    RenderTargetViewDesc.Format = texture2dDesc.Format;
    RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    hr = device->CreateRenderTargetView(TEX->Get(), &RenderTargetViewDesc,
        RTV->GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    shaderResourceViewDesc.Format = texture2dDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(TEX->Get(), &shaderResourceViewDesc,
        SRV->GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

}
void DsvBuffer(ID3D11Device* device, uint32_t width, uint32_t height, Microsoft::WRL::ComPtr<ID3D11Texture2D>* TEX, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>* DSV, Microsoft::WRL::ComPtr <ID3D11ShaderResourceView>* SRV) {
    HRESULT hr{ S_OK };

    //Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
    D3D11_TEXTURE2D_DESC texture2dDesc{};
    texture2dDesc.Width = width;
    texture2dDesc.Height = height;
    texture2dDesc.MipLevels = 1;
    texture2dDesc.ArraySize = 1;
    texture2dDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    texture2dDesc.SampleDesc.Count = 1;
    texture2dDesc.SampleDesc.Quality = 0;
    texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
    texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    texture2dDesc.CPUAccessFlags = 0;
    texture2dDesc.MiscFlags = 0;
    hr = device->CreateTexture2D(&texture2dDesc, 0, TEX->GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    
  

    D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc{};
    DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DepthStencilViewDesc.Flags = 0;
    hr = device->CreateDepthStencilView(TEX->Get(), &DepthStencilViewDesc,
        DSV->GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};

    shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(TEX->Get(), &shaderResourceViewDesc,
        SRV->GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

}