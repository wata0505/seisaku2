#include "InstancingModel.hlsli"
VS_OUT main(VS_IN vin, uint instanceId : SV_INSTANCEID)
{
    vin.normal.w = 0;
    float sigma = vin.tangent.w;
    vin.tangent.w = 0;
    //float4 blended_position = { 0, 0, 0, 1 };
    //float4 blended_normal = { 0, 0, 0, 0 };
    //float4 blended_tangent = { 0,0,0,0 };
    //for (int bone_index = 0; bone_index < 4; ++bone_index)
    //{
    //    blended_position += vin.bone_weights[bone_index] * mul(vin.position, boneTransforms[vin.bone_indices[bone_index]]);
    //    blended_normal += vin.bone_weights[bone_index] * mul(vin.normal, boneTransforms[vin.bone_indices[bone_index]]);
    //    blended_tangent += vin.bone_weights[bone_index] * mul(vin.tangent, boneTransforms[vin.bone_indices[bone_index]]);
    //
    //}
    vin.position = float4(vin.position.xyz, 1.0f);
    vin.normal = float4(vin.normal.xyz, 0.0f);
    vin.tangent = float4(vin.tangent.xyz, 0.0f);

    float4 position = mul(vin.position, mul(world[instanceId], view_projection));
    float scan = step(scanBorder, -vin.position.y);
    // スキャンの境界判定
    if (scan < 1.0f && maxHeight > 0.0f)
    {
        // キャラのローカル座標
        float4 localPosition = vin.position;
        localPosition.xyz = float3(0.0f, maxHeight, 0.0f);
        //localPosition.xyz = float3(localPosition.x, maxHeight, localPosition.z);
        position = mul(localPosition, mul(world[instanceId], view_projection));
    }

    VS_OUT vout;
    vout.position = position;
    vout.localPosition = vin.position;
    vout.world_position = mul(vin.position, world[instanceId]);
    //vin.normal.w = 0;
    vout.world_normal = normalize(mul(vin.normal, world[instanceId]));
    vout.world_tangent = normalize(mul(vin.tangent, world[instanceId]));
    vout.world_tangent.w = sigma;
    //vout.world_binormal = vin.binormal;
    vout.texcoord = vin.texcoord;
    row_major float4x4 m = (world, ShadowVP);
    //vout.ShadowParam = mul(m, vin.position);
    vout.ShadowParam = CalcShadowTexcoord(vout.world_position.xyz, ShadowVP);
    return vout;
}