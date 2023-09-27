#include "StaticMesh.h"
#include "Shader.h"
#include "misc.h"
#include <vector>
#include <fstream>
#include <filesystem>
#include "texture.h"


using namespace DirectX;

StaticMesh::StaticMesh(ID3D11Device* device, const wchar_t* obj_filename, bool flipping_v_coordinates)
{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t current_index{ 0 };

    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texcoords;
    std::vector<std::wstring> mtl_filenames;

    std::wifstream fin(obj_filename);
    _ASSERT_EXPR(fin, L"'OBJ file not found.");


    // 行の情報
    wchar_t command[256];
    while (fin)
    {
        // １番上にある１行を取得
        fin >> command;

        if (0 == wcscmp(command, L"v"))
        {
            // 頂点座標を取得
            float x, y, z;
            // 区切り単位でデータを取得していく
            fin >> x >> y >> z;
            positions.push_back({ x, y, z });

            // １番上にある１行を削除
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vn"))
        {
            // 法線を取得
            float i, j, k;
            fin >> i >> j >> k;
            normals.push_back({ i, j, k });

            // １番上にある１行を削除
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vt"))
        {
            float u, v;
            fin >> u >> v;
            texcoords.push_back({ u, flipping_v_coordinates ? 1.0f - v : v });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; i++)
            {
                vertex vertex;
                size_t v, vt, vn;

                // １番上にある１行目の１つ目のデータを取得し
                // データのうちの１つ目の数値を取得
                fin >> v;
                vertex.position = positions.at(v - 1);
                if (L'/' == fin.peek())
                {
                    // １文字削除
                    fin.ignore(1);
                    if (L'/' != fin.peek())
                    {
                        // スラッシュ区切りで次の数値を取得
                        fin >> vt;
                        vertex.texcoord = texcoords.at(vt - 1);
                    }
                    if (L'/' == fin.peek())
                    {
                        // １文字削除
                        fin.ignore(1);
                        // スラッシュ区切りで次の数値を取得
                        fin >> vn;
                        vertex.normal = normals.at(vn - 1);
                    }
                }

                // 頂点データを設定
                vertices.push_back(vertex);

                // 頂点インデックスを設定
                indices.push_back(current_index++);
            }

            // １番上にある１行を削除
            fin.ignore(1024, L'\n');
        }
        else if(0 == wcscmp(command, L"mtllib"))
        {
            //マテリアルファイル名を取得
            wchar_t mtllib[256];
            fin >> mtllib;
            mtl_filenames.push_back(mtllib);
        }
        else if (0 == wcscmp(command, L"usemtl"))
        {
            // マテリアル名
            wchar_t usemtl[MAX_PATH]{ 0 };
            fin >> usemtl;
            
            // マテリアル名とインデックス開始番号を保存
            subsets.push_back({ usemtl, static_cast<uint32_t>(indices.size()),0 });
       
        }
        else
        {
            // １番上にある１行を削除
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();

    // それぞれのサブセットのインデックスの数を計算していく
    std::vector<subset>::reverse_iterator iterator = subsets.rbegin();
    iterator->indexCount = static_cast<uint32_t>(indices.size()) - iterator->indexStart;
    for (iterator = subsets.rbegin() + 1; iterator != subsets.rend(); ++iterator)
    {
        iterator->indexCount = (iterator - 1)->indexStart - iterator->indexStart;
    }
    // MTL　ファイル名、OBJファイル名を設定
    std::filesystem::path mtl_filename(obj_filename);

    //ファイル名部分のみMTLファイル名に入れ替える
    mtl_filename.replace_filename(std::filesystem::path(mtl_filenames[0]).filename());

    //マテリアルファイルを開く
    fin.open(mtl_filename);
    //_ASSERT_EXPR(fin, L"'MTL file not found.");



    while (fin)
    {
        //一番上にある１行を取得
        fin >> command;

        if (0 == wcscmp(command, L"map_Kd"))
        {
            fin.ignore();

            //テクニックファイルを読み込む
            wchar_t map_kd[256];
            fin >> map_kd;

            //テクスチャファイル名にパスを取り付ける
            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_kd).filename());
            materials.rbegin()->textureFilenames[0] = path;

            fin.ignore(1024, L'\n');
        }
        else if(0 == wcscmp(command, L"map_bump") || 0 == wcscmp(command, L"bump"))
        {
            fin.ignore();

            //バンプマップ用のテクスチャファイル名を取得
            wchar_t map_bump[256];
            fin >> map_bump;
            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_bump).filename());
            materials.rbegin()->textureFilenames[1] = path;
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"newmtl"))
        {
            fin.ignore();
            wchar_t newmtl[256];
            // 新規でマテリアルを作成
            material mat;
            fin >> newmtl;
            mat.name = newmtl;
            // マテリアルを取り付け
            materials.push_back(mat);
        
        }
        else if (0 == wcscmp(command, L"Kd"))
        {
            // 色を取得し生成したマテリアルに設定
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Kd = { r, g, b, 1 };
            fin.ignore(1024, L'\n');
            
        }
        else
        {
            fin.ignore(1024, L'\n');
        }

        
    }

    CreateComBuffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());

    HRESULT hr{ S_OK };

    // 入力レイアウトオブジェクトの設定
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    // 頂点シェーダーオブジェクトの作成
    create_vs_from_cso(device, "static_mesh_vs.cso", vertexShader.GetAddressOf(),
        inputLayout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    // ピクセルシェーダーオブジェクトの作成
    create_ps_from_cso(device, "static_mesh_ps.cso", pixelShader.GetAddressOf());

    // 定数バッファの作成
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    
    if (materials.size() == 0)
    {
        for (const subset& s : subsets)
        {
            materials.push_back({ s.usemtl });
        }
    }
    
    // 読み込んだテクスチャを生成する
    D3D11_TEXTURE2D_DESC texture2d_desc{};


    //load_texture_from_file(device, texture_filename.c_str(), shader_resource_view.GetAddressOf(), &texture2d_desc);
    
#if false
    for (material & mat : materials)
    {
        if (mat.textureFilenames[0].size() > 0) {
            load_texture_from_file(device, mat.textureFilenames[0].c_str(),
                mat.shader_resource_views[0].GetAddressOf(), &texture2d_desc);
        }
        if (mat.textureFilenames[1].size() > 0) {
            load_texture_from_file(device, mat.textureFilenames[1].c_str(),
                mat.shader_resource_views[1].GetAddressOf(), &texture2d_desc);
        }
    }
#else
    for (material& mat : materials)
    {
        if (mat.textureFilenames[0].size() > 0) {
            LoadTextureFromFile(device, mat.textureFilenames[0].c_str(),
                mat.shader_resource_views[0].GetAddressOf(), &texture2d_desc);
        }
        else
        {
            //ダミー用のカラーマップテクスチャを生成し設定する
            MakeDummyTexture(device, mat.shader_resource_views[0].GetAddressOf(),
                0xFFFFFFFF, 16);
        }
        if (mat.textureFilenames[1].size() > 0) {
            LoadTextureFromFile(device, mat.textureFilenames[1].c_str(),
                mat.shader_resource_views[1].GetAddressOf(), &texture2d_desc);
        }
        else
        {
            // ダミー用の法線マップテクスチャを生成し設定する
                MakeDummyTexture(device, mat.shader_resource_views[1].GetAddressOf(),
                    0xFFFF7F7F, 16);
        }
    }
#endif
    // 読み込んだ頂点座標の x y z の最小、最大をそれぞれバウンディングボックスに設定する
    for (const vertex& v : vertices)
     {
      boundingBox[0].x = std::min<float>(boundingBox[0].x, v.position.x);
      boundingBox[0].y = std::min<float>(boundingBox[0].y, v.position.y);
      boundingBox[0].z = std::min<float>(boundingBox[0].z, v.position.z);
      boundingBox[1].x = std::max<float>(boundingBox[1].x, v.position.x);
      boundingBox[1].y = std::max<float>(boundingBox[1].y, v.position.y);
      boundingBox[1].z = std::max<float>(boundingBox[1].z, v.position.z);
        
    }
    
}

