#include "FullscreenQuad.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
Texture2D DiffuseTexture : register(t0);
Texture2D BoomTexture : register(t1);
SamplerState sampler_states[3] : register(s0);

float4 main(VS_OUT pin) : SV_TARGET0
{
	float4 tex = DiffuseTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
	float4 color = tex;
	float4 bloom = BoomTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord)*0.5;

	color.rgb += bloom.rgb;
	//color.rgb *= float3(0.2, 0.2, 0.9);
	//color = 1;
	color.rgb = ColorGrading(color.rgb);
	// ビネット
	color.rgb = Vignette(color.rgb, pin.texcoord);

	//color = float4(1, 1, 1, 1);
	return color;
}