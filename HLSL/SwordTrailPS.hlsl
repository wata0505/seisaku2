#include "SwordTrail.hlsli"
Texture2D SwordTex : register(t0);
Texture2D dissolve_map : register(t5);
SamplerState anisotropic_sampler_state : register(s2);
SamplerState linear_sampler_state : register(s1);
float4 main(PS_IN input) : SV_TARGET0
{
  float2 uv = input.uv + materialColor.w;
  float4 color = SwordTex.SampleLevel(anisotropic_sampler_state, input.uv, 0);
  float4 dissolve = dissolve_map.Sample(anisotropic_sampler_state, uv);
  float d = (dissolve.x + dissolve.y + dissolve.z);

  if (d < materialColor.x) {
	  //color.rgb = float3(1,0,0);
	  //color.a = 0;
  }
 // color *= materialColor;
  return color;
}