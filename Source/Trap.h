#pragma once

#include "Shader.h"

class Trap
{
public:
	Trap() {}
	~Trap() {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) = 0;
	// �c���G�t�F�N�g�`�揈��
	virtual void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader) = 0;
	// �f�o�b�O�v���~�e�B�u�`�揈��
	virtual void DrawDebugPrimitive();
	// �f�o�b�O���\��
	virtual void DrawDebugGUI() = 0;

	// �s��X�V����
	void UpdateTransform(int axisType, int lengthType);

	// �j��
	void Destroy();

	// ��ԋ߂��G���G
	bool SearchEnemy(float territoryRange, float radius);

	// ���񏈗�
	void TurnToTarget(float elspsedTime, float speedRate);
	// XZ���ʐ��񏈗�
	void TurnHorizontal(float elapsedTime, float vx, float vz, float speed);
	// Y�����񏈗�
	void TurnVertical();

	// �_���[�W����
	void InputDamage(int damage);

	// �z���O�����V�F�[�_�[��񏉊���
	void HologramShaderDataInitialize(float minHeight, float maxHeight);
	// �z���O�����V�F�[�_�[�X�V����
	void UpdateHologramShader(float elapsedTime, bool isActivate);

public:
	// �s��擾
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// �ʒu�ݒ�Ǝ擾
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �p�x�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// ���a�擾
	float GetRadius()const { return radius; }

	// �����擾
	float GetHeight() const { return height; }

	// HP�`��t���O�擾
	//bool GetHpRenderFlag() const { return hpRenderFlag; }

	// �ݒu�O��t���O�ݒ�Ǝ擾
	void SetIsActive(bool isActivate) { this->isActivate = isActivate; }
	bool GetIsActive() { return isActivate; }

	// �C���X�^���V���O�擾
	bool GetInstancing() { return instancing; }

	// HP�ݒ�Ǝ擾
	void SetHealth(const int health) { this->health = health; }
	int GetHealth() const { return health; }
	int GetMaxHealth() const { return maxHealth; }	

	// �g���b�v��ސݒ�Ǝ擾
	void SetType(int type) { this->type = type; }
	int GetType() { return type; }

	
	float GetHologramTimer() { return hologramTimer; }
	float GetScanGlowBorder() { return scanBorder; }
	float GetHologramBorder() { return hologramBorder; }
	float GetHologramColor() { return hologramColor; }
	float GetYUp() { return yUp; }

public:
	// �g���b�v���
	enum TrapType
	{
		TrapTurret,	// �^���b�g
		TrapMine,	// �n��
		TrapTotem,	// �g�[�e��
		TrapDecoy,	// �f�R�C

		TrapMax,
	};

protected:
	DirectX::XMFLOAT4X4		transform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };		// �s��
	DirectX::XMFLOAT3		position = {};											// �ʒu
	DirectX::XMFLOAT3		targetPosition = {};									// �^�[�Q�b�g�ʒu
	DirectX::XMFLOAT3		angle = {};												// �p�x
	DirectX::XMFLOAT3		scale = { 50.0f, 50.0f, 50.0f };						// �T�C�Y
	DirectX::XMFLOAT3		territoryOrigin = { 0.0f, 0.0f, 0.0f };					// �e���g���[�ʒu
	float					radius = 1.0f;											// ���a
	float					height = 1.0f;											// ����
	float					turnSpeed = 5.0f;										// ���񑬓x
	float					territoryRange = 10.0f;									// �e���g���[�͈�
	float					notAttackRange = 5.0f;									// �U���s�͈�
	float					dist = 0.0f;											// ����
	float					hologramColor = 1.0f;									// �z���O�����F
	float					scanTimer = 0.0f;										// �X�L�������C���̎���
	float					scanBorder = 0.0f;										// �X�L�������C���̕`��͈�
	float					glowTimer = 0.0f;										// �O���E���C���̎���
	float					glowBorder = 0.0f;										// �O���E���C���̕`��͈�
	float					hologramTimer = 0.0f;									// �z���O�������C���̎���
	float					hologramBorder = 0.0f;									// �z���O�������C���̕`��͈�
	float					minHeight = 0.0f;										// �Œ�_
	float					maxHeight = 0.0f;										// �ō��_
	float					progressTimer = 0.0f;									// �o�ߎ���	
	float					glitchIntensity = 0.0f;									// ���x
	float					glitchScale = 50.0f;									// �U�ꕝ
	float					yUp = 1;
	float					brokenTimer = 1.0f;										// �j�󎞂̔j���܂ł̎���
	//bool					hpRenderFlag = false;									// HP�`��t���O
	bool					isActivate = false;										// �ݒu�O��
	int						health = 10;											// �̗�
	int						maxHealth = 10;											// �ő�̗�
	int						attack = 1;												// �_���[�W��
	int						type = 0;												// �G�̎��
	bool					instancing = false;										// �C���X�^���V���O
};