#include "ParticleSprite.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps : register(t0);
Texture2D color_maps : register(t1);
Texture2D dissolve_map : register(t5);
float4 main(PS_INPUT Input) : SV_Target
{
	float4 tex = texture_maps.Sample(sampler_states[ANISOTROPIC], Input.Tex);
	float2 uv = Input.Color + Input.uvTimer;
	float4 color = color_maps.Sample(sampler_states[ANISOTROPIC], uv);
	float4 dissolves = dissolve_map.Sample(sampler_states[ANISOTROPIC], Input.Tex);

	float d = (dissolves.x + dissolves.y + dissolves.z);
	float4 addcolor = tex;
	//addcolor *= MaterialColor;
	//addcolor += color;
	//tex.a *= MaterialColor.a;
	tex.rgb = (color.rgb * tex.a);
	//tex.rgb = color.rgb;
	if (d < UvStatus.x) {
		tex.rgb = MaterialColor.rgb;
		tex.a *= MaterialColor.a;
	}
	tex.a *= MaterialColor.a;
	//tex.rgb = color.rgb;
	//color *= MaterialColor;
	//tex.rgb =  float3(1,0,0);
	return tex;
	//				float4 vec1 = normalize(Input.Pos);
	//				return float4(vec1.x, vec1.y, vec1.z, 1);
}