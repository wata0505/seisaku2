struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    //in uint vertexid : VERTEXID
};

// �W�b�^�[�h���t�g�萔�o�b�t�@
cbuffer JitterDriftConstantBuffer : register(b13)
{
	float jitterStrength;
	float time;
	float2 dummy;
};

// �J���[�O���[�f�B���O
float3 ColorGrading(float3 color)
{
	float contrast = 1.3; //�R���g���X�g
	float saturation = 0.7; // �ʓx
	float3 colorlv = float3(0.9, 1.0, 1.05);

	// �R���g���X�g(���Â̍�)
	color = (color - 0.5) * contrast + 0.5;
	// �ʓx
	float gray = (color.r + color.g + color.b) / 3.0;
	color = (color - gray) * saturation + gray;

	// �F����
	color *= colorlv.rgb;
	return color;
};

// �r�l�b�g
float3 Vignette(float3 color, float2 position)
{
	float vignette = 1.0;
	// ���S����̋���
	float rate = length(position - 0.5);
	rate *= 1.5; // �S�̒���
	rate += (vignette - 1) * 0.5 - 0.1; // ���͈͒���
	rate = pow(rate, 3.0); // �������̕�����
	color = lerp(color, float3(0, 0, 0), saturate(rate));
	return color;
};

