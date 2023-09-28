#define SLASHING 0
#define DIFFUSION 1
#define CONVERGENCE 2
#define SLASH 3
#define EXPANSION 4
#define WEPONCONVERGENCE 5
#define CORRUPTION 6
// VS�V�F�[�_�̓��̓f�[�^��`
struct GS_INPUT_GPU2 {
	float3 Position    : POSITION;     // ���_���W(���[���h���W�n)
	float3 Velocity    : VELOCITY;       // ���݂̑��x�x�N�g��(���[���h���W�n)
	float4 ParticleSize: PARTICLESIZE;
	float  Speed       : SPEED;       // ���݂̉����x
	uint   ParticleType: PARTICLETYPE;
	float  Timer       : TIMER;
};

// GS�V�F�[�_�̓��̓f�[�^��`
struct GS_INPUT {
	float4 Pos  : SV_POSITION;     // ���_���W(�ˉe���W�n)
	float3 Dir  : DIRECTION;
	float2 Tex  : TEXTURE;         // �e�N�X�`�����W
	float4 Size : PARTICLESIZE;  // �p�[�e�B�N���̑傫��
	uint   Type : TYPE;
	float  Mer  : MER;
};
cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 View;       // �r���[�ϊ��s��
	float4x4 Projection; // �����ϊ��s��
};
cbuffer CALCULATION_BUFFER : register(b1)
{

	float3 Pattern; // �r���[�ϊ��s��
	float3 Tail;  // �����ϊ��s��
	float  Max;
	float3 UvStatus;
};
cbuffer COLOR_BUFFER : register(b2)
{
	float4 MaterialColor; // �r���[�ϊ��s��
};
// �s�N�Z���E�V�F�[�_�̓��̓f�[�^��`
struct PS_INPUT {
	float4 Pos  : SV_POSITION;     // ���_���W(�ˉe���W�n)
	float2 Tex  : TEXTURE;         // �e�N�X�`�����W
	float2 Color: COLOR;
	float3 wPosition : TEXCOORD2;
	float  uvTimer : UVTIMER;
};

struct PS_OUT {
	float4 Color : SV_TARGET0; // �J���[
	float4 Depth : SV_TARGET1; // ����
	float4 Normal : SV_TARGET2; // �@��
	float4 Position : SV_TARGET3; // ���W
};