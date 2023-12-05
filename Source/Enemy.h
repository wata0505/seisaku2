#pragma once

#include "Shader.h"
#include "Character.h"
#include "Telegram.h"
#include "StateMachine.h"

// �G�l�~�[
class Enemy : public Character
{
public:

	Enemy() {}
	~Enemy() override {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;


	// �j��
	void Destroy();

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	// �f�o�b�O�G�l�~�[���\��
	virtual void DrawDebugGUI();

	// �U���t���O�擾
	virtual bool	GetAttackFlg() { return attackFlg; }
	// �U���t���O�Z�b�g
	virtual void	SetAttackFlg(bool flg) { attackFlg = flg; };
	virtual void	SetId(int id) { this->id = id; }
	virtual int		GetId() { return id; }
	// �U���͈͎擾
	float GetAttackRange() { return attackRange; }

	float GetSearchRange() { return searchRange; }

	// ���b�Z�[�W��M�����Ƃ��̏���
	virtual bool OnMessage(const Telegram& msg);

	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	void UpdateVerticalMove(float elapsedTime);

	void UpdateHorizontalMove(float elapsedTime);

	void UpdateRnderflag();

	void SetTargetPosition(DirectX::XMFLOAT3 t) { targetPosition = t; }

	void ReMove() {};

	bool GetActiveflag() { return activeflag; }
	bool GetRenderflag() { return renderflag; }
	void SetActiveflag(bool flag) { activeflag = flag; }
	void SetReMoveflag(bool flag) { reMoveflag = flag; }

	void SetReMoveTimer(float timer) { reMoveTimer = timer; }

	void SetLowAltitude(float low) { lowAltitude = low; }

	void SetHealth(float heal) { health = static_cast<int>(heal); }

	Animation::Keyframe GetEnemyKeyframe() { return enemyKeyframe; };

	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius, DirectX::XMFLOAT2 pow, float damage,float invincibleTime = 1.0);
	void CollisionNodeVsBase(const char* nodeName, float nodeRadius, DirectX::XMFLOAT2 pow, float damage, float invincibleTime = 1.0);

	virtual DirectX::XMFLOAT3 SearchNodePos(const char* nodeName) = 0;

	int GetEnemyTimer() { return enemyTimer; }

	// �꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	float GetNodeRudius() { return nodeRudius; }

	//�o���A��Ԏ擾
	bool GetBarrierFlag() { return barrierFlg; }
	//�o���A��Ԏ擾
	void SetBarrierFlag(bool barrierFlg) { this->barrierFlg = barrierFlg; }

	// �f�o�b�O�G�l�~�[���\��
	//void DrawDebugGUI();
	// �^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTargetPosition();

	void HitInpnt(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 pow, float damage, float invincibleTime);

protected:
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	Animation::Keyframe enemyKeyframe = {};
	DirectX::XMFLOAT3   nodePosition = {};
	int	state = 0;
	//�U����
	bool attackFlg = false;
	//�o���A���
	bool barrierFlg = false;
	int id = 0;
	//���G�͈�
	float searchRange = 0.0f;
	//�o�g���X�e�[�g�͈�
	float attackRange = 0.0f;
	//�A�N�e�B�u�t���O
	bool  activeflag = true;
	//�ċN��
	bool  reMoveflag = false;
	//�ċN���܂ł̎���
	float  reMoveTimer = 0;
	//���~���E
	float lowAltitude = 2.1f;
	float efLife = 0.3f;
	float efMax = 1.0f;
	float eria = 50.0f;
	bool renderflag = true;
	//�m�[�h�����蔻��
	float nodeRudius = 1.0f;
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	int enemyTimer = 0;
	
};