#include"SwordTrail.h"
#include "Graphics.h"
#include "texture.h"
#include "misc.h"


SwordTrail::SwordTrail(int bufferSize, const wchar_t* filename) {
    posArray.resize(bufferSize);
    SwordTrailVertex stvex[52];
    //vertex.resize(0);
    ID3D11Device* device = Graphics::Instance().GetDevice();

    HRESULT hr = S_OK;
    // 頂点バッファの生成
    {
        // 頂点バッファを作成するための設定オプション
        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.ByteWidth = sizeof(stvex);	// バッファ（データを格納する入れ物）のサイズ
        buffer_desc.Usage = D3D11_USAGE_DYNAMIC;	// UNIT.03
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてバッファを作成する。
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// UNIT.03
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        // 頂点バッファに頂点データを入れるための設定
        D3D11_SUBRESOURCE_DATA subresource_data = {};
        subresource_data.pSysMem = &vertex;	// ここに格納したい頂点データのアドレスを渡すことでCreateBuffer()時にデータを入れることができる。
        subresource_data.SysMemPitch = 0; //Not use for vertex buffers.
        subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.
        // 頂点バッファオブジェクトの生成
        hr = device->CreateBuffer(&buffer_desc, &subresource_data, &vertexBuffer);
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }

    {// ダミーテクスチャの生成
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

        hr = device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }
    //if (filename) {
    //    D3D11_TEXTURE2D_DESC	texture2dDesc;
    //    load_texture_from_file(device, filename, shaderResourceView.GetAddressOf(), &texture2dDesc);
    //}
}

void SwordTrail::Update()
{
    //データを更新
    for (size_t i = posArray.size() - 1; i > 0; --i)
    {
        posArray[i] = posArray[i - 1];

    }
    posArray.front() = tempPos;
    tempPos = PosBuffer();

    //曲線を作る
    std::vector<PosBuffer> usedPosArray = posArray;
    if (usedPosArray.empty())return;
    CreateCurveVertex(usedPosArray);

    //頂点データを更新する

    float amount = 1.0f / (usedPosArray.size() - 1);
    float v = 0;
    vertex.clear();
    vertex.resize(usedPosArray.size() * 2);
    for (size_t i = 0, j = 0; i < vertex.size() && j < usedPosArray.size(); i += 2, ++j)
    {
        vertex[i].position = usedPosArray[j].headPos;
        vertex[i].texcoord = XMFLOAT2(1.0f, v);
        vertex[i + 1].position = usedPosArray[j].tailPos;
        vertex[i + 1].texcoord = XMFLOAT2(0.0f, v);
        v += amount;
    }
}
void SwordTrail::SetPos(XMFLOAT3& head, XMFLOAT3& tail)
{
    tempPos.headPos = head;
    tempPos.tailPos = tail;
    //tempPos.isUsed = true;
}

void SwordTrail::CreateCurveVertex(std::vector<PosBuffer>& usedPosArray)
{
    if (usedPosArray.size() < 3 || split < 1) { return; }
    std::vector<PosBuffer> newPosArray;
    newPosArray.reserve(usedPosArray.size() + (usedPosArray.size() - 1) * split);
    const float amount = 1.0f / (split + 1);

    PosBuffer newPos;
    newPosArray.push_back(usedPosArray.front());
    for (size_t i = 0; i < usedPosArray.size() - 1; ++i)
    {
        float ratio = amount;
        //  CatMulに使う4つの点を作る（p0, p3がない時の処理も書く）
        XMVECTOR p0Head = i == 0 ? (XMLoadFloat3(&usedPosArray[1].headPos) + XMLoadFloat3(&usedPosArray[2].headPos)) * 0.5f : XMLoadFloat3(&usedPosArray[i - 1].headPos);
        XMVECTOR p1Head = XMLoadFloat3(&usedPosArray[i].headPos);
        XMVECTOR p2Head = XMLoadFloat3(&usedPosArray[i + 1].headPos);
        XMVECTOR p3Head = i == usedPosArray.size() - 2 ? (p0Head + p2Head) * 0.5f : XMLoadFloat3(&usedPosArray[i + 2].headPos);

        XMVECTOR p0Tail = i == 0 ? (XMLoadFloat3(&usedPosArray[1].tailPos) + XMLoadFloat3(&usedPosArray[2].tailPos)) * 0.5f : XMLoadFloat3(&usedPosArray[i - 1].tailPos);
        XMVECTOR p1Tail = XMLoadFloat3(&usedPosArray[i].tailPos);
        XMVECTOR p2Tail = XMLoadFloat3(&usedPosArray[i + 1].tailPos);
        XMVECTOR p3Tail = i == usedPosArray.size() - 2 ? (p0Tail + p2Tail) * 0.5f : XMLoadFloat3(&usedPosArray[i + 2].tailPos);

        for (size_t j = 0; j < static_cast<size_t>(split - 1); ++j)
        {
            newPos = PosBuffer();

            //newPos.isUsed = true;
            XMStoreFloat3(&newPos.headPos, XMVectorCatmullRom(p0Head, p1Head, p2Head, p3Head, ratio));
            XMStoreFloat3(&newPos.tailPos, XMVectorCatmullRom(p0Tail, p1Tail, p2Tail, p3Tail, ratio));

            newPosArray.push_back(newPos);
            ratio += amount;
        }
        newPosArray.push_back(usedPosArray[i + 1]);
    }
    usedPosArray = newPosArray;
}
//DirectX::XMFLOAT3 SwordTrail::GetTrailDir() {
//    //DirectX::XMFLOAT3 farst = posArray[0].tailPos;
//    //DirectX::XMFLOAT3 second = posArray[10].tailPos;
//    //DirectX::XMFLOAT3 dir = Vector::subset(farst, second);
//    //dir = Vector::Normalize(dir);
//    //return dir;
//    return {};
//}