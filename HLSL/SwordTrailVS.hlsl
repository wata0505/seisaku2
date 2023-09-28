#include "SwordTrail.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN result = (PS_IN)0;
    result.pos = mul(view_projection,float4(input.position, 1));
    result.uv = input.texcoord;

    return result;
}