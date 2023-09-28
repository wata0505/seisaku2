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
    float2 texcoord : TEXCOORD;
    uint instanceId : SV_INSTANCEID;
    float4 materialColor : MATERIALCOLOR;
    float4 dissolve : Dissolve;
};

struct PS_OUT {
    float4 Color : SV_TARGET0; // カラー
    float4 Depth : SV_TARGET1; // 距離
    float4 Normal : SV_TARGET2; // 法線
    float4 Position : SV_TARGET3; // 座標
};

#define MAX_INSTANCES 512
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world[MAX_INSTANCES];
    float4 materialColor[MAX_INSTANCES];
    float4  dissolve[MAX_INSTANCES];
};
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;
    float4 light_direction;
    float4 camera_position;
};