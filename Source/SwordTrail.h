#pragma once
#include<d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <string>

using namespace DirectX;

//剣の位置を保存する構造体
struct PosBuffer
{
    XMFLOAT3 headPos; //剣の先端の位置
    XMFLOAT3 tailPos; //剣の末端の位置
};

//頂点バッファ
struct SwordTrailVertex
{
    XMFLOAT3 position = { 0, 0, 0 };
    XMFLOAT2 texcoord = { 0, 0 };
};

class SwordTrail
{
private:
    
    PosBuffer tempPos;                           //現在フレームでの剣の位置
    Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
public:
    SwordTrail(int bufferSize, const wchar_t* filename);                   //コンストラクタでposArrayのサイズを決める
    void Update();                                //履歴を更新して、頂点バッファを更新する
    void SetPos(XMFLOAT3& head, XMFLOAT3& tail);  //現在フレームの剣の位置を保存する
    void CreateCurveVertex(std::vector<PosBuffer>& usedPosArray);
    int split = 5;
     //何フレーム前軌跡の位置
     DirectX::XMFLOAT3 GetTrail(int i) { return posArray[i].tailPos; };

     DirectX::XMFLOAT3 GetTrailVertex(int i) { return vertex[i].position; };

    std::vector<PosBuffer> posArray;             //剣の位置を保存するバッファ
    std::vector<SwordTrailVertex> vertex;        //頂点バッファ

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }
    const std::vector<SwordTrailVertex>& GetVertex() { return vertex; };
    // 頂点バッファの取得
    const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }

    

};