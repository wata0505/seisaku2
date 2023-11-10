#include    "ShadowmapFunctions.hlsli"
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
    float4 world_position : POSITION;
    float4 world_normal : NORMAL;
    float4 world_tangent : TANGENT;
    float3 world_binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;

    float3 ShadowParam : TEXCOORD3;
};

struct PS_OUT 
{
    float4 Color : SV_TARGET0; // カラー
    float4 Depth : SV_TARGET1; // 距離
    float4 Normal : SV_TARGET2; // 法線
    float4 Position : SV_TARGET3; // 座標
    float4 MetalSmoothness : SV_TARGET4;    // メタリック
    float4 AmbientOcclusion : SV_TARGET5;   // 粗さ
    float4 Emission : SV_TARGET6;           // エミッシブ
};
static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 materialColor;
    row_major float4x4 boneTransforms[MAX_BONES];
};
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;
    float4 light_direction;
    float4 camera_position;
};
cbuffer UV_BUFFER : register(b2)
{
    float4 uvStatus;
};
cbuffer SHADOW_CONSTANT_BUFFER : register(b5)
{
    row_major float4x4 ShadowVP;
};