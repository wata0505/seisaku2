#pragma once

#include <DirectXMath.h>

class CameraController
{
public:
	CameraController() {}
	~CameraController() {}

	// �X�V����
	void Update(float elapsedTime);
	// ���b�N�I���X�V����
	void LockOnUpdate(float elapsedTime, DirectX::XMFLOAT3 front, float Length);
	// ��ʐU���X�V����
	void ShakeUpdate(DirectX::XMFLOAT3& eye,float elapsedTime);
	// �^�C�g���X�V����
	void TitleUpdate(float elapsedTime);
	// �Q�[���J�n�X�V����
	void EntryUpdate(float elapsedTime);
	// �Q�[���I���X�V����
	void GameAfterUpdate(float elapsedTime);

	// �f�o�b�O���\��
	void DrawDebugGUI();

public:
	// �^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
	void SetTarget2(const DirectX::XMFLOAT3& target) { this->target2 = target; }
	float GetAngleY() { return this->angle.y; }

	void SetHitPow(float pow) { hitPow = pow/100; }
	void SetHitTimer(float timer) { shakeTimer = timer / 10; }

	void SetHitDir(DirectX::XMFLOAT3 dir) { hitDir = dir; }

	// �J���������ݒ�
	void SetRange(float range) { this->range = range; }
	// �J�����ő勗���ݒ�
	void SetRangeMax(float rangeMax) { this->rangeMax = rangeMax; }

	void SetCorrectionSpeed(float correctionSpeed) { this->correctionSpeed = correctionSpeed; }

private:
	DirectX::XMFLOAT3	target = {};
	DirectX::XMFLOAT3	target2 = {};
	DirectX::XMFLOAT3	angle = {};										// �p�x
	DirectX::XMFLOAT3	hitDir = {};
	DirectX::XMFLOAT3   cameraEye = {};
	float				rollSpeed = DirectX::XMConvertToRadians(90);	// ��]���x
	float				correctionSpeed = 20;
	float				range = 5.0f;									// �J��������
	float				rangeMax = 5.0f;								// �J�����ő勗��
	float				heightRange = 0.0f;
	float				eyeheight = 0.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(0);		// X���ő�p�x
	float				minAngleX = DirectX::XMConvertToRadians(0);		// X���ŏ��p�x
	float				maxAngle = DirectX::XMConvertToRadians(35);
	float				minAngle = DirectX::XMConvertToRadians(-15);
	float               hitPow = 0;
	float               shakeTimer = 0;		// �h�ꎞ��
	float               correction = 0.03f;
	float               lengthMax = 15;
	float               lengthMin = 1;
	float               lengthCorrection = 0.1f;
	float				entryTimer = 0.0f;			// �Q�[���J�n���o���`�⊮����
	bool                isShake = true;		// ��ʐU�������t���O
};