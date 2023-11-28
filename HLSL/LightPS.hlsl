#include "Light.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
Texture2D DiffuseTexture : register(t0);
Texture2D PositionTexture : register(t1);
SamplerState sampler_states[3] : register(s0);
float4 main(VS_OUT pin) : SV_TARGET
{
	//return DiffuseTexture.Sample(sampler_states[LINEAR],pin.texcoord);
	float4 tex = DiffuseTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	float4 color = tex;
	// �_��������
	float4 P = PositionTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	float uv = (P.x + P.y + P.z) / 3;
	//if (uv == 0 && LightDirection.w > 0)return tex;
	float3 PLightPos = LightDirection.xyz;
	float3 PLightDir = P.xyz - PLightPos;
	// ���C�g���
	// 
	//float type = LightDirection.w<0.001?0:1;
	float type = step(0.001, LightDirection.w);
	// �����̐ݒ�
	float attenuation = type < 0.1 ? 1.0 : 1.0 - (length(PLightDir) / LightDirection.w);
	attenuation = saturate(attenuation);

	// ���s��
	//float3 L = type<0.1? LightDirection.xyz : PLightDir;
	float3 L = lerp(LightDirection.xyz, PLightDir, type);

	float3 N = tex.xyz * 2.0 - 1.0;
	float NL = length(N);
	float3 C = LightColor.rgb;
	C *= attenuation; //����
	
	L = normalize(L);
	N = normalize(N);
	float d = -dot(N, L); //= -cos
	d = saturate(d); // 0�`1�ɃN���b�s���O
	tex.rgb = C * d; //���C�g����
	if (NL > 1.3) tex = 1.0;
	//if (uv == 0)  tex = 0.7;
	//tex = {1,1,1,1};
	return tex;
}