struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	//float2 texcoord : TEXCOORD;
	float3 worldPosition	: POSITION;
	float influence : INFLUENCE;
	float scroll : SCROLL;
};
cbuffer CbScene : register(b0)
{
	float4				viewPosition;
	row_major float4x4	inverseView;
	row_major float4x4	inverseProjection;
};
