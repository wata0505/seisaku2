#include "Sprite.hlsli"
VS_OUT main( float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 dissolve : DISSOLVE)
{
	VS_OUT vout;
	vout.position = position;
	vout.color = color;

	vout.texcoord = texcoord;

	vout.dissolve = dissolve;

	return vout;
}