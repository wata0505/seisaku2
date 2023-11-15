#pragma once

#include <memory>
#include <wrl.h>
#include "Shader.h"

class DefaultShader : public ModelShader
{
public:
	DefaultShader(ID3D11Device* device);
	~DefaultShader() override {}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc)override;
	void Draw(ID3D11DeviceContext* dc, Model* model, const DirectX::XMFLOAT4 uvstatus = { 0,0,0,0 }, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })override;
	void Draw(ID3D11DeviceContext* dc, const Model* model, InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	void End(ID3D11DeviceContext* dc) override;	

private:
	// 輪郭線用の初期化処理
	//void InitializeOutline(ID3D11Device* device);

private:
	static const int MaxBones = 128;

	static const int MAX_BONES{ 256 };
	// 定数バッファフォーマット
	struct constants
	{
		DirectX::XMFLOAT4X4 world;			// ワールド行列
		DirectX::XMFLOAT4 materialColor;	// マテリアルカラー
		DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES]{ {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1} };
	};
	struct scene_constants
	{
		DirectX::XMFLOAT4X4 viewProjection;	// ビュー・プロジェクション変換行列
		DirectX::XMFLOAT4 lightDirection;		// ライトの向き
		DirectX::XMFLOAT4 cameraPosition;  //カメラ位置
	};
	struct ShadowConstants
	{
		DirectX::XMFLOAT4X4 shadowProjection;
	};
	// サブセット用定数バッファ
	struct SubsetConstantBuffer
	{
		float adjustMetalness = 0.0f;	// 金属度
		float adjustSmoothness = 0.0f;	// 粗さ
		float emissiveStrength = 0.0f;	// エミッシブ強度
		float timer = 0.0f;				// 更新時間
		float scanTiling = 20.0f;		// 解像度
		float scanSpeed = -2.0f;		// スクロール速度
		float scanBorder = 10.0f;		// 描画範囲
		float glowTiling = 1.0f;		// 解像度
		float glowSpeed = 2.0f;			// スクロール速度
		float glowBorder = 10.0f;		// 描画範囲
		float hologramBorder = 10.0f;	// 描画範囲
		float rimStrength = 3.0f;		// リムライト強度
		float glitchSpeed = 50.0f;		// スクロール速度
		float glitchIntensity = 1.0f;	// 強度
		float glitchScale = 50.0f;		// 振れ幅
		float dummy;
	};
#if 0
	// アウトライン用定数バッファ
	struct OutlineConstantBuffer
	{
		DirectX::XMFLOAT4X4		viewProjection;
		DirectX::XMFLOAT4		outlineColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		float					outlineSize = 0.1f;
		DirectX::XMFLOAT3		dummy;
	};
#endif
	Microsoft::WRL::ComPtr<ID3D11Buffer> shadowBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> sceneBuffers;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> uvBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> instanBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader[Max];
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader[Max];
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	
	Microsoft::WRL::ComPtr<ID3D11SamplerState>      shadowMapSamplerState;

	DirectX::XMFLOAT4	materialColor = {1,1,1,1};

	DirectX::XMFLOAT4X4 VP;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	outlineRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		outlineVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		outlinePixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		outlineInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			outlineConstantBuffer;
};