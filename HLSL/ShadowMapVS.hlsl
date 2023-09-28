#include "ShadowMap.hlsli"

VS_OUT main(VS_IN vin)
{
    float4 blended_position = { 0, 0, 0, 1 };
    
    for (int bone_index = 0; bone_index < 4; ++bone_index)
    {
        blended_position += vin.bone_weights[bone_index] * mul(vin.position, boneTransforms[vin.bone_indices[bone_index]]);
    }
    vin.position = float4(blended_position.xyz, 1.0f);
    row_major float4x4 m = mul(world, ShadowVP);
    VS_OUT vout;
    vout.position = mul(vin.position, m);
    vout.ShadowParam = vout.position;

    vout.texcoord = vin.texcoord;
    return vout;
}