#pragma once

#include<DirectXMath.h>

//�L�����N�^�[
class Character
{
public:
	enum  AxisType
	{
		RHSYUP, //�E��n
		LHSYUP, //����n
		RHSZUP, //�E��n
		LHSZUP //����n
	};
	enum   LengthType
	{
		Cm, //cm�n
		M  //m�n
		
	};
public:
	Character() {}
	virtual ~Character() {}

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	const DirectX::XMFLOAT3 GetEfPos() { return { position.x,position.y + height / 2,position.z }; }

	//��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	//�X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//�X�P�[���擾
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//�s��X�V����
	//i = 0:RHS Y-UP�@�E��n
	//i = 1:LHS Y-UP�@����n
	//i = 2:RHS Z-UP�@�E��n
	//i = 3:LHS Z-UP�@����n
	//j = 0 cm�n
	//j = 1 m�n
	void UpdateTransform(int axisType,int lengthType);

	//���a�擾
	float GetRadius()const { return radius; }

	//�n�ʂɐڒn���Ă��邩
	bool IsGround()const { return isGround; }

	//�d�͒l�ݒ�
	void SetGravity(float g){ gravity = g; }

	//�����擾
	float GetHeight() const { return height; }

	//���ʎ擾
	float GetMass() const { return mass; }

	// �_���[�W��^����
	bool ApplyDamage(int damage, float invincibleTime, bool DamageFlag = true);

	void SetInvincibleTime(float invincibleTime);

	// �Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);


	// ���N��Ԃ��擾
	int GetHealth() const { return health; }

	// �ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return maxHealth; }

	// TODO:02/20 0
	// �m�b�N�o�b�N����
	void KnockBack(Character& taisyou, DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY);

	// �s��̎擾
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	//��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT4X4& angle) { this->transform = angle; }

	//�ő呬�x�ݒ�
	void SetMaxMoveSpeed(float max) { this->maxMoveSpeed = max; }

	int GetSaveDamage() { return saveDamage; }

	void  SetFriction(float fn) { this->friction = fn; }

	bool GetDamageFlag() { return damageFlag; }

	float GetInvincibleTimer() { return invincibleTimer; }

	//���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);
	//�W�����v����
	void JumpInput(float speed);

	DirectX::XMFLOAT3 GetVelocity() { return velocity; };

    void SetVelocity(DirectX::XMFLOAT3 v) { velocity = v; };
	void SetVelocityY(float y) { velocity.y = y; };

	bool GetFallDeath() { return fallDeath; }

	float GetMaxMoveSpeed() { return maxMoveSpeed; }

	// �z���O�����V�F�[�_�[��񏉊���
	void HologramShaderDataInitialize(float minHeight, float maxHeight);
	// �z���O�����V�F�[�_�[�X�V����
	bool UpdateHologramShader(float elapsedTime);

protected:

	//�ړ�����
	//void Move(float elapsedTime, float vx, float vz, float speed);
	void MoveInput(float vx, float vz, float speed);

	


	//�������񏈗�
	void VerticalTurn(float elapsedTime, float vx, float vz, float vy, float speed);


	//���x�����X�V
	void UpdateVelocity(float elapsedTime);

	virtual void OnLanding() {}

	// �_���[�W���󂯂����ɌĂ΂��
	virtual void OnDamaged() {}

	// ���S�������ɌĂ΂��
	virtual void OnDead() {}

	// ���G���ԍX�V
	void UpdateInvincibleTimer(float elapsedTime);

	

private:
	// �������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	virtual void UpdateVerticalMove(float elapsedTime);

	// �������͍X�V����
	void UpdateHorizontalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	virtual void UpdateHorizontalMove(float elapsedTime);

	// ���C�L���X�g�֐��̒��g�ȗ�(�g���܂킵)
	bool OmissionRaycastContents(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMVECTOR Velocity);

protected:
	DirectX::XMFLOAT3		position = { 0,0,0 };
	DirectX::XMFLOAT3		angle = { 0,0,0 };
	DirectX::XMFLOAT3		scale = { 1,1,1 };
	DirectX::XMFLOAT4X4		transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	float radius = 0.7f;
	float gravity = -1.0f;
	DirectX::XMFLOAT3   velocity = { 0,0,0 };
	//�n�ʂƐڒn���Ă��邩
	bool isGround = false;
	float height = 2.0f;
	int health = 5;
	//���G����
	float invincibleTimer = 1.0f;
	//���C
	float friction = 0.9f;
	//������
	float	acceleration = 1.5f;
	bool jumpflag = false;
	//����
	float   mass = 1.0f;
	float	maxMoveSpeed = 5.0f;
	float	moveVecX = 0.0f;
	float	moveVecZ = 0.0f;
	float   stepOffset = 1.0f;
	float   slopeRate = 1.0f;
	//���S����
	float   deathHight = -5;
	//������
	bool    fallDeath = false;
	int     maxHealth = 5;
	//�㏸�̌��E�l
	float   altitude = 15;
	//�_���[�W�ۑ�
	int     saveDamage = 0;
    //���G���ԃt���O
	bool damageFlag = false;

	DirectX::XMFLOAT2 lastPoint = {};
	float scanTimer = 0.0f;			// �X�L�������C���̎���
	float scanBorder = 0.0f;		// �X�L�������C���̕`��͈�
	float glowTimer = 0.0f;			// �O���E���C���̎���
	float glowBorder = 0.0f;		// �O���E���C���̕`��͈�
	float hologramTimer = 0.0f;		// �z���O�������C���̎���
	float hologramBorder = 0.0f;	// �z���O�������C���̕`��͈�
	float minHeight = 0.0f;			// �Œ�_
	float maxHeight = 0.0f;			// �ō��_
	bool isActiveStart = false;		// �z���O�����V�F�[�_�[���s���t���O
};