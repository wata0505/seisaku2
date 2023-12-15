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

	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

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
	float attack = 1.0;
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

};