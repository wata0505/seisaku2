#include "InstancingModel.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps : register(t0);
Texture2D Normal_maps : register(t1);
Texture2D ShadowMap : register(t2);
Texture2D MetalSmoothnessTexture : register(t3);
Texture2D AmbientOcclusionTexture : register(t4);
Texture2D EmissionTexture : register(t7);
Texture2D nomal_maps : register(t8);
Texture2D posison_maps : register(t9);
SamplerState ShadowSampler : register(s10);

PS_OUT main(VS_OUT pin)
{
    float4 color = texture_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    // アルベドカラーの最大値
    float maxColor = max(color.r, max(color.g, max(color.b, color.a)));
    // エミッシブ
    float4 emissive = EmissionTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    // エミッシブカラーの最大値
    float emissiveMax = max(emissive.r, max(emissive.g, emissive.b));

    float alpha = color.a;
    float4 n = nomal_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    float4 p = posison_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
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
    ////ShadowMapから距離Get
    //float d = ShadowMap.Sample(ShadowSampler, uv).r;
    //float shadow =  1.0 - step(0.001, pin.ShadowParam.z - d) * 0.4;
    float3 shadow = CalcShadowColorPCFFilter(ShadowMap, ShadowSampler, pin.ShadowParam, float3(0.2, 0.2, 0.2), 0.001);

    float4 directColor = float4((diffuse + specular) * shadow, alpha);

    // エミッシブ適応可能値なら加算
    if (emissiveMax > 0.0f)
    {
        directColor.rgb += emissive.rgb;
    }

    // キャラ
    if (glitchScale > 0.0f)
    {
        // 赤色
        float3 red = float3(1.0f, 0.0f, 0.0f);

        // ホログラムと実体の境界判定
        // モデルのローカル空間での境界判定
        float hologram = 0.0f;
        if (yUp)
        {
            hologram = step(hologramBorder, -pin.localPosition.y);
        }
        else
        {
            hologram = step(hologramBorder, -pin.localPosition.z);
        }

        float3 holoColor = 0;
        switch ((int)hologramColor.r)
        {
        case 0:
            holoColor = directColor.rgb;
            break;
        case 1:
            holoColor = float3(1.0f, 0.0f, 0.0f);
            break;
        case 2:
            holoColor = float3(0.0f, 1.0f, 0.0f);
            break;
        case 3:
            holoColor = float3(0.0f, 0.0f, 1.0f);
            break;
        }
        // ホログラムなら
        if (hologram <= 0.0f)
        {
            // Borderlines Direction
            float3 direction = float3(0.0f, 1.0f, 0.0f);
            float dirVertex = (dot(pin.world_position, normalize(float4(direction, 1.0f))) + 1.0f) * 0.5f;

            // Scanlines
            float scanTiling = 20.0f;
            float scanSpeed = -2.0f;
            float scan = step(frac(dirVertex * scanTiling + timer * scanSpeed), 0.5f) * 0.65f;

            // Glow
            float glowTiling = 1.0f;
            float glowSpeed = 2.0f;
            float glow = frac(dirVertex * glowTiling - timer * glowSpeed);

            // ホログラム内でのスキャンとグロウの境界判定
            if (yUp)
            {
                scan *= step(scanBorder, -pin.localPosition.y);
                glow *= step(glowBorder, -pin.localPosition.y);
            }
            else
            {
                scan *= step(scanBorder, -pin.localPosition.z);
                glow *= step(glowBorder, -pin.localPosition.z);
            }

            // ホログラムカラーの合計値
            directColor.rgb = (scan * holoColor + glow * (holoColor * 1.2f));

            // glitchIntensity : 被弾 or 死亡時に強まる
            // アルファ値 = (ベースカラーの透過値 * (スキャンライン + グロウライン)) * (1.0f - グリッチ強度の半分)
            directColor.a = (color.a * (scan + glow)) * (1.0f - glitchIntensity * 0.5f);
        }
        // 実体なら
        else
        {
            // アルファ値 = (ベースカラーの透過値 * ホログラムライン) * (1.0f - グリッチ強度の半分)
            directColor.a = (color.a * hologram) * (1.0f - glitchIntensity * 0.5f);
        }

        // ホログラムと実体の淵表現
        // モデルのローカル空間での処理の性質上、モデルサイズが大きければ大きいほどsizeの値を大きくしないと見えない
        float size = 0.2f;
        float edgeValue = 0.0f;
        if (yUp)
        {
            edgeValue = saturate(1.0f - abs(hologramBorder - (-pin.localPosition.y)) * (1.0f / size));
        }
        else
        {
            edgeValue = saturate(1.0f - abs(hologramBorder - (-pin.localPosition.z)) * (1.0f / size));
        }

        directColor.rgb += holoColor * edgeValue * 2.0f + red * glitchIntensity;
    }

    float dist = length(pin.world_position.xyz - camera_position.xyz);
    normal = float4(normalize(pin.world_normal.xyz) * 0.5 + 0.5, 1);
    float4 P = float4(pin.world_position.xyz, 1);
#if 0
    if (dist < 4) 
    {
        alpha = dist / 10;
        normal = n;
        P = p;
    }
#endif
    PS_OUT ret;
    //ret.Color = float4((diffuse + specular) * shadow, alpha);
    ret.Color = directColor;
    ret.Depth = float4(dist, dist, dist, 1);
    ret.Normal = normal;
    ret.Position = P;
    //ret.MetalSmoothness = float4(metallic, 0.0f, 0.0f, smoothness);
    //ret.AmbientOcclusion = float4(ambientOcculusion, 0.0f, 0.0f, 1.0f);
    ret.Emission = float4(emissive.rgb, 1.0f);
    return ret;
}