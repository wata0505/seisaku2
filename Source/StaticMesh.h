#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <string>
#include <vector>

/// <summary>
/// スタティックメッシュ
/// </summary>
class StaticMesh
{
public:
    /// <summary>
    /// 頂点フォーマット
    /// </summary>
    struct vertex
    {
        DirectX::XMFLOAT3 position;	// 座標
        DirectX::XMFLOAT3 normal;	// 法線
        DirectX::XMFLOAT2 texcoord; //テクスチャ座標
    };

    /// <summary>
    /// 定数バッファ用フォーマット
    /// </summary>
    struct constants
    {
        DirectX::XMFLOAT4X4 world;			// ワールド行列
        DirectX::XMFLOAT4 materialColor;	// マテリアルカラー
    };

   
    
    ///<summary>
    ///マテリアル名とDrawIndex()に必要な情報部分をサブセット化
    ///</summary>
    struct subset
    {
        std::wstring usemtl;       //マテリアル名
        uint32_t indexStart{ 0 }; //インデックスの開始位置
        uint32_t indexCount{ 0 }; //インデックスの数（頂点数）
    };
    std::vector<subset>subsets;
   

   // //テクスチャファイル名
   // std::wstring texture_filename;
   // //テクスチャ情報
   // Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;

    //マテリアル
    struct material
    {
        //マテリアル名
        std::wstring name;
        DirectX::XMFLOAT4 Ka{ 0.2f,0.2f,0.2f,1.0f };
        DirectX::XMFLOAT4 Kd{ 0.8f,0.8f,0.8f,1.0f };
        DirectX::XMFLOAT4 Ks{ 1.0f,1.0f,1.0f,1.0f };
        ////テクスチャファイル名
        //std::wstring texture_filename;
        ////テクスチャの情報
        //Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
        //テクスチャファイル名
        std::wstring textureFilenames[2];
        //テクスチャの情報
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_views[2];
    };
    //読み込んだマテリアル
    std::vector<material>materials;

    DirectX::XMFLOAT3 boundingBox[2]{
        { D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX },
        { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
    };

private:
    // 頂点バッファオブジェクト
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    // インデックスバッファオブジェクト
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
    StaticMesh(ID3D11Device* device, const wchar_t* obj_filename, bool flipping_v_coordinates);
    virtual ~StaticMesh() = default;


    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="immediate_context">デバイスコンテキスト</param>
    /// <param name="world">親のワールド行列</param>
    /// <param name="materialColor">マテリアルカラー</param>
    /// /// <param name="alternative_pixel_shader">切替用ピクセルシェーダー</param>
    void Render(ID3D11DeviceContext* immediate_context,
        const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& materialColor,
        ID3D11PixelShader* alternative_pixel_shader = nullptr);

protected:
    /// <summary>
    /// 頂点バッファオブジェクトの作成
    /// </summary>
    /// <param name="device"></param>
    /// <param name="vertices">頂点情報</param>
    /// <param name="vertex_count">頂点数</param>
    /// <param name="indices">インデックス情報</param>
    /// <param name="index_count">インデックス数</param>
    void CreateComBuffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
        uint32_t* indices, size_t index_count);
};