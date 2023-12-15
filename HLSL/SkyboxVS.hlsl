#include "Skybox.hlsli"

VS_OUT main(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 dissolve : Dissolve)
{
	VS_OUT vout;
	vout.position = position;
	vout.color = color;
	//vout.texcoord = texcoord;
	// NDC���W���烏�[���h�ϊ��ɕϊ�
	float4 p;
	//	NDC���W����N���b�v��ԍ��W�ւ̕ϊ�
	float4	viewSpacePosition = mul(position, inverseProjection);

	//	�N���b�v��ԍ��W����J������ԍ��W�ւ̕ϊ�
	viewSpacePosition /= viewSpacePosition.w;

	//	�J������ԍ��W���烏�[���h��ԍ��W�ւ̕ϊ�
	p = mul(viewSpacePosition, inverseView);

	//	�J������ԍ��W���烏�[���h��ԍ��W�ւ̕ϊ�
	vout.worldPosition = p.xyz;

	vout.influence = dissolve.x;
	vout.scroll = dissolve.y;

	return vout;
}
