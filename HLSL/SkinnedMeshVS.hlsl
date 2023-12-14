#include "SkinnedMesh.hlsli"

float random(float2 seeds)
{
    return frac(sin(dot(seeds, float2(12.9898, 78.233))) * 43758.5453);
}

float blockNoise(float2 seeds)
{
    return random(floor(seeds));
}

float noiserandom(float2 seeds)
{
    return -1.0 + 2.0 * blockNoise(seeds);
}

VS_OUT main(VS_IN vin)
{
    vin.normal.w = 0.0f;
    float sigma = vin.tangent.w;
    vin.tangent.w = 0.0f;

    float4 blended_position = { 0.0f, 0.0f, 0.0f, 1.0f };
    float4 blended_normal = { 0.0f, 0.0f, 0.0f, 0.0f };
    float4 blended_tangent = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (int bone_index = 0; bone_index < 4; ++bone_index)
    {
        blended_position += vin.bone_weights[bone_index] * mul(vin.position, boneTransforms[vin.bone_indices[bone_index]]);
        blended_normal   += vin.bone_weights[bone_index] * mul(vin.normal, boneTransforms[vin.bone_indices[bone_index]]);
        blended_tangent  += vin.bone_weights[bone_index] * mul(vin.tangent, boneTransforms[vin.bone_indices[bone_index]]);
    }

    vin.position = float4(blended_position.xyz, 1.0f);
    vin.normal = float4(blended_normal.xyz, 0.0f);
    vin.tangent = float4(blended_tangent.xyz, 0.0f);

    float4 position = mul(vin.position, mul(world, view_projection));    
    float scan = step(scanBorder, -vin.position.y);    
    // スキャンの境界判定
    //if (scan < 1.0f)
    if (scan < 1.0f && maxHeight > 0.0f)
    {
        // キャラのローカル座標
        float4 localPosition = vin.position;
        localPosition.xyz = float3(0.0f, maxHeight, 0.0f);
        //localPosition.xyz = float3(localPosition.x, maxHeight, localPosition.z);
        position = mul(localPosition, mul(world, view_projection));
    }
    
    // グリッチ処理
    if (glitchIntensity > 0.0f)
    {
        float glitchSpeed = 50.0f;
        float noise = blockNoise(position.y * (glitchIntensity + 1.0f) * glitchScale);
        noise += random(position.x) * 0.3f;
        float2 randomvalue = noiserandom(float2(position.y, timer * glitchSpeed));
        position.x += randomvalue * sin(sin(glitchIntensity) * 0.5f) * sin(-sin(noise) * 0.2f) * frac(timer) * 10.0f;
    }

    VS_OUT vout;
    //vout.position = mul(vin.position, mul(world, view_projection));
    vout.position = position;
    vout.localPosition = vin.position;
    vout.world_position = mul(vin.position, world);
    //vin.normal.w = 0;
    vout.world_normal = normalize(mul(vin.normal, world));
    vout.world_tangent = normalize(mul(vin.tangent, world));
    vout.world_tangent.w = sigma;
    //vout.world_binormal = vin.binormal;
    vout.texcoord = vin.texcoord;
    row_major float4x4 m = (world, ShadowVP);
    //vout.ShadowParam = mul(m, vin.position);
    vout.ShadowParam = CalcShadowTexcoord(vout.world_position.xyz, ShadowVP);
    #if 1
       vout.color = materialColor;
    #else
       vout.color = 0;
       const float4 bone_colors[4] = {
       {1, 0, 0, 1},
       {0, 1, 0, 1},
       {0, 0, 1, 1},
       {1, 1, 1, 1},
       };
      for (int bone_index = 0; bone_index < 4; ++bone_index)
      {
           vout.color += bone_colors[vin.bone_indices[bone_index] % 4]
            *vin.bone_weights[bone_index];  
      }
    #endif

    return vout;
}