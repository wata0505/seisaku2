#include "Particle.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps : register(t0);
Texture2D dissolve_map : register(t5);
Texture2D dissolveCircle_map : register(t6);
PS_OUT main(VS_OUT pin)
{

	 float4 color = texture_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	 float4 dissolves = dissolve_map.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	 
	 float alpha = color.a;
	 float d = (dissolves.x + dissolves.y + dissolves.z)/3;
	 if (d < pin.dissolve.x * 1.05) {
	  color.rgb = float3(1, 0, 0);
	  //alpha *= 0.5;
	 }
	 if (d < pin.dissolve.x) {
	  alpha = 0;
	 }
	float3 nomal = float3(0, 0, 0);
	 const float GAMMA = 1.5;
	 color.rgb = pow(color.rgb, GAMMA);
	 color.a = alpha;
     PS_OUT ret;
     ret.Color = color;
     ret.Depth = float4(1, 1, 1, 1);
     ret.Normal = float4(normalize(nomal) * 0.5 + 0.5, alpha);
     ret.Position = float4(nomal, 1);

     return ret;
	 //return color;
}