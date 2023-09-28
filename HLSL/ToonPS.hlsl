#include "Toon.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps : register(t0);
Texture2D color_maps : register(t2);
Texture2D dissolve_map : register(t5);
Texture2D nomal_maps : register(t8);
Texture2D posison_maps : register(t9);
PS_OUT main(VS_OUT pin) : SV_TARGET
{

	 float2 uv = pin.texcoord + uvStatus.xx;
	 float4 color = texture_maps.Sample(sampler_states[ANISOTROPIC], uv);
	 float4 dissolves = dissolve_map.Sample(sampler_states[ANISOTROPIC], uv);
	 float4 n = nomal_maps.Sample(sampler_states[ANISOTROPIC], uv);
	 float4 p = posison_maps.Sample(sampler_states[ANISOTROPIC], uv);
	 float alpha = color.a;
	 float d = (dissolves.x + dissolves.y + dissolves.z) / 3;
	 if (d < uvStatus.y * 1.15) {
	   color.rgb = materialColor;
	  //alpha *= 0.5;
	 }
	 float3 nomal = float3(0, 0, 0);
	 float4 N = float4(normalize(nomal) * 0.5 + 0.5, alpha);
	 float4 P = float4(pin.position.xyz, 1);
	 if (d < uvStatus.y) {
	  alpha = 0;
	  N = n;
	  P = p;
	 }
	 if (d < uvStatus.z) {
		 color.rgb = materialColor;
		
	 }
	 //N = n;
	 //P = p;
	 const float GAMMA = 1.5;
	 color.rgb = pow(color.rgb, GAMMA);
	 color.a = alpha;
	 PS_OUT ret;
	 ret.Color = color;
	 ret.Depth = float4(1, 1, 1, 1);
	 ret.Normal = N;
	 ret.Position = P;

	 return ret;
}
