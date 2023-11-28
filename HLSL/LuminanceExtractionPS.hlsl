#include "LuminanceExtraction.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps[4] : register(t0);
float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC], pin.texcoord) * pin.color;
	if (color.x >= 0.7 || color.y >= 0.7 || color.z >= 0.7) {
		if (color.x >= 0.3 && color.y >= 0.3 && color.z >= 0.3) {
			//color = float4(1.0, 1.0, 1.0, 1.0);
			//return  color;
		}
		else {
			//color.rgb *= intensity;
			//return  color;
		}
	}
	// RGB > 輝度値に変換
	float luminance = RGB2Luminance(color.rgb);
	float colorX = (color.x + color.y + color.z) / 3;
	// 閾値との差を算出
	float contribution = max(0, luminance - threshold);
	if (color.x == 0 && color.y == 0 && color.z == 0) {
		return  color;
	}
	
	// 出力する色を補正する
	contribution /= luminance;
	color.rgb *= contribution * intensity;
	if (color.x >= 0.1 && color.y >= 0.1&& color.z >= 0.1) {
		//color = float4(0.1, 0.1, 0.1, 1);
		//return  color;
	}
	return color;

    //float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC],pin.texcoord);
    //float alpha = color.a;
    //color.rgb = smoothstep(0.6, 0.8, dot(color.rgb, float3(0.299, 0.587, 0.114))) * color.rgb;
    //return float4(color.rgb, alpha);
}
//float3(0.299, 0.587, 0.114))