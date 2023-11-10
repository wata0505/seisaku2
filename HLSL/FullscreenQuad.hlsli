struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    //in uint vertexid : VERTEXID
};

// ジッタードリフト定数バッファ
cbuffer JitterDriftConstantBuffer : register(b13)
{
	float jitterStrength;
	float time;
	float2 dummy;
};

// カラーグレーディング
float3 ColorGrading(float3 color)
{
	float contrast = 1.3; //コントラスト
	float saturation = 0.7; // 彩度
	float3 colorlv = float3(0.9, 1.0, 1.05);

	// コントラスト(明暗の差)
	color = (color - 0.5) * contrast + 0.5;
	// 彩度
	float gray = (color.r + color.g + color.b) / 3.0;
	color = (color - gray) * saturation + gray;

	// 色合い
	color *= colorlv.rgb;
	return color;
};

// ビネット
float3 Vignette(float3 color, float2 position)
{
	float vignette = 1.0;
	// 中心からの距離
	float rate = length(position - 0.5);
	rate *= 1.5; // 全体調整
	rate += (vignette - 1) * 0.5 - 0.1; // 黒範囲調整
	rate = pow(rate, 3.0); // 黒→白の幅調整
	color = lerp(color, float3(0, 0, 0), saturate(rate));
	return color;
};

