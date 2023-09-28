#include "Toon.hlsli"
VS_OUT main(VS_IN vin)
{
    vin.normal.w = 0;
    float sigma = vin.tangent.w;
    vin.tangent.w = 0;

    float4 blended_position = { 0, 0, 0, 1 };
    float4 blended_normal = { 0, 0, 0, 0 };
   
    for (int bone_index = 0; bone_index < 4; ++bone_index)
    {
        blended_position += vin.bone_weights[bone_index] * mul(vin.position, boneTransforms[vin.bone_indices[bone_index]]);
        blended_normal += vin.bone_weights[bone_index] * mul(vin.normal, boneTransforms[vin.bone_indices[bone_index]]);

    }
    vin.position = float4(blended_position.xyz, 1.0f);
    vin.normal = float4(blended_normal.xyz, 0.0f);

    VS_OUT vout;
    //vin.normal = normalize(mul(vin.normal, world));
    vin.position.xyz += vin.normal.xyz * 0.05;

    vout.position = mul(vin.position, mul(world, view_projection));

    vout.world_position = mul(vin.position, world);
    //vin.normal.w = 0;
    vout.world_normal = normalize(mul(vin.normal, world));
    
    vout.texcoord = vin.texcoord;

    return vout;
}