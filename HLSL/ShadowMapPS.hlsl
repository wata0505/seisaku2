#include "ShadowMap.hlsli"

float4 main(VS_OUT pin) : SV_TARGET
{
	float depth = pin.ShadowParam.z;
	return float4(depth, 0, 0, 1);
}