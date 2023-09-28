#include "FullscreenQuad.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps[4] : register(t0);
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = 0;//texture_maps[0].Sample(sampler_states[ANISOTROPIC],pin.texcoord);
    float2 uv = pin.texcoord - 0.5;
    float dist = length(uv);
    float factor = 0.5 / 8 * dist;
    for (int j = 0; j < 8; j++) {
        float uvOffset = 1 - factor * j;
        color += texture_maps[0].Sample(sampler_states[ANISOTROPIC], uv * uvOffset + 0.5);
    }
    color /= 8;
   
    return color;
}