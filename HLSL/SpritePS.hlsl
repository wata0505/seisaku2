#include "Sprite.hlsli"
Texture2D color_map : register(t0);
Texture2D dissolve_map : register(t5);
Texture2D dissolveCircle_map : register(t6);
Texture2D dissolveDiagonal_map : register(t7);
SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);

#define NOISE 0
#if !NOISE
float random(float2 seeds)
{
	return frac(sin(dot(seeds, float2(12.9898f, 78.233f))) * 43758.5453f);
}
float blockNoise(float2 seeds)
{
	return random(floor(seeds));
}
#endif
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
#if NOISE
	if (d < pin.dissolve.x * 1.05) 
#else
	float2 tex = pin.texcoord * 100.0f;
	float rate = 0.0000005f;
	float valueX = random(blockNoise(tex) + pin.dissolve.x * rate);
	float valueZ = random(blockNoise(tex) + pin.dissolve.z * rate);
	if (valueX < pin.dissolve.x * 1.025)
#endif
	{
		color.rgb = float3(1, 0, 0);
		//alpha *= 0.5;
	}
#if NOISE
	if (dd < pin.dissolve.z * 1.01)
#else
	if (valueZ < pin.dissolve.z * 1.01)
#endif
	{
		color.rgb = float3(1, 0, 0);
		//alpha *= 0.5;
	}
#if NOISE
	if (d < pin.dissolve.x)
#else
	if (valueX < pin.dissolve.x)
#endif
	{
		alpha = 0;
	}
	if (dc < pin.dissolve.y)
	{
		alpha = 0;
	}
#if NOISE
	if (dd < pin.dissolve.z)
#else
	if (valueZ < pin.dissolve.z)
#endif
	{
		alpha = 0;
	}

	return float4(color.rgb, alpha) * pin.color;
}