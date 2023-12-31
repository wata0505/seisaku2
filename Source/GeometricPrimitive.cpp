#include "GeometricPrimitive.h"
#include "Shader.h"
#include "misc.h"
#include <vector>
#include<algorithm>



/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="device">デバイス</param>
GeometricPrimitive::GeometricPrimitive(ID3D11Device* device)
{
    /// <summary>
    /// サイズが 1.0 の正立方体データを作成する（重心を原点にする）。
    /// 正立方体のコントロールポイント数は 8 個、
    ///  1 つのコントロールポイントの位置には法線の向きが違う頂点が 3 個あるので頂点情報の総数は 8x3=24 個、
    /// 頂点情報配列（vertices）にすべて頂点の位置・法線情報を格納する。
    /// </summary>
    vertex vertices[24]{};

    /// <summary>
    ///  正立方体は 6 面持ち、1 つの面は 2 つの 3 角形ポリゴンで構成されるので
    /// 3 角形ポリゴンの総数は 6x2=12 個、
    /// 正立方体を描画するために 12 回の 3 角形ポリゴン描画が必要、よって参照される頂点情報は 12x3=36 回、
    ///  3 角形ポリゴンが参照する頂点情報のインデックス（頂点番号）を描画順に配列（indices）に格納する。
    ///  時計回りが表面になるように格納すること。
    /// </summary>
    uint32_t indices[36]{};

    // 上面
    // 0---------1
    // |         |
    // |   -Y    |
    // |         |
    // 2---------3
    uint32_t face = 0;
    vertices[face * 4 + 0].position = { -0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 1].position = { +0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 2].position = { -0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 3].position = { +0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 0].normal = { +0.0f, +1.0f, +0.0f };
    vertices[face * 4 + 1].normal = { +0.0f, +1.0f, +0.0f };
    vertices[face * 4 + 2].normal = { +0.0f, +1.0f, +0.0f };
    vertices[face * 4 + 3].normal = { +0.0f, +1.0f, +0.0f };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 1;
    indices[face * 6 + 2] = face * 4 + 2;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 3;
    indices[face * 6 + 5] = face * 4 + 2;

    // 下面
    // 0---------1
    // |         |
    // |   -Y    |
    // |         |
    // 2---------3
    face += 1;
    vertices[face * 4 + 0].position = { -0.5f, -0.5f, +0.5f };
    vertices[face * 4 + 1].position = { +0.5f, -0.5f, +0.5f };
    vertices[face * 4 + 2].position = { -0.5f, -0.5f, -0.5f };
    vertices[face * 4 + 3].position = { +0.5f, -0.5f, -0.5f };
    vertices[face * 4 + 0].normal = { +0.0f, -1.0f, +0.0f };
    vertices[face * 4 + 1].normal = { +0.0f, -1.0f, +0.0f };
    vertices[face * 4 + 2].normal = { +0.0f, -1.0f, +0.0f };
    vertices[face * 4 + 3].normal = { +0.0f, -1.0f, +0.0f };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 2;
    indices[face * 6 + 2] = face * 4 + 1;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 2;
    indices[face * 6 + 5] = face * 4 + 3;

    // 前面
    // 0---------1
    // |         |
    // |   +Z    |
    // |         |
    // 2---------3
    face += 1;
    vertices[face * 4 + 0].position = { -0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 1].position = { +0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 2].position = { -0.5f, -0.5f, -0.5f };
    vertices[face * 4 + 3].position = { +0.5f, -0.5f, -0.5f };
    vertices[face * 4 + 0].normal = { +0.0f, +0.0f, -1.0f };
    vertices[face * 4 + 1].normal = { +0.0f, +0.0f, -1.0f };
    vertices[face * 4 + 2].normal = { +0.0f, +0.0f, -1.0f };
    vertices[face * 4 + 3].normal = { +0.0f, +0.0f, -1.0f };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 1;
    indices[face * 6 + 2] = face * 4 + 2;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 3;
    indices[face * 6 + 5] = face * 4 + 2;

    // 後面
    // 0---------1
    // |         |
    // |   +Z    |
    // |         |
    // 2---------3
    face += 1;
    vertices[face * 4 + 0].position = { -0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 1].position = { +0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 2].position = { -0.5f, -0.5f, +0.5f };
    vertices[face * 4 + 3].position = { +0.5f, -0.5f, +0.5f };
    vertices[face * 4 + 0].normal = { +0.0f, +0.0f, +1.0f };
    vertices[face * 4 + 1].normal = { +0.0f, +0.0f, +1.0f };
    vertices[face * 4 + 2].normal = { +0.0f, +0.0f, +1.0f };
    vertices[face * 4 + 3].normal = { +0.0f, +0.0f, +1.0f };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 2;
    indices[face * 6 + 2] = face * 4 + 1;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 2;
    indices[face * 6 + 5] = face * 4 + 3;

    // 右面
    // 0---------1
    // |         |      
    // |   -X    |
    // |         |
    // 2---------3
    face += 1;
    vertices[face * 4 + 0].position = { +0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 1].position = { +0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 2].position = { +0.5f, -0.5f, -0.5f };
    vertices[face * 4 + 3].position = { +0.5f, -0.5f, +0.5f };
    vertices[face * 4 + 0].normal = { +1.0f, +0.0f, +0.0f };
    vertices[face * 4 + 1].normal = { +1.0f, +0.0f, +0.0f };
    vertices[face * 4 + 2].normal = { +1.0f, +0.0f, +0.0f };
    vertices[face * 4 + 3].normal = { +1.0f, +0.0f, +0.0f };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 1;
    indices[face * 6 + 2] = face * 4 + 2;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 3;
    indices[face * 6 + 5] = face * 4 + 2;

    // 左面
    // 0---------1
    // |         |      
    // |   -X    |
    // |         |
    // 2---------3
    face += 1;
    vertices[face * 4 + 0].position = { -0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 1].position = { -0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 2].position = { -0.5f, -0.5f, -0.5f };
    vertices[face * 4 + 3].position = { -0.5f, -0.5f, +0.5f };
    vertices[face * 4 + 0].normal = { -1.0f, +0.0f, +0.0f };
    vertices[face * 4 + 1].normal = { -1.0f, +0.0f, +0.0f };
    vertices[face * 4 + 2].normal = { -1.0f, +0.0f, +0.0f };
    vertices[face * 4 + 3].normal = { -1.0f, +0.0f, +0.0f };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 2;
    indices[face * 6 + 2] = face * 4 + 1;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 2;
    indices[face * 6 + 5] = face * 4 + 3;

    // 頂点データとインデックスデータを使って頂点バッファオブジェクトを作成します
    CreateComBuffer(device, vertices, 24, indices, 36);
    //create_com_buffer(device, vertices, 4, indices, 6);

    HRESULT hr{ S_OK };

    // 入力レイアウトオブジェクトの設定
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // 頂点シェーダーオブジェクトの作成(入力レイアウトもこの中で作成しています。)
    create_vs_from_cso(device, "GeometricPrimitiveVS.cso", vertexShader.GetAddressOf(),
          inputLayout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    // ピクセルシェーダーオブジェクトの作成
    create_ps_from_cso(device, "GeometricPrimitivePS.cso", pixelShader.GetAddressOf());

    // 定数(コンスタント)バッファの作成
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="immediate_context">デバイスコンテキスト</param>
/// <param name="world">ワールド行列</param>
/// <param name="materialColor">マテリアルカラー</param>
void GeometricPrimitive::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
    const DirectX::XMFLOAT4& materialColor)
{
    uint32_t stride{ sizeof(vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    // インデックスバッファオブジェクトの設定
    immediate_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(inputLayout.Get());

    immediate_context->VSSetShader(vertexShader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixelShader.Get(), nullptr, 0);

    // 定数バッファとして、ワールド行列とマテリアルカラーを設定
    constants data{ world, materialColor };
    immediate_context->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
    // 定数(コンスタント)バッファオブジェクトの設定
    immediate_context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    // インデックスを指定して描画
    D3D11_BUFFER_DESC buffer_desc{};
    indexBuffer->GetDesc(&buffer_desc);
    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
}


/// <summary>
/// 頂点バッファオブジェクトの作成
/// </summary>
/// <param name="device"></param>
/// <param name="vertices">頂点情報</param>
/// <param name="vertex_count">頂点数</param>
/// <param name="indices">インデックス情報</param>
/// <param name="index_count">インデックス数</param>
void GeometricPrimitive::CreateComBuffer(ID3D11Device* device, vertex* vertices, size_t vertex_count,
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

GeometricCube::GeometricCube(ID3D11Device* device) : GeometricPrimitive(device)
 {
    // create a mesh for a cube
    vertex vertices[24]{};
    uint32_t indices[36]{};

    uint32_t face{ 0 };

     // top-side
     // 0---------1
     // |         |
     // |   -Y    |
     // |         |
     // 2---------3
    face = 0;
    vertices[face * 4 + 0].position = { -0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 1].position = { +0.5f, +0.5f, +0.5f };
    vertices[face * 4 + 2].position = { -0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 3].position = { +0.5f, +0.5f, -0.5f };
    vertices[face * 4 + 0].normal = { +0.0f, +1.0f, +0.0f };
    vertices[face * 4 + 1].normal = { +0.0f, +1.0f, +0.0f };
    vertices[face * 4 + 2].normal = { +0.0f, +1.0f, +0.0f };
    vertices[face * 4 + 3].normal = { +0.0f, +1.0f, +0.0f };
    indices[face * 6 + 0] = face * 4 + 0;
    indices[face * 6 + 1] = face * 4 + 1;
    indices[face * 6 + 2] = face * 4 + 2;
    indices[face * 6 + 3] = face * 4 + 1;
    indices[face * 6 + 4] = face * 4 + 3;
    indices[face * 6 + 5] = face * 4 + 2;

    CreateComBuffer(device, vertices, 24, indices, 36);
    
}
GeometricCylinder::GeometricCylinder(ID3D11Device* device, uint32_t slices) : GeometricPrimitive(device)
{
    //std::vector<vertex> vertices;
    //std::vector<uint32_t> indices;

    // 分割数(円を作るための三角形の数、６個の三角形)
    //slices = 6;
    // 半径
    float radius = 0.5f;

    // 頂点データ
    vertex* vertices = new vertex[(slices + 1) * 2 + slices * 2];
    // 頂点インデックス
    uint32_t* indices = new uint32_t[(slices * 3) * 2 + (3 * 2 * slices)];

    // 三角形の角度 (デグリー角)
    float d{ 2.0f * DirectX::XM_PI / slices };
    // 半径
    float r{ 0.5f };

    // 真ん中の頂点座標と法線
    vertices[0].position = { 0.0f, 0.5f, 0.0f };
    vertices[0].normal = { 0.0f, 1.0f, 0.0f };

    // 円の周りの頂点座標と法線を設定
    for (int i = 0; i < slices; ++i) {

        // 角度から頂点座標を計算
        vertices[1 + i].position.x = radius * cosf(i * d);
        vertices[1 + i].position.y = 0.5f;
        vertices[1 + i].position.z = radius * sinf(i * d);
        // 法線を設定
        vertices[1 + i].normal = { 0.0f, 1.0f, 0.0f };
    }

    for (int i = 0; i < slices - 1; ++i)
    {
        indices[(i * 3) + 0] = 0;
        indices[(i * 3) + 1] = i + 2;
        indices[(i * 3) + 2] = i + 1;
    }

    indices[slices * 3 - 3] = 0;
    indices[slices * 3 - 2] = 1;
    indices[slices * 3 - 1] = slices;

    // 下の円
    int base_slices = slices + 1;

    // 下の円の真ん中の頂点座標と法線
    vertices[base_slices].position = { 0.0f, -0.5f, 0.0f };
    vertices[base_slices].normal = { 0.0f, -1.0f, 0.0f };

    // 円の周りの頂点座標と法線を設定
    for (int i = 0; i < slices; ++i)
    {
        // 角度から頂点座標を計算
        vertices[slices + 1 + 1 + i].position.x = radius * cosf(i * d);
        vertices[slices + 1 + 1 + i].position.y = -0.5f;
        vertices[slices + 1 + 1 + i].position.z = radius * sinf(i * d);
        // 法線を設定
        vertices[slices + 1 + 1 + i].normal = { 0.0f, -1.0f, 0.0f };
    }

    // 下の円のインデックスの設定
    int base_index = slices + 1;
    for (uint32_t i = 0; i < slices - 1; ++i)
    {
        indices[(slices * 3) + (i * 3) + 0] = base_index + 0;
        indices[(slices * 3) + (i * 3) + 1] = base_index + i + 1;
        indices[(slices * 3) + (i * 3) + 2] = base_index + i + 2;
    }

    indices[(slices * 3 * 2) - 1] = base_index + 1;
    indices[(slices * 3 * 2) - 2] = slices * 2 + 1;
    indices[(slices * 3 * 2) - 3] = base_index;


    // 側面
    for (int i = 0; i < slices; ++i)
    {
        float x = radius * cosf(i * d);
        float z = radius * sinf(i * d);

        // 上の点の座標と法線
        vertices[slices + 1 + slices + 1 + i * 2 + 0].position = { x, 0.5f , z };
        vertices[slices + 1 + slices + 1 + i * 2 + 0].normal = { x, 0.0f , z };

        // 下の点の座標と法線
        vertices[slices + 1 + slices + 1 + i * 2 + 1].position = { x, -0.5f , z };
        vertices[slices + 1 + slices + 1 + i * 2 + 1].normal = { x, 0.0f , z };
    }

    for (int i = 0; i < slices - 1; ++i)
    {
        // i 番目の四角形の１つ目の三角形
        indices[slices * 3 * 2 + i * 6 + 0] = slices + 1 + slices + 1 + i * 2 + 0;
        indices[slices * 3 * 2 + i * 6 + 1] = slices + 1 + slices + 1 + i * 2 + 2;
        indices[slices * 3 * 2 + i * 6 + 2] = slices + 1 + slices + 1 + i * 2 + 1;
        // i 番目の四角形の２つ目の三角形
        indices[slices * 3 * 2 + i * 6 + 3] = slices + 1 + slices + 1 + i * 2 + 1;
        indices[slices * 3 * 2 + i * 6 + 4] = slices + 1 + slices + 1 + i * 2 + 2;
        indices[slices * 3 * 2 + i * 6 + 5] = slices + 1 + slices + 1 + i * 2 + 3;
    }

    // ６つ目の四角形の１つ目の三角形
    indices[slices * 3 * 2 + (slices - 1) * 6 + 0] = slices + 1 + slices + 1 + slices - 1 + slices - 1;
    indices[slices * 3 * 2 + (slices - 1) * 6 + 1] = slices + 1 + slices + 1;
    indices[slices * 3 * 2 + (slices - 1) * 6 + 2] = slices + 1 + slices + 1 + slices - 1 + slices - 1 + 1;
    // ６つ目の四角形の２つ目の三角形
    indices[slices * 3 * 2 + (slices - 1) * 6 + 3] = slices + 1 + slices + 1 + slices - 1 + slices - 1 + 1;
    indices[slices * 3 * 2 + (slices - 1) * 6 + 4] = slices + 1 + slices + 1;
    indices[slices * 3 * 2 + (slices - 1) * 6 + 5] = slices + 1 + slices + 1 + 1;


    CreateComBuffer(device, vertices, (slices + 1) * 2 + slices * 2, indices, (slices * 3) * 2 + (3 * 2 * slices));

    delete[] vertices;
    delete[] indices;

}

GeometricCapsule::GeometricCapsule(ID3D11Device* device,
    float mantle_height,
    const DirectX::XMFLOAT3& radius,
    uint32_t slices,
    uint32_t ellipsoid_stacks,
    uint32_t mantle_stacks) : GeometricPrimitive(device)
{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;
    const int base_offset = 0;

    slices = std::max<uint32_t>(3u, slices);
    mantle_stacks = std::max<uint32_t>(1u, mantle_stacks);
    ellipsoid_stacks = std::max<uint32_t>(2u, ellipsoid_stacks);

    const float inv_slices = 1.0f / static_cast<float>(slices);
    const float inv_mantle_stacks = 1.0f / static_cast<float>(mantle_stacks);
    const float inv_ellipsoid_stacks = 1.0f / static_cast<float>(ellipsoid_stacks);

    const float pi_2{ 3.14159265358979f * 2.0f };
    const float pi_0_5{ 3.14159265358979f * 0.5f };
    const float angle_steps = inv_slices * pi_2;
    const float half_height = mantle_height * 0.5f;

    /* Generate mantle vertices */
    struct spherical {
        float radius, theta, phi;
    } point{ 1, 0, 0 };
    DirectX::XMFLOAT3 position, normal;
    DirectX::XMFLOAT2 texcoord;

    float angle = 0.0f;
    for (uint32_t u = 0; u <= slices; ++u)
    {
        /* Compute X- and Z coordinates */
        texcoord.x = sinf(angle);
        texcoord.y = cosf(angle);

        position.x = texcoord.x * radius.x;
        position.z = texcoord.y * radius.z;

        /* Compute normal vector */
        normal.x = texcoord.x;
        normal.y = 0;
        normal.z = texcoord.y;

        float magnitude = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x = normal.x / magnitude;
        normal.y = normal.y / magnitude;
        normal.z = normal.z / magnitude;

        /* Add top and bottom vertex */
        texcoord.x = static_cast<float>(slices - u) * inv_slices;

        for (uint32_t v = 0; v <= mantle_stacks; ++v)
        {
            texcoord.y = static_cast<float>(v) * inv_mantle_stacks;
//#if _HAS_CXX20
//          position.y = lerp(half_height, -half_height, texcoord.y);
//#else
          position.y = half_height * (1 - texcoord.y) + -half_height * texcoord.y;
//endif
            vertices.push_back({ position, normal });
        }

        /* Increase angle for the next iteration */
        angle += angle_steps;
    }

    /* Generate bottom and top cover vertices */
    const float cover_side[2] = { 1, -1 };
    uint32_t base_offset_ellipsoid[2] = { 0 };
    for (size_t i = 0; i < 2; ++i)
    {
        base_offset_ellipsoid[i] = static_cast<uint32_t>(vertices.size());

        for (uint32_t v = 0; v <= ellipsoid_stacks; ++v)
        {
            /* Compute theta of spherical coordinate */
            texcoord.y = static_cast<float>(v) * inv_ellipsoid_stacks;
            point.theta = texcoord.y * pi_0_5;

            for (uint32_t u = 0; u <= slices; ++u)
            {
                /* Compute phi of spherical coordinate */
                texcoord.x = static_cast<float>(u) * inv_slices;
                point.phi = texcoord.x * pi_2 * cover_side[i] + pi_0_5;

                /* Convert spherical coordinate into cartesian coordinate and set normal by coordinate */
                const float sin_theta = sinf(point.theta);
                position.x = point.radius * cosf(point.phi) * sin_theta;
                position.y = point.radius * sinf(point.phi) * sin_theta;
                position.z = point.radius * cosf(point.theta);

                std::swap(position.y, position.z);
                position.y *= cover_side[i];

                /* Get normal and move half-sphere */
                float magnitude = sqrtf(position.x * position.x + position.y * position.y + position.z * position.z);
                normal.x = position.x / magnitude;
                normal.y = position.y / magnitude;
                normal.z = position.z / magnitude;

                /* Transform coordiante with radius and height */
                position.x *= radius.x;
                position.y *= radius.y;
                position.z *= radius.z;
                position.y += half_height * cover_side[i];

                //TODO: texCoord wrong for bottom half-sphere!!!
                /* Add new vertex */
                vertices.push_back({ position, normal });
            }
        }
    }

    /* Generate indices for the mantle */
    int offset = base_offset;
    for (uint32_t u = 0; u < slices; ++u)
    {
        for (uint32_t v = 0; v < mantle_stacks; ++v)
        {
            auto i0 = v + 1 + mantle_stacks;
            auto i1 = v;
            auto i2 = v + 1;
            auto i3 = v + 2 + mantle_stacks;

            indices.emplace_back(i0 + offset);
            indices.emplace_back(i1 + offset);
            indices.emplace_back(i3 + offset);
            indices.emplace_back(i1 + offset);
            indices.emplace_back(i2 + offset);
            indices.emplace_back(i3 + offset);
        }
        offset += (1 + mantle_stacks);
    }

    /* Generate indices for the top and bottom */
    for (size_t i = 0; i < 2; ++i)
    {
        for (uint32_t v = 0; v < ellipsoid_stacks; ++v)
        {
            for (uint32_t u = 0; u < slices; ++u)
            {
                /* Compute indices for current face */
                auto i0 = v * (slices + 1) + u;
                auto i1 = v * (slices + 1) + (u + 1);

                auto i2 = (v + 1) * (slices + 1) + (u + 1);
                auto i3 = (v + 1) * (slices + 1) + u;

                /* Add new indices */
                indices.emplace_back(i0 + base_offset_ellipsoid[i]);
                indices.emplace_back(i1 + base_offset_ellipsoid[i]);
                indices.emplace_back(i3 + base_offset_ellipsoid[i]);
                indices.emplace_back(i1 + base_offset_ellipsoid[i]);
                indices.emplace_back(i2 + base_offset_ellipsoid[i]);
                indices.emplace_back(i3 + base_offset_ellipsoid[i]);
            }
        }
    }
    CreateComBuffer(device, vertices.data(), vertices.size(), indices.data(), indices.size());
}