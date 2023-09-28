#include "StaticMech.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
	VS_OUT vout;
	vout.position = mul(position, mul(world, view_projection));

	//normal.w = 0;
	//float4 N = normalize(mul(normal, world));
	//float4 L = normalize(-light_direction);
	//
	//vout.color.rgb = materialColor.rgb * max(0, dot(L, N));
	//vout.color.a = materialColor.a;

	vout.world_position = mul(position, world);
	normal.w = 0;
	vout.world_normal = normalize(mul(normal, world));

	vout.color = materialColor;
	
	vout.texcoord = texcoord;

	return vout;
}