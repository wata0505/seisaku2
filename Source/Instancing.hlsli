
// VSシェーダの入力データ定義
struct VS_INPUT {
	float4 Position : SV_POSITION;
	float2 Tex  : TEXTURE;
	float2 Texcoord : TEXCOORD;
	float2 Dissolve :DISSOLVE;
	float  Timer : TIMER;
};

// GSシェーダの入力データ定義
struct GS_INPUT {
	float4 Pos  : SV_POSITION;     // 頂点座標(射影座標系)
	float4 Color: COLOR;
	float4 Size : SIZE;  // パーティクルの大きさ
	float4 Dis  : DIS;         // テクスチャ座標
	float   Mer  : MER;
};
cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 View;       // ビュー変換行列
	float4x4 Projection; // 透視変換行列
};
#define MAX_INSTANCES 512
cbuffer OBJECT_CONSTANT_BUFFER : register(b1)
{
	float4 World[MAX_INSTANCES];
	float4 MaterialColor[MAX_INSTANCES];
	float4 SpriteSize[MAX_INSTANCES];
	float4 Dissolve[MAX_INSTANCES];
	float4 Timer[MAX_INSTANCES];
};

// ピクセル・シェーダの入力データ定義
struct PS_INPUT {
	float4 Pos  : SV_POSITION;     // 頂点座標(射影座標系)
	float4 MaterialColor : MATERIALCOLOR;
	float2 Tex  : TEXTURE;         // テクスチャ座標
	float2 Color: COLOR;
	float4 Dissolve: DISSOLVE;

};