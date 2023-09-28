struct VS_IN
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};
cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
    float4 materialColor;	// マテリアルカラー
};
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    float4x4 view_projection;
    float4 light_direction;
    float4 camera_position;
};
