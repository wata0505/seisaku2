#include "Particle.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps : register(t0);
Texture2D color_maps : register(t2);
Texture2D dissolve_map : register(t5);
Texture2D dissolveCircle_map : register(t6);
Texture2D nomal_maps : register(t8);
Texture2D posison_maps : register(t9);

PS_OUT main(VS_OUT pin) : SV_TARGET
{

	 float2 uv = pin.texcoord + pin.dissolve.y;
	 float4 color = color_maps.Sample(sampler_states[ANISOTROPIC], uv);
	 float4 dissolves = dissolve_map.Sample(sampler_states[ANISOTROPIC], uv);
	 float4 n = nomal_maps.Sample(sampler_states[ANISOTROPIC], uv);
	 float4 p = posison_maps.Sample(sampler_states[ANISOTROPIC], uv);

	 float alpha = color.a;
	 float d = (dissolves.x + dissolves.y + dissolves.z) / 3;
	 if (d < pin.dissolve.x * 1.01) {
	  color.rgb = pin.materialColor.rgb;
	  //alpha *= 0.5;
	 }
	 if (d < 0.5) {
	  color.rgb = pin.materialColor.rgb;
	  alpha *= pin.materialColor.a;
	 }
	 alpha *= pin.materialColor.a;
	 float3 nomal = float3(0, 0, 0);
	 float4 N = float4(normalize(nomal) * 0.5 + 0.5, 1);
	 float4 P = float4(pin.position.xyz, 1);
	 float4 D = float4(1,1,1,1);
	 if (d < pin.dissolve.x) {
	    alpha = 0;
	    N = n;
	    P = p;
		D = float4(0, 0, 0, 0);
	 }

	
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