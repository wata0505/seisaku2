#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;	// 向き
	DirectX::XMFLOAT4 color;		// 色
};

// 色調補正情報
struct ColorGradingData
{
	float hueShift = 0;		// 色相調整
	float saturation = 1;	// 彩度調整
	float brightness = 1;	// 明度調整
	float dummy;
};

// ガウスフィルター計算情報
struct GaussianFilterData
{
	int					KernelSize = 8;		// カーネルサイズ
	float				deviation = 10.0f;	// 標準偏差
	DirectX::XMFLOAT2	textureSize;		// ぼかすテクスチャのサイズ
};

// ガウスフィルターの最大カーネルサイズ
static const int MaxKernelSize = 16;

// 高輝度抽出情報
struct LuminanceExtractionData
{
	float threshold = 0.5f;
	float intensity = 1.0f;
	DirectX::XMFLOAT2 dummy2;
};

// ジッタードリフト情報
struct JitterDriftData
{
	float jitterStrength = 0.0f;
	float time = 0.0f;
	DirectX::XMFLOAT2 dummy;
};

// ポストエフェクトの最終パス用情報
struct FinalpassData
{
	// ブルームテクスチャ
	ID3D11ShaderResourceView* bloomTexture;
};
// シャドウマップ用情報
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap; // シャドウマップテクスチャ
	DirectX::XMFLOAT4X4 lightViewProjection; // ライトビュープロジェクション行列
	DirectX::XMFLOAT3 shadowColor; // 影の色
	float shadowBias; // 深度比較用のオフセット値
};
struct FogData
{
	DirectX::XMFLOAT4 fog_color;
	DirectX::XMFLOAT4 fog_range;	// x : near, y : far, zw : 空き
};


// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4		CameraPosition;
	// ライト情報
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData directionalLightData;

	// 色調補正情報
	ColorGradingData colorGradingData;
	// シャドウマップ情報
	ShadowMapData shadowMapData;
	//フォグ
	FogData fogData;

	FinalpassData finalpassData;

	LuminanceExtractionData luminanceExtractionData;

	JitterDriftData jitterDriftData;

	DirectX::XMFLOAT4 color;
};