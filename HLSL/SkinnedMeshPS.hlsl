#include "SkinnedMesh.hlsli"
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
Texture2D dissolve_map : register(t5);
SamplerState ShadowSampler : register(s10);

// 円周率
static const float PI = 3.141592654f;
// 誘電率(非金属でも最低4%(0.04f)は鏡面反射する)
static const float Dielectric = 0.04f;
//-----------------------------------------
// 拡散反射BRDF(正規化ランバートの拡散反射)
//-----------------------------------------
// diffuseReflectance : 入射光のうち拡散反射になる割合
float3 DiffuseBRDF(float3 diffuseReflectance)
{
    return diffuseReflectance / PI;
}
//-----------------------------------------
// フレネル項
//-----------------------------------------
// F0 : 垂直入射時の反射率
// VdotH : 視線ベクトルとハーフベクトル(光源へのベクトルと視点へのベクトルの中間ベクトル)
float3 CalcFresnel(float3 F0, float VdotH)
{
    return F0 + (1 - F0) * ((1 - VdotH) * (1 - VdotH) * (1 - VdotH) * (1 - VdotH) * (1 - VdotH));
}
//-----------------------------------------
// 法線分布関数
//-----------------------------------------
// NdotH : 法線ベクトルとハーフベクトル(光源へのベクトルと視点へのベクトルの中間ベクトル)
// roughness : 粗さ
float CalcNormalDistributionFunction(float NdotH, float roughness)
{
    return (roughness * roughness) / (PI * ((NdotH * NdotH) * ((roughness * roughness) - 1) + 1) * ((NdotH * NdotH) * ((roughness * roughness) - 1) + 1));
}
//-----------------------------------------
// 幾何減衰項の算出
//-----------------------------------------
// NdotV : 法線ベクトルと他ベクトルとの内積
// k : 粗さ / 2
float CalcGeometryFunction(float NdotV, float k)
{
    return NdotV / (NdotV * (1 - k) + k);
}
// NdotL : 法線ベクトルと光源へのベクトルとの内積
// NdotV : 法線ベクトルと視線へのベクトルとの内積
// roughness : 粗さ
float CalcGeometryFunction(float NdotL, float NdotV, float roughness)
{
    float r = roughness * 0.5f;
    float shadowing = CalcGeometryFunction(NdotL, r);
    float masking = CalcGeometryFunction(NdotV, r);
    return shadowing * masking;
}
//-----------------------------------------
// 鏡面反射BRDF(クック・トランスのマイクロファセットモデル)
//-----------------------------------------
// NdotV : 法線ベクトルと視線へのベクトルとの内積
// NdotL : 法線ベクトルと光源へのベクトルとの内積
// NdotH : 法線ベクトルとハーフベクトルとの内積
// VdotH : 視線へのベクトルとハーフベクトルとの内積
// fresnelIF0 : 垂直入射時のフレネル反射色
// roughness : 粗さ
float3 SpecularBRDF(float NdotV, float NdotL, float NdotH, float VdotH, float3 fresnelIF0, float roughness)
{
    // D項(法線分布)
    float D = CalcNormalDistributionFunction(NdotH, roughness);
    // G項(幾何減衰項)
    float G = CalcGeometryFunction(NdotL, NdotV, roughness);
    // F項(フレネル反射)
    float3 F = CalcFresnel(fresnelIF0, VdotH);

    return (D * G * F) / (4 * (NdotL * NdotV));
}
//--------------------------------------------
//	直接光の物理ベースライティング
//--------------------------------------------
// diffuseReflectance	: 入射光のうち拡散反射になる割合
// F0					: 垂直入射時のフレネル反射色
// normal				: 法線ベクトル(正規化済み)
// eyeVector			: 視点に向かうベクトル(正規化済み)
// lightVector			: 光源に向かうベクトル(正規化済み)
// lightColor			: ライトカラー
// roughness			: 粗さ
void DirectBDRF(float3 diffuseReflectance, float3 F0, float3 normal, float3 eyeVector,
    float3 lightVector, float3 lightColor, float roughness, out float3 outDiffuse, out float3 outSpecular)
{
    float3 N = normal;
    float3 L = -lightVector;
    float3 V = eyeVector;
    float3 H = normalize(L + V);

    float NdotV = max(0.0001f, dot(N, V));
    float NdotL = max(0.0001f, dot(N, L));
    float NdotH = max(0.0001f, dot(N, H));
    float VdotH = max(0.0001f, dot(V, H));

    float3	irradiance = lightColor * NdotL;

    // レガシーなライティングとの互換性を保つ場合はPIで乗算する
    irradiance *= PI;

    // 拡散反射BRDF(正規化ランバートの拡散反射)
    outDiffuse = DiffuseBRDF(diffuseReflectance) * irradiance;

    // 鏡面反射BRDF（クック・トランスのマイクロファセットモデル）
    outSpecular = SpecularBRDF(NdotV, NdotL, NdotH, VdotH, F0, roughness) * irradiance;
}
//--------------------------------------------
// 環境光のBRDF
//--------------------------------------------
// F0			: 垂直入射時のフレネル反射色
// roughness	: 粗さ
// NdotV		: 法線ベクトルと視線へのベクトルとの内積
float3 EnvBRDFApprox(float3 F0, float roughness, float NdotV)
{
    const	float4	c0 = { -1.0f, -0.0275f, -0.572f, 0.022f };
    const	float4	c1 = { 1.0f,  0.0425f,   1.04f, -0.04f };
    float4	r = roughness * c0 + c1;
    float	a004 = min(r.x * r.x, exp2(-9.28f * NdotV)) * r.x + r.y;
    float2	AB = float2(-1.04f, 1.04f) * a004 + r.zw;
    return	F0 * AB.x + AB.y;
}