void StaticMesh::Render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
    const DirectX::XMFLOAT4& materialColor,
    ID3D11PixelShader* alternative_pixel_shader)
{
    uint32_t stride{ sizeof(vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    // インデックスバッファオブジェクトの設定
    immediate_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(inputLayout.Get());

    immediate_context->VSSetShader(vertexShader.Get(), nullptr, 0);
    // 切替用のピクセルシェーダーがあったらそちらを設定
    alternative_pixel_shader ? immediate_context->PSSetShader(alternative_pixel_shader, nullptr, 0) : immediate_context->PSSetShader(pixelShader.Get(), nullptr, 0);

    //// テクスチャの情報をシェーダー側に設定
    //immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
    //
    //
    //// 定数バッファとして、ワールド行列とマテリアルカラーを設定
    //constants data{ world, materialColor };
    //immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
    //// 定数(コンスタント)バッファオブジェクトの設定
    //immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
    //
    //// インデックスを指定して描画
    //D3D11_BUFFER_DESC buffer_desc{};
    //index_buffer->GetDesc(&buffer_desc);
    //immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
    // マテリアル配列(ベクター)にあるマテリアルをすべて描画
    for (const material& mat : materials)
    {
        // テクスチャの情報をシェーダー側に設定
        immediate_context->PSSetShaderResources(0, 1, mat.shader_resource_views[0].GetAddressOf());
        // バンプマップ用のテクスチャをシェーダー側に設定
        immediate_context->PSSetShaderResources(1, 1, mat.shader_resource_views[1].GetAddressOf());
        // 定数バッファとして、ワールド行列とマテリアルカラーを設定
        constants data{ world, materialColor };
        // マテリアルカラーは読み込んだ色も反映
        XMStoreFloat4(&data.materialColor, XMLoadFloat4(&materialColor) * XMLoadFloat4(&mat.Kd));
        immediate_context->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
        // 定数(コンスタント)バッファオブジェクトの設定
        immediate_context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

        // サブセットを捜査
        for (const subset& s : subsets)
        {
            // サブセットにあるマテリアル名と一致するものをチェック
            if (mat.name == s.usemtl)
            {
                // 一致したサブセットのインデックスの数と開始番号をを指定
                immediate_context->DrawIndexed(s.indexCount, s.indexStart, 0);
            }
        }
    }
}


/// <summary>
/// 頂点バッファオブジェクトの作成
/// </summary>
/// <param name="device"></param>
/// <param name="vertices">頂点情報</param>
/// <param name="vertex_count">頂点数</param>
/// <param name="indices">インデックス情報</param>
/// <param name="index_count">インデックス数</param>
void StaticMesh::CreateComBuffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
    uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    // 頂点座標分のサイズを計算
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    // 頂点座標が設定されている配列を設定
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, indexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}