#include "Skybox.hlsli"

VS_OUT main(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 dissolve : Dissolve)
{
	VS_OUT vout;
	vout.position = position;
	vout.color = color;
	//vout.texcoord = texcoord;
	// NDC座標からワールド変換に変換
	float4 p;
	//	NDC座標からクリップ空間座標への変換
	float4	viewSpacePosition = mul(position, inverseProjection);

	//	クリップ空間座標からカメラ空間座標への変換
	viewSpacePosition /= viewSpacePosition.w;

	//	カメラ空間座標からワールド空間座標への変換
	p = mul(viewSpacePosition, inverseView);

	//	カメラ空間座標からワールド空間座標への変換
	vout.worldPosition = p.xyz;

	vout.influence = dissolve.x;
	vout.scroll = dissolve.y;

	return vout;
}
