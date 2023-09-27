#include "Instancing.hlsli"

Texture2D tex_map : register(t0);
Texture2D color_maps : register(t1);
Texture2D dissolve_map : register(t5);
Texture2D dissolveCircle_map : register(t6);
SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);

float4 main(PS_INPUT pin) : SV_TARGET
{
	float4 tex = tex_map.Sample(anisotropic_sampler_state, pin.Tex);
	float2 uv = pin.Color * pin.Dissolve.b;
	float4 dissolve = dissolve_map.Sample(anisotropic_sampler_state, uv);
	float4 dissolveCircle = dissolveCircle_map.Sample(anisotropic_sampler_state, pin.Tex);
	
	float4 color = color_maps.Sample(anisotropic_sampler_state, uv);
	float4 addcolor = tex;
	addcolor *= pin.MaterialColor;
	//addcolor += color;
	tex.a *= pin.MaterialColor.a;
	tex.rgb = addcolor + (color.rgb * tex.a);
	float alpha = tex.a;
	float d = (dissolve.x + dissolve.y + dissolve.z);
	float dc = (dissolveCircle.x + dissolveCircle.y + dissolveCircle.z) / 3;
	
	if (d < pin.Dissolve.y) {
		alpha = 0;
	}
	alpha *= pin.Dissolve.x;
#if 1
	//Inverse gamma process
	//const float GAMMA = 2.2;
	//color.rgb = pow(color.rgb, GAMMA);
#endif
	return float4(color.rgb,alpha);
}