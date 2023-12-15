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

// �~����
static const float PI = 3.141592654f;
// �U�d��(������ł��Œ�4%(0.04f)�͋��ʔ��˂���)
static const float Dielectric = 0.04f;
//-----------------------------------------
// �g�U����BRDF(���K�������o�[�g�̊g�U����)
//-----------------------------------------
// diffuseReflectance : ���ˌ��̂����g�U���˂ɂȂ銄��
float3 DiffuseBRDF(float3 diffuseReflectance)
{
    return diffuseReflectance / PI;
}
//-----------------------------------------
// �t���l����
//-----------------------------------------
// F0 : �������ˎ��̔��˗�
// VdotH : �����x�N�g���ƃn�[�t�x�N�g��(�����ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���̒��ԃx�N�g��)
float3 CalcFresnel(float3 F0, float VdotH)
{
    return F0 + (1 - F0) * ((1 - VdotH) * (1 - VdotH) * (1 - VdotH) * (1 - VdotH) * (1 - VdotH));
}
//-----------------------------------------
// �@�����z�֐�
//-----------------------------------------
// NdotH : �@���x�N�g���ƃn�[�t�x�N�g��(�����ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���̒��ԃx�N�g��)
// roughness : �e��
float CalcNormalDistributionFunction(float NdotH, float roughness)
{
    return (roughness * roughness) / (PI * ((NdotH * NdotH) * ((roughness * roughness) - 1) + 1) * ((NdotH * NdotH) * ((roughness * roughness) - 1) + 1));
}
//-----------------------------------------
// �􉽌������̎Z�o
//-----------------------------------------
// NdotV : �@���x�N�g���Ƒ��x�N�g���Ƃ̓���
// k : �e�� / 2
float CalcGeometryFunction(float NdotV, float k)
{
    return NdotV / (NdotV * (1 - k) + k);
}
// NdotL : �@���x�N�g���ƌ����ւ̃x�N�g���Ƃ̓���
// NdotV : �@���x�N�g���Ǝ����ւ̃x�N�g���Ƃ̓���
// roughness : �e��
float CalcGeometryFunction(float NdotL, float NdotV, float roughness)
{
    float r = roughness * 0.5f;
    float shadowing = CalcGeometryFunction(NdotL, r);
    float masking = CalcGeometryFunction(NdotV, r);
    return shadowing * masking;
}
//-----------------------------------------
// ���ʔ���BRDF(�N�b�N�E�g�����X�̃}�C�N���t�@�Z�b�g���f��)
//-----------------------------------------
// NdotV : �@���x�N�g���Ǝ����ւ̃x�N�g���Ƃ̓���
// NdotL : �@���x�N�g���ƌ����ւ̃x�N�g���Ƃ̓���
// NdotH : �@���x�N�g���ƃn�[�t�x�N�g���Ƃ̓���
// VdotH : �����ւ̃x�N�g���ƃn�[�t�x�N�g���Ƃ̓���
// fresnelIF0 : �������ˎ��̃t���l�����ːF
// roughness : �e��
float3 SpecularBRDF(float NdotV, float NdotL, float NdotH, float VdotH, float3 fresnelIF0, float roughness)
{
    // D��(�@�����z)
    float D = CalcNormalDistributionFunction(NdotH, roughness);
    // G��(�􉽌�����)
    float G = CalcGeometryFunction(NdotL, NdotV, roughness);
    // F��(�t���l������)
    float3 F = CalcFresnel(fresnelIF0, VdotH);

    return (D * G * F) / (4 * (NdotL * NdotV));
}
//--------------------------------------------
//	���ڌ��̕����x�[�X���C�e�B���O
//--------------------------------------------
// diffuseReflectance	: ���ˌ��̂����g�U���˂ɂȂ銄��
// F0					: �������ˎ��̃t���l�����ːF
// normal				: �@���x�N�g��(���K���ς�)
// eyeVector			: ���_�Ɍ������x�N�g��(���K���ς�)
// lightVector			: �����Ɍ������x�N�g��(���K���ς�)
// lightColor			: ���C�g�J���[
// roughness			: �e��
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

    // ���K�V�[�ȃ��C�e�B���O�Ƃ̌݊�����ۂꍇ��PI�ŏ�Z����
    irradiance *= PI;

    // �g�U����BRDF(���K�������o�[�g�̊g�U����)
    outDiffuse = DiffuseBRDF(diffuseReflectance) * irradiance;

    // ���ʔ���BRDF�i�N�b�N�E�g�����X�̃}�C�N���t�@�Z�b�g���f���j
    outSpecular = SpecularBRDF(NdotV, NdotL, NdotH, VdotH, F0, roughness) * irradiance;
}
//--------------------------------------------
// ������BRDF
//--------------------------------------------
// F0			: �������ˎ��̃t���l�����ːF
// roughness	: �e��
// NdotV		: �@���x�N�g���Ǝ����ւ̃x�N�g���Ƃ̓���
float3 EnvBRDFApprox(float3 F0, float roughness, float NdotV)
{
    const	float4	c0 = { -1.0f, -0.0275f, -0.572f, 0.022f };
    const	float4	c1 = { 1.0f,  0.0425f,   1.04f, -0.04f };
    float4	r = roughness * c0 + c1;
    float	a004 = min(r.x * r.x, exp2(-9.28f * NdotV)) * r.x + r.y;
    float2	AB = float2(-1.04f, 1.04f) * a004 + r.zw;
    return	F0 * AB.x + AB.y;
}

