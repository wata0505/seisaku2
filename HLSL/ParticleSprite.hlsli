#define SLASHING 0
#define DIFFUSION 1
#define CONVERGENCE 2
#define SLASH 3
#define EXPANSION 4
#define WEPONCONVERGENCE 5
#define CORRUPTION 6
// VSシェーダの入力データ定義
struct GS_INPUT_GPU2 {
	float3 Position    : POSITION;     // 頂点座標(ワールド座標系)
	float3 Velocity    : VELOCITY;       // 現在の速度ベクトル(ワールド座標系)
	float4 ParticleSize: PARTICLESIZE;
	float  Speed       : SPEED;       // 現在の加速度
	uint   ParticleType: PARTICLETYPE;
	float  Timer       : TIMER;
};

// GSシェーダの入力データ定義
struct GS_INPUT {
	float4 Pos  : SV_POSITION;     // 頂点座標(射影座標系)
	float3 Dir  : DIRECTION;
	float2 Tex  : TEXTURE;         // テクスチャ座標
	float4 Size : PARTICLESIZE;  // パーティクルの大きさ
	uint   Type : TYPE;
	float  Mer  : MER;
};
cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 View;       // ビュー変換行列
	float4x4 Projection; // 透視変換行列
};
cbuffer CALCULATION_BUFFER : register(b1)
{

	float3 Pattern; // ビュー変換行列
	float3 Tail;  // 透視変換行列
	float  Max;
	float3 UvStatus;
};
cbuffer COLOR_BUFFER : register(b2)
{
	float4 MaterialColor; // ビュー変換行列
};
// ピクセル・シェーダの入力データ定義
struct PS_INPUT {
	float4 Pos  : SV_POSITION;     // 頂点座標(射影座標系)
	float2 Tex  : TEXTURE;         // テクスチャ座標
	float2 Color: COLOR;
	float3 wPosition : TEXCOORD2;
	float  uvTimer : UVTIMER;
};

struct PS_OUT {
	float4 Color : SV_TARGET0; // カラー
	float4 Depth : SV_TARGET1; // 距離
	float4 Normal : SV_TARGET2; // 法線
	float4 Position : SV_TARGET3; // 座標
};