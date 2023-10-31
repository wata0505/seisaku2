#include "Skybox.hlsli"

Texture2D texture0 : register(t0);
SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);

float4 main(VS_OUT pin) : SV_TARGET
{
	//return texture0.Sample(linear_sampler_state, pin.texcoord) * pin.color;
	//	�J�������烏�[���h���W�ւ̃x�N�g�����Z�o���A��������ʏ�̍��W�Ƃ��Ĉ���
	float3	E = normalize(pin.worldPosition.xyz - viewPosition.xyz);

	//	�����x�N�g����UV���W�֕ϊ�����
	float2	texcoord;
    
	texcoord.x = -((1 / 3.14159264) * atan(E.z / E.x));
	texcoord.y = -((1 / 3.14159264) * atan(E.y / sqrt((E.x * E.x) + (E.z * E.z)))) + 0.5;

	texcoord.y -= pin.dis * 0.5;
	return texture0.Sample(anisotropic_sampler_state, texcoord) * pin.color;

}
