#include "Skybox.hlsli"

Texture2D texture0 : register(t0);
SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);

float4 main(VS_OUT pin) : SV_TARGET
{
	//return texture0.Sample(linear_sampler_state, pin.texcoord) * pin.color;
	//	カメラからワールド座標へのベクトルを算出し、それを球面上の座標として扱う
	float3	E = normalize(pin.worldPosition.xyz - viewPosition.xyz);

	//	視線ベクトルをUV座標へ変換する
	float2	texcoord;
    
	texcoord.x = -((1 / 3.14159264) * atan(E.z / E.x));
	texcoord.y = -((1 / 3.14159264) * atan(E.y / sqrt((E.x * E.x) + (E.z * E.z)))) + 0.5;

	texcoord.y -= pin.dis * 0.5;
	return texture0.Sample(anisotropic_sampler_state, texcoord) * pin.color;

}
