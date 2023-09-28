#include "Sprite.hlsli"
Texture2D color_map : register(t0);
Texture2D dissolve_map : register(t5);
Texture2D dissolveCircle_map : register(t6);
Texture2D dissolveDiagonal_map : register(t7);
SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = color_map.Sample(anisotropic_sampler_state, pin.texcoord);
	//float2 uv = pin.texcoord * options.zz;
	float4 dissolve = dissolve_map.Sample(anisotropic_sampler_state, pin.texcoord);
	float4 dissolveCircle = dissolveCircle_map.Sample(anisotropic_sampler_state, pin.texcoord);
	float4 dissolveDiagonal = dissolveDiagonal_map.Sample(anisotropic_sampler_state, pin.texcoord);

	float alpha = color.a;
	float d = (dissolve.x + dissolve.y + dissolve.z);
	float dc = (dissolveCircle.x + dissolveCircle.y + dissolveCircle.z) / 3;
	float dd = (dissolveDiagonal.x + dissolveDiagonal.y + dissolveDiagonal.z);
	if (d < pin.dissolve.x * 1.05) {
		color.rgb = float3(1, 0, 0);
		//alpha *= 0.5;
	}
	if (dd < pin.dissolve.z * 1.01) {
		color.rgb = float3(1, 0, 0);
		//alpha *= 0.5;
	}
	if (d < pin.dissolve.x) {
		alpha = 0;
	}
	if (dc < pin.dissolve.y) {
		alpha = 0;
	}
	if (dd < pin.dissolve.z) {
		alpha = 0;
	}
#if 1
	//Inverse gamma process
	//const float GAMMA = 2.2;
	//color.rgb = pow(color.rgb, GAMMA);
#endif
	return float4(color.rgb,alpha ) * pin.color;
}