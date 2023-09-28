#include "InstancingShadowMap.hlsli"

VS_OUT main(VS_IN vin, uint instanceId : SV_INSTANCEID)
{
    //float4 blended_position = { 0, 0, 0, 1 };

   
    //vin.position = float4(blended_position.xyz, 1.0f);
    row_major float4x4 m = mul(world[instanceId], ShadowVP);
    VS_OUT vout;
    vout.position = mul(vin.position, m);
    vout.ShadowParam = vout.position;

    vout.texcoord = vin.texcoord;
    return vout;
}