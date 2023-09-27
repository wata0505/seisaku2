
// VS�V�F�[�_�̓��̓f�[�^��`
struct VS_INPUT {
	float4 Position : SV_POSITION;
	float2 Tex  : TEXTURE;
	float2 Texcoord : TEXCOORD;
	float2 Dissolve :DISSOLVE;
	float  Timer : TIMER;
};

// GS�V�F�[�_�̓��̓f�[�^��`
struct GS_INPUT {
	float4 Pos  : SV_POSITION;     // ���_���W(�ˉe���W�n)
	float4 Color: COLOR;
	float4 Size : SIZE;  // �p�[�e�B�N���̑傫��
	float4 Dis  : DIS;         // �e�N�X�`�����W
	float   Mer  : MER;
};
cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 View;       // �r���[�ϊ��s��
	float4x4 Projection; // �����ϊ��s��
};
#define MAX_INSTANCES 512
cbuffer OBJECT_CONSTANT_BUFFER : register(b1)
{
	float4 World[MAX_INSTANCES];
	float4 MaterialColor[MAX_INSTANCES];
	float4 SpriteSize[MAX_INSTANCES];
	float4 Dissolve[MAX_INSTANCES];
	float4 Timer[MAX_INSTANCES];
};

// �s�N�Z���E�V�F�[�_�̓��̓f�[�^��`
struct PS_INPUT {
	float4 Pos  : SV_POSITION;     // ���_���W(�ˉe���W�n)
	float4 MaterialColor : MATERIALCOLOR;
	float2 Tex  : TEXTURE;         // �e�N�X�`�����W
	float2 Color: COLOR;
	float4 Dissolve: DISSOLVE;

};