#include "SpriteBatch.h"
#include "texture.h"
#include "misc.h"
#include <sstream>
#include <functional>
#include <WICTextureLoader.h>
#include "Shader.h"

SpriteBatch::SpriteBatch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites)
    :max_vertices(max_sprites * 6)
{
	HRESULT hr{ S_OK };

	// UNIT.09
	//vertex vertices[]
	//{
	//	{ { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } /*UNIT.05*/},
	//	{ { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } /*UNIT.05*/},
	//	{ { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } /*UNIT.05*/},
	//	{ { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } /*UNIT.05*/},
	//};
	std::unique_ptr<vertex[]> vertices{ std::make_unique<vertex[]>(max_vertices) };

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * max_vertices);	// UNIT.09
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA subresource_data{};
	subresource_data.pSysMem = vertices.get();	// UNIT.09
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;
	hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	{
		


		D3D11_INPUT_ELEMENT_DESC input_element_desc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};  
        create_vs_from_cso(device, "Shader\\sprite_vs.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
        create_ps_from_cso(device, "Shader\\sprite_ps.cso", pixelShader.GetAddressOf());
	
	}
	

	//ID3D11Resource* resource{};
	//hr = DirectX::CreateWICTextureFromFile(device, filename, &resource, &shader_resource_view);
	//_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//resource->Release();
    //
	//ID3D11Texture2D* texture2d{};
	//hr = resource->QueryInterface<ID3D11Texture2D>(&texture2d);
	//_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//texture2d->GetDesc(&texture2d_desc);
	//texture2d->Release();
    LoadTextureFromFile(device, filename,shaderResourceView.GetAddressOf(), &texture2dDesc);
}
void SpriteBatch::Render(ID3D11DeviceContext* immediate_context,
    // UNIT.03
    float dx, float dy, float dw, float dh,
    float r, float g, float b, float a,
    // UNIT.04
    float angle/*degree*/)
{
    // UNIT.06
    Render(immediate_context, dx, dy, dw, dh, r, g, b, a, angle, 0.0f, 0.0f, static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height));
}

void SpriteBatch::Render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle/*degree*/, float sx, float sy, float sw, float sh) {

    D3D11_VIEWPORT viewport{}; //ビューポート情報
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);

    //(x0,y0)*----*(x1, y1)
    //       |   /|
    //       |  / |
    //       | /  |
    //       |/   |
    //(x2,y2)*----*(x3,y3)

    //left-top
    float x0(dx);
    float y0(dy);
    //right-top
    float x1{ dx + dw };
    float y1{ dy };
    //left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    //right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };

    //auto rotate = [](float& x, float& y, float cx, float cy, float angle) {
    //    x -= cx;
    //    y -= cy;
    //
    //    float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
    //    float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
    //    float tx{ x }, ty{ y };
    //
    //    x = cos * tx + -sin * ty;
    //    y = sin * tx + cos * ty;
    //
    //    x += cx;
    //    y += cy;
    //
    //};

    float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
    float sin{ sinf(DirectX::XMConvertToRadians(angle)) };

    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;

    rotate(x0, y0, cx, cy, sin, cos);
    rotate(x1, y1, cx, cy, sin, cos);
    rotate(x2, y2, cx, cy, sin, cos);
    rotate(x3, y3, cx, cy, sin, cos);

    //スクリーン座標系から NDC　座標系に変換
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;

    float u0{ sx / texture2dDesc.Width };
    float v0{ sy / texture2dDesc.Height };
    float u1{ (sx + sw) / texture2dDesc.Width };
    float v1{ (sy + sh) / texture2dDesc.Height };

    vertices.push_back({ {x0,y0,0},{r,g,b,a},{u0,v0} });
    vertices.push_back({ {x1,y1,0},{r,g,b,a},{u1,v0} });
    vertices.push_back({ {x2,y2,0},{r,g,b,a},{u0,v1} });
    vertices.push_back({ {x2,y2,0},{r,g,b,a},{u0,v1} });
    vertices.push_back({ {x1,y1,0},{r,g,b,a},{u1,v0} });
    vertices.push_back({ {x3,y3,0},{r,g,b,a},{u1,v1} });
   



    ////プリミティブトポロジーの設定
    //immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


    ////入力レイアウトオブジェクトの設定
    //immediate_context->IASetInputLayout(input_layout);

    ////頂点シェーダーオブジェクトの設定
    //immediate_context->VSSetShader(vertex_shader, nullptr, 0);
    ////ピクセルシェーダーオブジェクトの設定
    //immediate_context->PSSetShader(pixel_shader, nullptr, 0);

    //immediate_context->PSSetShaderResources(0, 1, &shader_resource_view);

    ////描画処理
    //immediate_context->Draw(4, 0);


}
void SpriteBatch::Render(ID3D11DeviceContext* immediate_context,
    float dx, float dy, float dw, float dh)
{
    // UNIT.06
    Render(immediate_context,
        dx, dy, dw, dh,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 
        static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height));
}

SpriteBatch::~SpriteBatch() {
   // vertex_buffer->Release();
   // pixel_shader->Release();
   // input_layout->Release();
   // vertex_shader->Release();
   // shader_resource_view->Release();
    ReleaseAllTextures();
}
void SpriteBatch::Begin(ID3D11DeviceContext* immediate_context, ID3D11PixelShader* replaced_pixel_shader, ID3D11ShaderResourceView* replaced_shader_resource_view )
{
    vertices.clear();
    immediate_context->VSSetShader(vertexShader.Get(), nullptr, 0);
    replaced_pixel_shader ? immediate_context->PSSetShader(replaced_pixel_shader, nullptr, 0) : immediate_context->PSSetShader(pixelShader.Get(), nullptr, 0);
    
    if(replaced_shader_resource_view)
    {
        // texture2d_desc に差し変わったテクスチャの情報を設定する
        HRESULT hr{ S_OK };
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        replaced_shader_resource_view->GetResource(resource.GetAddressOf());
        Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
        hr = resource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
        texture2d->GetDesc(&texture2dDesc);

                   // 差し変わったテクスチャを設定
           immediate_context->PSSetShaderResources(0, 1, &replaced_shader_resource_view);
        
    }
    else
         {
                   // 今までのテクスチャを設定
           immediate_context->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
        
    }
}


void SpriteBatch::End(ID3D11DeviceContext* immediate_context)
{
    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    size_t vertex_count = vertices.size();
    _ASSERT_EXPR(max_vertices >= vertex_count, "Buffer overflow");
    vertex* data{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (data != nullptr)
    {
        const vertex* p = &(vertices.at(0));
        memcpy_s(data, max_vertices * sizeof(vertex), p, vertex_count * sizeof(vertex));

    }
    immediate_context->Unmap(vertexBuffer.Get(), 0);

    UINT stride{ sizeof(vertex) };
    UINT offsef{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offsef);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(inputLayout.Get());

    immediate_context->Draw(static_cast<UINT>(vertex_count), 0);
}

