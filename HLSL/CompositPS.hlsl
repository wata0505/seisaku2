#include "Light.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D LightTexture : register(t2);
SamplerState sampler_states[3] : register(s0);

float4 main(VS_OUT pin) : SV_TARGET0
{
	float4 tex = DiffuseTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	float4 light = LightTexture.Sample(sampler_states[ANISOTROPIC],pin.texcoord);
	float4 N = NormalTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	float dist = length(N.xyz * 2.0 - 1.0);
	float n = step(0.5, dist);

	float4 color = tex * lerp(1.0, light, n);
	//color.rgb *= float3(0.2, 0.2, 0.9);
	//color = 1;
	return color;
}