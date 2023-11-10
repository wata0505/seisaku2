#include "FullscreenQuad.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

Texture2D DiffuseTexture : register(t0);
SamplerState sampler_states[3] : register(s0);

float nrand(float x, float y)
{
	return frac(sin(dot(float2(x, y), float2(12.9898, 78.233))) * 43758.5453);
}

float4 main(VS_OUT pin) : SV_TARGET0
{
	float4 color = DiffuseTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	// 処理するなら
	if (jitterStrength > 0.0f)
	{
		// UV座標
		float u = pin.texcoord.x;
		float v = pin.texcoord.y;

		// 動的に値取得
		float2 scanLineJitter = float2(jitterStrength, jitterStrength);
		float2 colorDrift = float2(jitterStrength, time);

		// ScanLineJitter
		float jitter = nrand(v, time) * 2 - 1;
		jitter *= step(scanLineJitter.y, abs(jitter)) * scanLineJitter.x;

		// ColorDrift
		float drift = sin(v + colorDrift.y) * colorDrift.x;

		// 色収差
		color.rb = DiffuseTexture.Sample(sampler_states[ANISOTROPIC], frac(float2(u + jitter, v))).rb;
		color.g = DiffuseTexture.Sample(sampler_states[ANISOTROPIC], frac(float2(u + jitter + drift, v))).g;
	}
	
	return color;
}