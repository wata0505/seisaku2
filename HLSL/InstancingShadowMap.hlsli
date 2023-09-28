struct VS_IN
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
    float3 binormal : BINORMAL;
};
struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float4 ShadowParam : TEXCOORD2;
};
static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world[100];
};
cbuffer SHADOW_CONSTANT_BUFFER : register(b5)
{
    row_major float4x4 ShadowVP;
};