// 1 : ���[���h��ԁA 0 : ���f���̃��[�J�����
#define WORLD 0

PS_OUT main(VS_OUT pin)
{
    // �A���x�h�J���[(���������)
    float4 albedo = texture_maps.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    // �A���x�h�J���[�̍ő�l
    float maxColor = max(albedo.r, max(albedo.g, max(albedo.b, albedo.a)));
    // �G�~�b�V�u
    float4 emissive = EmissionTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    
    //emissive.rgb *= emissiveStrength;
    // �G�~�b�V�u�J���[�̍ő�l
    //float emissiveMax = max(emissive.r, max(emissive.g, max(emissive.b, emissive.a)));
    float emissiveMax = max(emissive.r, max(emissive.g, emissive.b));
    // �ő�l��0�Ȃ瓧���s�N�Z���₩��}�e���A���J���[����(��ɃG�~�b�V�u���ǉ��Ŕ���)
#if 0
    if (albedo.a < 0.9f)
    {
        if (emissiveMax <= 0.0f && emissive.a <= 0.0f)
        {
            float maxMaterialColor = min(pin.color.r, min(pin.color.g, pin.color.b));
            if (maxMaterialColor < 0.79f)
            {
                albedo = pin.color;
            }
            else
            {
                float3 emissiveColor = float3(0.0f, 0.5f, 1.0f);
                emissive.rgb = emissiveColor;
                albedo.rgb = emissive.rgb;
            }
        }
    }
#endif
    
    // Inverse gamma process
    const float GAMMA = 1.5f;
    albedo.rgb = pow(albedo.rgb, GAMMA);

    // �����x
    float metallic = MetalSmoothnessTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).r;
    // ���炩��
    float smoothness = MetalSmoothnessTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).a;
    // �����Օ��x
    float ambientOcculusion = AmbientOcclusionTexture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).r;
    // ����
    metallic = saturate(metallic + adjustMetalness);
    smoothness = saturate(smoothness + adjustSmoothness);

    // �e��
    float roughness = 1.0f - smoothness;
    // ���ˌ��̂����g�U���˂ɂȂ銄��
    float3 diffuseReflectance = lerp(albedo.rgb, 0.02f, metallic);
    // �������ˎ��̃t���l�����˗�
    float3 F0 = lerp(Dielectric, albedo.rgb, metallic);

    // �@���}�b�v����xyz�������擾����( -1 �` +1 )�̊ԂɃX�P�[�����O
    float3 N = normalize(pin.world_normal.xyz);
    N = float3(0.0f, 1.0f, 0.0f);
    float3 T = normalize(pin.world_tangent.xyz);
    float sigma = pin.world_tangent.w;
    T = normalize(T - dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);
    float4 normal = Normal_maps.Sample(sampler_states[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
    float3 V = normalize(camera_position.xyz - pin.world_position.xyz);
    float3 L = normalize(-light_direction.xyz);

    // ���C�e�B���O�v�Z
    float3 directDiffuse = 0.0f, directSpecular = 0.0f;
    float3 LightColor = float3(1.0f, 1.0f, 1.0f);
    // ���s�����̏���
    DirectBDRF(diffuseReflectance, F0, N, V, -L, LightColor.rgb, roughness, directDiffuse, directSpecular);

    //float3 diffuse = albedo.rgb * max(0.8f, dot(N, L));
    //float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);

    float3 shadow = CalcShadowColorPCFFilter(ShadowMap, ShadowSampler, pin.ShadowParam, float3(0.2f, 0.2f, 0.2f), 0.001);
    //directDiffuse *= shadow;
    //directSpecular *= shadow;

    float4 directColor = float4(directDiffuse + directSpecular, albedo.a);
    
    // �G�~�b�V�u�K���\�l�Ȃ���Z
    if (emissiveMax > 0.0f)
    {
        directColor.rgb += emissive.rgb;
    }

    // �L����
    if (glitchScale > 0.0f)
    {
        // �ԐF
        float3 red = float3(1.0f, 0.0f, 0.0f);    

        // �z���O�����Ǝ��̂̋��E����
#if WORLD
        // ���[���h��Ԃł̋��E����
        float hologram = step(hologramBorder, -pin.world_position.y);
#else
        // ���f���̃��[�J����Ԃł̋��E����
        float hologram = step(hologramBorder, -pin.localPosition.y);
#endif
        // �z���O�����Ȃ�
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

            // �z���O�������ł̃X�L�����ƃO���E�̋��E����
#if WORLD
            scan *= step(scanBorder, -pin.world_position.y);
            glow *= step(glowBorder, -pin.world_position.y);
#else
            scan *= step(scanBorder, -pin.localPosition.y);
            glow *= step(glowBorder, -pin.localPosition.y);
#endif            
            //scan *= (1.0f - hologram);
            //glow *= (1.0f - hologram);
            directColor.rgb = (scan * hologramColor + glow * (hologramColor * 1.2f));

            // glitchIntensity : ��e or ���S���ɋ��܂�
            // �A���t�@�l = (�x�[�X�J���[�̓��ߒl * (�X�L�������C�� + �O���E���C��)) * (1.0f - �O���b�`���x�̔���)
            directColor.a = (albedo.a * (scan + glow)) * (1.0f - glitchIntensity * 0.5f);
        }
        // ���̂Ȃ�
        else
        {
            // �A���t�@�l = (�x�[�X�J���[�̓��ߒl * �z���O�������C��) * (1.0f - �O���b�`���x�̔���)
            directColor.a = (albedo.a * hologram) * (1.0f - glitchIntensity * 0.5f);
        }

        // �z���O�����Ǝ��̂̕��\��
#if WORLD
        float size = 0.2f;
        float edgeValue = saturate(1.0f - abs(hologramBorder - (-pin.world_position.y)) * (1.0f / size));
#else
        // ���f���̃��[�J����Ԃł̏����̐�����A���f���T�C�Y���傫����Α傫���ق�size�̒l��傫�����Ȃ��ƌ����Ȃ�
        //float size = 5.0f;
        float size = 0.2f;
        float edgeValue = saturate(1.0f - abs(hologramBorder - (-pin.localPosition.y)) * (1.0f / size));
#endif        
        
        // ���f���F += �I�u�W�F�N�g�ʐF�����̂ƃz���O�����̋��E���̕��̕� + �O���b�`�̋������ԐF
        directColor.rgb += hologramColor * edgeValue * 2.0f + red * glitchIntensity;
    }
    
    // �X�e�[�W
    float glitchSpeed = 50.0f;
    float alp = (sin(pin.world_position.z / glitchSpeed + timer));
    if (albedo.a < 0.9f)
    {
        directColor.a = alp;
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
    //�A�E�g���C��
    //float rate = Outline(pin.texcoord, DepthTexture, NormalTexture, sampler_states[ANISOTROPIC]);
    //color.rgb = lerp(float3(1, 1, 0), color.rgb, rate);
    ret.Color = directColor;
    ret.Depth = float4(dist, dist, dist, 1);
    ret.Normal = normal;
    ret.Position = float4(pin.world_position.xyz, 1);
    ret.MetalSmoothness = float4(metallic, 0.0f, 0.0f, smoothness);
    ret.AmbientOcclusion = float4(ambientOcculusion, 0.0f, 0.0f, 1.0f);
    ret.Emission = float4(emissive.rgb, 1.0f);
    return ret;
}