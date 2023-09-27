#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
/// <summary>
/// 幾何プリミティブ
/// </summary>
class GeometricPrimitive
{
public:
    // 頂点フォーマット
    struct vertex
    {
        // 位置
        DirectX::XMFLOAT3 position;
        // 法線
        DirectX::XMFLOAT3 normal;
    };
    // 定数バッファ用フォーマット
    struct constants
    {
        // ワールド行列
        DirectX::XMFLOAT4X4 world;
        // マテリアルカラー
        DirectX::XMFLOAT4 materialColor;
    };

private:
    // 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    // インデックスバッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    // 頂点シェーダーオブジェクト
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    // ピクセルシェーダーオブジェクト
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    // 入力レイアウトオブジェクト
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    // 定数バッファオブジェクト
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

public:
  //  GeometricPrimitive(ID3D11Device* device);
    virtual ~GeometricPrimitive() = default;

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="immediate_context">デバイスコンテキスト</param>
    /// <param name="world">ワールド行列</param>
    /// <param name="materialColor">マテリアルカラー</param>
    void render(ID3D11DeviceContext* immediate_context,
        const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& materialColor);

protected:
    GeometricPrimitive(ID3D11Device* device);
    /// <summary>
    /// 頂点バッファオブジェクトの作成
    /// </summary>
    /// <param name="device">デバイス</param>
    /// <param name="vertices">頂点情報</param>
    /// <param name="vertex_count">頂点数</param>
    /// <param name="indices">インデックス情報</param>
    /// <param name="index_count">インデックス数</param>
    void CreateComBuffer(ID3D11Device* device, vertex* vertices, size_t vertex_count,
        uint32_t* indices, size_t index_count);
};

/// <summary>
///　正立方体専用幾何プリミティブ
/// </summary>
class GeometricCube : public GeometricPrimitive
{
public:
    GeometricCube(ID3D11Device* device);

};

/// <summary>
/// シリンダー
/// </summary>
class GeometricCylinder : public GeometricPrimitive
{
public:
    // 第２引数はシリンダーの円を何分割するかの数を指定
    GeometricCylinder(ID3D11Device* device, uint32_t slices);
};
class GeometricCapsule : public GeometricPrimitive
{
public:
    GeometricCapsule(
        ID3D11Device* device,
        float mantle_height,
        const DirectX::XMFLOAT3& radius,
        uint32_t slices,
        uint32_t ellipsoid_stacks,
        uint32_t mantle_stacks);
};