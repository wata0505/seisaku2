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
		float glitchIntensity = 1.0f;	// 強度
		float glitchScale = 50.0f;		// 振れ幅
		float timer = 0.0f;				// 更新時間
		float scanBorder = 10.0f;		// 描画範囲
		float glowBorder = 10.0f;		// 描画範囲
		float hologramBorder = 10.0f;	// 描画範囲
		DirectX::XMFLOAT3 hologramColor = { 1.0f, 0.0f, 0.0f }; // ホログラム色
		float maxHeight;				// 最高点
	};

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
};