// アウトライン
float Outline(float2 uv, Texture2D Depth, Texture2D Normal, SamplerState Decal)
{
    float depth = Depth.Sample(Decal, uv).r;
    float2 error1 = float2(0.001f, 0.0f);
    float2 error2 = float2(0.0f, 0.0015f);
    float dR = Depth.Sample(Decal, uv + error1).r;
    float dD = Depth.Sample(Decal, uv + error2).r;

    float d = abs(depth - dR);
    float d2 = abs(depth - dD);
    //if (d > 0.3 * depth) return 0;
    //if (d2 > 0.3 * depth) return 0;
    //法線に差がある→輪郭
    float3 N = Normal.Sample(Decal, uv).xyz;
    float3 NR = Normal.Sample(Decal, uv + error1).xyz;
    float3 ND = Normal.Sample(Decal, uv + error2).xyz;
    N = N * 2.0 - 1.0; //0-1 => -1 - +1
    NR = NR * 2.0 - 1.0;
    ND = ND * 2.0 - 1.0;
    if (dot(N, NR) < 0.1) return 0;
    if (dot(N, ND) < 0.1) return 0;
    return 1;
}

PS_OUT main(VS_OUT pin)
{
    // アルベドカラー(非金属部分)
    float4 albedo = texture_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    // アルベドカラーの最大値
    float maxColor = max(albedo.r, max(albedo.g, max(albedo.b, albedo.a)));
    // エミッシブ
    float3 emissive = EmissionTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).rgb;
    emissive *= emissiveStrength;
    // エミッシブカラーの最大値
    float emissiveMax = max(emissive.r, max(emissive.g, emissive.b));
    // 最大値が0なら透明ピクセルやからマテリアルカラーを代入(後にエミッシブも追加で判定)
    if (maxColor <= 0.0f)
    {
        if (emissiveMax <= 0.0f)
        {
            albedo = pin.color;
        }
        else
        {
            albedo.rgb = emissive;
            albedo.a = 1.0f;
        }
    }
    // Inverse gamma process
    const float GAMMA = 1.5f;
    albedo.rgb = pow(albedo.rgb, GAMMA);

    // 金属度
    float metallic = MetalSmoothnessTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).r;
    // 滑らかさ
    float smoothness = MetalSmoothnessTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).a;
    // 環境光遮蔽度
    float ambientOcculusion = AmbientOcclusionTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).r;
    // 調整
    metallic = saturate(metallic + adjustMetalness);
    smoothness = saturate(smoothness + adjustSmoothness);

    // 粗さ
    float roughness = 1.0f - smoothness;
    // 入射光のうち拡散反射になる割合
    float3 diffuseReflectance = lerp(albedo.rgb, 0.02f, metallic);
    // 垂直反射時のフレネル反射率
    float3 F0 = lerp(Dielectric, albedo.rgb, metallic);

    // 法線マップからxyz成分を取得して( -1 ～ +1 )の間にスケーリング
    float3 N = normalize(pin.world_normal.xyz);
    float3 T = normalize(pin.world_tangent.xyz);
    float sigma = pin.world_tangent.w;
    T = normalize(T - dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);
    float4 normal = Normal_maps.Sample(sampler_states[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
    float3 V = normalize(camera_position.xyz - pin.world_position.xyz);
    float3 L = normalize(-light_direction.xyz);

    // ライティング計算
    float3 directDiffuse = 0.0f, directSpecular = 0.0f;
    float3 LightColor = float3(1.0f, 1.0f, 1.0f);
    // 平行光源の処理
    DirectBDRF(diffuseReflectance, F0, N, V, L, LightColor.rgb, roughness, directDiffuse, directSpecular);

    //float3 diffuse = albedo.rgb * max(0.8f, dot(N, L));
    //float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);

    float3 shadow = CalcShadowColorPCFFilter(ShadowMap, ShadowSampler, pin.ShadowParam, float3(0.2f, 0.2f, 0.2f), 0.001);
    //directDiffuse *= shadow;
    //directSpecular *= shadow;

    float4 directColor = float4(directDiffuse + directSpecular, albedo.a);
    float alpha = albedo.a;
    
    if (emissiveMax > 0.0f)
    {
        directColor.rgb += emissive;
    }
#if 1
#if 0
    //float4 texColor = texture_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord) * pin.color;
    float4 texColor = albedo;

    float3 direction = float3(0, 1, 0);
    float dirVertex = (dot(pin.world_position, normalize(float4(direction, 1.0))) + 1) / 2;

    // Scanlines
    float scan = step(frac(dirVertex * scanTiling + timer * scanSpeed), 0.5) * 0.65;

    // Glow
    float glow = frac(dirVertex * glowTiling - timer * glowSpeed);

    // Rim Light
    //float3 red = float3(0, 1, 0);
    //L = V;
    //L.y -= 0.1f;
    //float3 rim = CalcRimLight(N, V, L, red, glowBorder);
    //float maxRim = max(rim.r, max(rim.g, rim.b));
    //col2.rgb += maxRim * 1;
    //col2.a = 1.0f;
    //col2.a = m;

    //fixed4 col = texColor * _MainColor + (glow * 0.35 * _MainColor) + rimColor;
    //col.a = texColor.a * _Alpha * (scan + rim + glow) * flicker;
    //float4 edgeColor = float4(1, 0, 0, 1);
    //float4 edgeColor2 = float4(1, 0.25, 0.25, 1);
    float4 edgeColor = pin.color;
    float4 edgeColor2 = pin.color;
    float4 col2 = texColor + (glow * 0.35 * pin.color);
    scan *= step(scanBorder, -pin.world_position.y);
    glow *= step(glowBorder, -pin.world_position.y);
    float alpha2 = step(hologramBorder, -pin.world_position.y);
    col2.rgb = directColor * alpha + (1.0f - alpha) * (scan * edgeColor + glow * edgeColor2);
    //texColor.a *= step(glitchSpeed, -pin.worldPosition.y);
    //col.a = texColor.a * (scan);
    //col.a = scan;
    //col.a = glow;
    // ディゾルブの淵表現
    float edgeValue = saturate(1 - abs(hologramBorder - (-pin.world_position.y)) * (1 / 0.4));
    col2.rgb += edgeColor.rgb * edgeValue;
    //alpha = saturate(alpha + edgeValue);
    col2.a = alpha * (scan + glow + alpha2);
    //float alpha = step(pin.worldPosition.y * glitchSpeed, pin.position.y);
    //float alpha = step(glitchIntensity, pin.position.y + glitchSpeed);
    //float alpha = step(glitchSpeed, -pin.worldPosition.y);
    //col.a += pin.worldPosition.y * glitchSpeed;
    //col.a *= alpha;
#endif
    //float alp = emissiveStrength * (emissiveMax * sin(pin.world_position.z / glitchSpeed + timer));
    float alp = emissiveStrength * (emissiveMax * sin(pin.world_position.z / 25.0f + timer));
#if 0
    if (emissiveStrength > 0.0)
    {
        if (glitchSpeed < 20.0)
        {
            float leng = length(camera_position.xyz - pin.world_position.xyz);
            float len = 10.0f;
            if (leng < len)
            {
                alp = 1.0 - leng / len;
            }
        }
    }
    else
    {
        alp = 1.0f;
    }
#endif

    //directColor.a = alpha + alp;
#endif
    if (directColor.a <= 0.9f)
    {
        directColor.rgb = emissive;
        directColor.a = 1.0f;
    }
    
    float dist = length(pin.world_position.xyz - camera_position.xyz);
    normal = float4(normalize(pin.world_normal.xyz) * 0.5 + 0.5, 1);

    float4 dissolves = dissolve_map.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    float d = (dissolves.x + dissolves.y + dissolves.z) / 3;
    //if (dist < 4 && pin.world_position.y > 0) 
    //{
    //    alpha = dist / 10;
    //    normal = float4(0, 0, 0, 0);
    //}
    //if (d < uvStatus.x * 1.05) 
    //{
    //    diffuse = float3(1, 0, 0);
    //    normal = float4(0, 0, 0, 0);
    //}
    //if (d < uvStatus.x) 
    //{
    //    alpha = 0;
    //    normal = float4(0, 0, 0, 0);
    //}
    PS_OUT ret;
    //ret.Color = float4((diffuse + specular) * shadow, alpha) * pin.color;
    //アウトライン
    //float rate = Outline(pin.texcoord, DepthTexture, NormalTexture, sampler_states[ANISOTROPIC]);
    //color.rgb = lerp(float3(1, 1, 0), color.rgb, rate);
    //ret.Color = col2;
    //ret.Color = float4(col2.rgb, 1.0f);
    ret.Color = directColor;
    ret.Depth = float4(dist, dist, dist, 1);
    ret.Normal = normal;
    ret.Position = float4(pin.world_position.xyz, 1);
    ret.MetalSmoothness = float4(metallic, 0.0f, 0.0f, smoothness);
    ret.AmbientOcclusion = float4(ambientOcculusion, 0.0f, 0.0f, 1.0f);
    ret.Emission = float4(emissive, 1.0f);
    return ret;
}