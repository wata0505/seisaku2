#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Sprite.h"
#include "Model.h"
#include "SkinnedMesh.h"
#include "RenderContext.h"
#define MAX_STAGE 100
struct InstancingConstants
{
	DirectX::XMFLOAT4X4 world[MAX_STAGE];			// ワールド行列
	//DirectX::XMFLOAT4X4 boneTransforms[256];
};
//	ボーン最大数
static	constexpr	int	MaxBones = 128;

//	インスタンシング最大数
static	constexpr	int	InstancingMax = 512;

//	メッシュ及びアニメーション関係の情報
struct BufferData
{
	const SkinnedMeshResouurce::Mesh& mesh;				//	メッシュ情報
	DirectX::XMFLOAT4X4	boneTransforms[MaxBones];	//	ボーン行列
	int	instancingCount = 0;						//	インスタンシング描画数
	DirectX::XMFLOAT4X4	transform[InstancingMax];	//	姿勢行列

	BufferData(const SkinnedMeshResouurce::Mesh& mesh) : mesh(mesh) {}
};
struct UvConstants
{
	DirectX::XMFLOAT4 uvStatus;			// uvアニメーション
};


class ModelShader
{
public:
	enum InstanModel
	{
		Default,
		Instancing,
		Max
	};
	ModelShader() {}
	virtual ~ModelShader() {}

	// 描画開始
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;

	// 描画
	virtual void Draw(ID3D11DeviceContext* dc, const Model* model, std::vector<SkinnedMeshResouurce::constants> buffer, const DirectX::XMFLOAT4 uvstatus= { 0,0,0,0 }, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }) = 0;

	virtual void Draw(ID3D11DeviceContext* dc, const Model* model, InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })  = 0;

	// 描画終了
	virtual void End(ID3D11DeviceContext* context) = 0;
	// 必要なバッファの設定
	//virtual void SetBuffers(const RenderContext& rc, const BufferData& bufferData) = 0;
};

class SpriteShader
{
public:
	SpriteShader() {}
	virtual ~SpriteShader() {}

	// 描画開始
	virtual void Begin(const RenderContext& rc) = 0;

	// 描画
	virtual void Draw(const RenderContext& rc, const Sprite* sprite = nullptr) = 0;

	// 描画終了
	virtual void End(const RenderContext& rc) = 0;
};


HRESULT create_vs_from_cso(
    ID3D11Device* device,
    const char* cso_name,
    ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout,
    D3D11_INPUT_ELEMENT_DESC* input_element_desc,
    UINT num_elements
);
HRESULT create_ps_from_cso(
    ID3D11Device* device,
    const char* cso_name,
    ID3D11PixelShader** pixel_shader
);
HRESULT create_gs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11GeometryShader** geometry_shader);
HRESULT create_cs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11ComputeShader** comp_shader);

           