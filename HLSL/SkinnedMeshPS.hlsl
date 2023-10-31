#include "SkinnedMesh.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps : register(t0);
Texture2D Normal_maps : register(t1);
Texture2D ShadowMap : register(t2);
Texture2D dissolve_map : register(t5);
SamplerState ShadowSampler : register(s10);
PS_OUT main(VS_OUT pin)
{

        float4 color = texture_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
        float4 dissolves = dissolve_map.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
        float alpha = color.a;

        float d = (dissolves.x + dissolves.y + dissolves.z)/3;
       
#if 1
        // Inverse gamma process
        const float GAMMA = 1.5;
        color.rgb = pow(color.rgb, GAMMA);
#endif
        float3 N = normalize(pin.world_normal.xyz);
        float3 T = normalize(pin.world_tangent.xyz);
        float sigma = pin.world_tangent.w;
        T = normalize(T - dot(N, T));
        float3 B = normalize(cross(N, T) * sigma);

        float4 normal = Normal_maps.Sample(sampler_states[LINEAR], pin.texcoord);
        normal = (normal * 2.0) - 1.0;
        N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));

        float3 L = normalize(-light_direction.xyz);
        float3 diffuse = color.rgb * max(0.8, dot(N, L));
        float3 V = normalize(camera_position.xyz - pin.world_position.xyz);
        float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);
        //float2 uv = pin.ShadowParam.xy;
        //uv.y = -uv.y;
        //uv = uv * 0.5 + 0.5;
        ////ShadowMap���狗��Get
        //float d = ShadowMap.Sample(ShadowSampler, uv).r;
        //float shadow =  1.0 - step(0.001, pin.ShadowParam.z - d) * 0.4;
        float3 shadow = CalcShadowColorPCFFilter(ShadowMap,ShadowSampler, pin.ShadowParam, float3(0.2,0.2,0.2), 0.001);

        float dist =
            length(pin.world_position.xyz - camera_position.xyz);
        normal = float4(normalize(pin.world_normal.xyz) * 0.5 + 0.5, 1);
        if (dist < 4 && pin.world_position.y > 0) {
            alpha = dist / 10;
            normal = float4(0, 0, 0, 0);
        }
        if (d < uvStatus.x * 1.05) {
            diffuse = float3(1, 0, 0);
            normal = float4(0, 0, 0, 0);
        }
        if (d < uvStatus.x) {
            alpha = 0;
            normal = float4(0, 0, 0, 0);
        }
        PS_OUT ret;
        ret.Color = float4((diffuse + specular) * shadow, alpha) * pin.color;
        ret.Depth = float4(dist, dist, dist, 1);
        ret.Normal = normal;
        ret.Position = float4(pin.world_position.xyz, 1);

        return ret;
}