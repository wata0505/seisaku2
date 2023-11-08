#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;	// ����
	DirectX::XMFLOAT4 color;		// �F
};

// �F���␳���
struct ColorGradingData
{
	float hueShift = 0;		// �F������
	float saturation = 1;	// �ʓx����
	float brightness = 1;	// ���x����
	float dummy;
};

// �K�E�X�t�B���^�[�v�Z���
struct GaussianFilterData
{
	int					KernelSize = 8;		// �J�[�l���T�C�Y
	float				deviation = 10.0f;	// �W���΍�
	DirectX::XMFLOAT2	textureSize;		// �ڂ����e�N�X�`���̃T�C�Y
};

// �K�E�X�t�B���^�[�̍ő�J�[�l���T�C�Y
static const int MaxKernelSize = 16;

// ���P�x���o���
struct LuminanceExtractionData
{
	float threshold = 0.5f;
	float intensity = 1.0f;
	DirectX::XMFLOAT2 dummy2;
};

// �W�b�^�[�h���t�g���
struct JitterDriftData
{
	float jitterStrength = 0.0f;
	float time = 0.0f;
	DirectX::XMFLOAT2 dummy;
};

// �|�X�g�G�t�F�N�g�̍ŏI�p�X�p���
struct FinalpassData
{
	// �u���[���e�N�X�`��
	ID3D11ShaderResourceView* bloomTexture;
};
// �V���h�E�}�b�v�p���
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap; // �V���h�E�}�b�v�e�N�X�`��
	DirectX::XMFLOAT4X4 lightViewProjection; // ���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 shadowColor; // �e�̐F
	float shadowBias; // �[�x��r�p�̃I�t�Z�b�g�l
};
struct FogData
{
	DirectX::XMFLOAT4 fog_color;
	DirectX::XMFLOAT4 fog_range;	// x : near, y : far, zw : ��
};


// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4		CameraPosition;
	// ���C�g���
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData directionalLightData;

	// �F���␳���
	ColorGradingData colorGradingData;
	// �V���h�E�}�b�v���
	ShadowMapData shadowMapData;
	//�t�H�O
	FogData fogData;

	FinalpassData finalpassData;

	LuminanceExtractionData luminanceExtractionData;

	JitterDriftData jitterDriftData;

	DirectX::XMFLOAT4 color;
};