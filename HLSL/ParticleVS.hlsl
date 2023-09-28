#include "Particle.hlsli"
VS_OUT main(VS_IN vin, uint instanceId : SV_INSTANCEID)
{
    vin.normal.w = 0;
    float sigma = vin.tangent.w;
    vin.tangent.w = 0;
    vin.position = float4(vin.position.xyz, 1.0f);
    vin.normal = float4(vin.normal.xyz, 0.0f);
    VS_OUT vout;
    //vin.normal = normalize(mul(vin.normal, world));
    vin.position.xyz += vin.normal.xyz * 0.05;
    vout.position = mul(vin.position, mul(world[instanceId], view_projection));

    vout.texcoord = vin.texcoord;
    vout.instanceId = instanceId;
    vout.materialColor = materialColor[instanceId];
    vout.dissolve = dissolve[instanceId];

    return vout;
}