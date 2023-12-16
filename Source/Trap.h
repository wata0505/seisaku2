#pragma once
#include "Shader.h"


//�g���b�v
class Trap
{
public:
	Trap() {}
	~Trap() {}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;
	//�c���G�t�F�N�g�`��
	virtual void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader) = 0;
	//�f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();
	// �f�o�b�O���\��
	virtual void DrawDebugGUI() = 0;

	//�j��
	void Destroy();


	//�s��X�V����
	void UpdateTransform(int axisType, int lengthType);

	//��ԋ߂��G���G
	bool SearchEnemy(float territoryRange, float radius);
	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);
	void TurnToTarget(float elspsedTime, float speedRate);
	//���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);
	void TurnVertical();

	// �z���O�����V�F�[�_�[��񏉊���
	void HologramShaderDataInitialize(float minHeight, float maxHeight);
	// �z���O�����V�F�[�_�[�X�V����
	void UpdateHologramShader(float elapsedTime, bool isActivate);

	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �p�x�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	//���a�擾
	float GetRadius()const { return radius; }

	//�����擾
	float GetHeight() const { return height; }

	//HP
	int GetHealth() const { return health; }
	int GetMaxHealth() const { return maxHealth; }
	void SetHealth(const int health) { this->health = health; }

	void InputDamage(int damage);

	bool GetHpRenderFlag() const { return hpRenderFlag; }

	enum TrapType
	{
		TrapTurret,
		TrapMine,
		TrapTotem,
		TrapDecoy,

		TrapMax,
	};
	//�G���
	void	SetType(int type) { this->type = type; }
	int		GetType() { return type; }
	bool GetActiveFlag() { return this->activateFlag; }
	void SetActiveFlag(bool activateflag) { this->activateFlag = activateflag; }
protected:

	int type = 0;//�G�̎��

	DirectX::XMFLOAT3		position = { 0,0,0 };
	DirectX::XMFLOAT3		targetPosition = { 0,0,0 };
	DirectX::XMFLOAT3		angle = { 0,0,0 };
	DirectX::XMFLOAT3		scale = { 50,50,50 };
	DirectX::XMFLOAT4X4		transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	int health = 10;
	int maxHealth = 10;
	int attack = 1;
	float radius = 1.0f;
	float height = 1.0f;
	float turnSpeed = 5.0f;

	float dist = 0.0f;

	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float notAttackRange = 5.0f;

	bool center = false;

	bool hpRenderFlag = false;

	bool activateFlag = false;
	
	DirectX::XMFLOAT3 hologramColor = { 0.0f, 1.0f, 0.0f };
	float scanTimer = 0.0f;			// �X�L�������C���̎���
	float scanBorder = 0.0f;		// �X�L�������C���̕`��͈�
	float glowTimer = 0.0f;			// �O���E���C���̎���
	float glowBorder = 0.0f;		// �O���E���C���̕`��͈�
	float hologramTimer = 0.0f;		// �z���O�������C���̎���
	float hologramBorder = 0.0f;	// �z���O�������C���̕`��͈�
	float minHeight = 0.0f;			// �Œ�_
	float maxHeight = 0.0f;			// �ō��_
	float adjustMetalness = 0.0f;	// �����x
	float adjustSmoothness = 0.0f;	// �e��
	float timer = 0.0f;				// �X�V����	
	float glitchSpeed = 50.0f;		// �X�N���[�����x
	float glitchIntensity = 0.0f;	// ���x
	float lerpGlitchIntensity = 0.0f;	// ���x
	float glitchScale = 50.0f;		// �U�ꕝ	
};