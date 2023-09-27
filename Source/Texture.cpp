#include "Texture.h"
#include "misc.h"
#include <DDSTextureLoader.h>
#include "memory"
#include <filesystem>
#include <WICTextureLoader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace DirectX;

#include<wrl.h>
using namespace  Microsoft::WRL;

#include<string>
#include<map>
using namespace std;

static map<wstring, ComPtr<ID3D11ShaderResourceView>> resources;

HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* filename,
    ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc) 
{
    HRESULT hr{ S_OK };
    ComPtr<ID3D11Resource> resource;
    
    auto it = resources.find(filename);
    std::filesystem::path dds_filename(filename);
    dds_filename.replace_extension("dds");
    if (std::filesystem::exists(dds_filename.c_str()))
    {
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;
        device->GetImmediateContext(immediate_context.GetAddressOf());
        hr = DirectX::CreateDDSTextureFromFile(device, immediate_context.Get(), dds_filename.c_str(),
            resource.GetAddressOf(), shader_resource_view);
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }
    else
    {
        hr = CreateWICTextureFromFile(device, filename, resource.GetAddressOf(), shader_resource_view);
        ////hr = make_dummy_texture(device, shader_resource_view, 0xFFFFFFFF, 16);
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
        resources.insert(make_pair(filename, *shader_resource_view));
    }
     ComPtr<ID3D11Texture2D> texture2d;
     hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
     _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
     texture2d->GetDesc(texture2d_desc);

     return hr;
}
HRESULT LoadTextureFromFile2(ID3D11Device* device, const char* filename,
    ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc)
{
    

    wchar_t wfilename[256];
    ::MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, 256);

    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilename, resource.GetAddressOf(), shader_resource_view);
   
        // WIC�ŃT�|�[�g����Ă��Ȃ��t�H�[�}�b�g�̏ꍇ�iTGA�Ȃǁj��
        // STB�ŉ摜�ǂݍ��݂����ăe�N�X�`���𐶐�����
    int width, height, bpp;
    unsigned char* pixels = stbi_load(filename, &width, &height, &bpp, STBI_rgb_alpha);
    if (pixels != nullptr)
    {
        D3D11_TEXTURE2D_DESC desc = { 0 };
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA data;
        ::memset(&data, 0, sizeof(data));
        data.pSysMem = pixels;
        data.SysMemPitch = width * 4;

        Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
        HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

        hr = device->CreateShaderResourceView(texture.Get(), nullptr, shader_resource_view);
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

        // ��n��
        stbi_image_free(pixels);
    }
    else
    {
         // �ǂݍ��ݎ��s������_�~�[�e�N�X�`�������
         //LOG("load failed : %s\n", filename);
       
         const int width = 8;
         const int height = 8;
         UINT pixels[width * height];
         ::memset(pixels, 0xFF, sizeof(pixels));
       
         D3D11_TEXTURE2D_DESC desc = { 0 };
         desc.Width = width;
         desc.Height = height;
         desc.MipLevels = 1;
         desc.ArraySize = 1;
         desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
         desc.SampleDesc.Count = 1;
         desc.SampleDesc.Quality = 0;
         desc.Usage = D3D11_USAGE_DEFAULT;
         desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
         desc.CPUAccessFlags = 0;
         desc.MiscFlags = 0;
         D3D11_SUBRESOURCE_DATA data;
         ::memset(&data, 0, sizeof(data));
         data.pSysMem = pixels;
         data.SysMemPitch = width;
       
         Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
         HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
         _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
       
         hr = device->CreateShaderResourceView(texture.Get(), nullptr, shader_resource_view);
         _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    return hr;
}
void ReleaseAllTextures()
{
    resources.clear();
}
    
/// <summary>
/// �_�~�[�̃e�N�X�`�����쐬
/// </summary>
/// <param name="device">�f�o�C�X</param>
/// <param name="shader_resource_view">�������ꂽ�e�N�X�`��</param>
/// <param name="value">�F</param>
/// <param name="dimension">�T�C�Y</param>
/// <returns></returns>
HRESULT MakeDummyTexture(
    ID3D11Device* device,
    ID3D11ShaderResourceView** shader_resource_view,
    DWORD value,
    UINT dimension)
{
    HRESULT hr{ S_OK };

    // �_�~�[�̃e�N�X�`������ݒ�
    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // �_�~�[�̃e�N�Z������ݒ�
    size_t texels = dimension * dimension;
    unique_ptr<DWORD[]> sysmem{ make_unique<DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i)
        sysmem[i] = value;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    // �_�~�[�̏��Ńe�N�X�`�����쐬
    ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    // �_�~�[�̃e�N�X�`�����ŃV�F�[�_�[���\�[�X�r���[���쐬
    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(
        texture2d.Get(),
        &shader_resource_view_desc,
        shader_resource_view);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    return hr;
}
