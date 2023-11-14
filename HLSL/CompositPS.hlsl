#include "Light.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D LightTexture : register(t2);
Texture2D DepthTexture : register(t3);
SamplerState sampler_states[3] : register(s0);

// アウトライン
float Outline(float2 uv, Texture2D Depth, Texture2D Normal, SamplerState Decal)
{
	float depth = Depth.Sample(Decal, uv).r;
	float2 error1 = float2(0.001f, 0.0f);
	float2 error2 = float2(0.0f, 0.0015f);
	float dR = Depth.Sample(Decal, uv + error1).r;
	float dD = Depth.Sample(Decal, uv + error2).r;

	float d = abs(depth - dR);
	float d2 = abs(depth - dD);
	//if (d > 0.3 * depth) return 0;
	//if (d2 > 0.3 * depth) return 0;
	//法線に差がある→輪郭
	float3 N = Normal.Sample(Decal, uv).xyz;
	float3 NR = Normal.Sample(Decal, uv + error1).xyz;
	float3 ND = Normal.Sample(Decal, uv + error2).xyz;
	N = N * 2.0 - 1.0; //0-1 => -1 - +1
	NR = NR * 2.0 - 1.0;
	ND = ND * 2.0 - 1.0;
	if (dot(N, NR) < 0.1) return 0;
	if (dot(N, ND) < 0.1) return 0;
	return 1;
}

float4 main(VS_OUT pin) : SV_TARGET0
{
	float4 tex = DiffuseTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	float4 light = LightTexture.Sample(sampler_states[ANISOTROPIC],pin.texcoord);
	float4 N = NormalTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	float dist = length(N.xyz * 2.0 - 1.0);
	float n = step(0.5, dist);

	float4 color = tex * lerp(1.0, light, n);
	//アウトライン
	float rate = Outline(pin.texcoord, DepthTexture, NormalTexture, sampler_states[ANISOTROPIC]);
	color.rgb = lerp(float3(1, 1, 0), color.rgb, rate);
	//color.rgb *= float3(0.2, 0.2, 0.9);
	//color = 1;
	return color;
}