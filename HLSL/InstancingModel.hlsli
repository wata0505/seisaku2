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

    float3 ShadowParam : TEXCOORD3;
    float4 localPosition : POSITION2;
};

struct PS_OUT
{
    float4 Color : SV_TARGET0; // �J���[
    float4 Depth : SV_TARGET1; // ����
    float4 Normal : SV_TARGET2; // �@��
    float4 Position : SV_TARGET3; // ���W
    float4 MetalSmoothness : SV_TARGET4;    // ���^���b�N
    float4 AmbientOcclusion : SV_TARGET5;   // �e��
    float4 Emission : SV_TARGET6;           // �G�~�b�V�u
};
static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world[100];
    //row_major float4x4 boneTransforms[MAX_BONES];
   
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

// �T�u�Z�b�g�p�萔�o�b�t�@
cbuffer SubsetConstantBuffer : register(b6)
{
    float adjustMetalness;	// �����x
    float adjustSmoothness;	// �e��
    float glitchIntensity;	// ���x
    float glitchScale;		// �U�ꕝ
    float timer;			// �X�V����
    float scanBorder;		// �`��͈�
    float glowBorder;		// �`��͈�
    float hologramBorder;	// �`��͈�
    float3 hologramColor;   // �z���O�����F
    float maxHeight;        // �ō��_
    int yUp;
    float3 dummy;
};