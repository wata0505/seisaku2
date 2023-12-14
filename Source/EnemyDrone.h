#pragma once
#include "Model.h"
#include "Enemy.h"
#include "StateMachine.h"
#include "AudioSource.h"
#include "ObjectManager.h"
#define BagSE 0.01



class EnemyDrone :public Enemy
{
public:
	enum  EnemyDroneSE
	{
		Walk,
		Run,
		Roar,
		Max
	};
	// �G�l�~�[�A�j���[�V����
	enum  EnemyDroneAnimation
	{
		Idle,
	};
	//�G�l�~�[�^�[�Q�b�g
	enum  EnemyTarget
	{
		BaseTarget,
		TrapTarget,
	};
public:
	EnemyDrone(bool tutorial = false);
	~EnemyDrone()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)override;
	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;


	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);
	//�v���C���[�𒆐S�ɉ�]
	void BattleMove(bool leftflag, float elapsedTime, float speedRate);
	//�v���C���[�ւ̕����Ɍ���
	void FacePlayer(float elapsedTime, float speedRate);

	//�g���b�v���G
	bool SearchTrap();

	//�t�@�C���{�[���V���b�g
	void BeemShoat();

	void TargetUpdate();

	// �v���C���[�ւ̕����擾
	DirectX::XMFLOAT2 ForwardToPlayer();

	DirectX::XMFLOAT2 ForwardToTarget();

	// TODO 05_02 ���b�Z�[�W��M�֐���ǉ�
	bool OnMessage(const Telegram& msg);

	void DrawDebugGUI() override;


	//// �^�[�Q�b�g�|�W�V�����ݒ�
	//void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }
	//
	//// �^�[�Q�b�g�|�W�V�����擾
	//DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	// �|�W�V�����擾
	DirectX::XMFLOAT3 GetPosition() { return position; }



	// �X�e�[�g�^�C�}�[�ݒ�
	void SetStateTimer(float timer) {
		stateTimer = timer;
	}
	// �X�e�[�g�^�C�}�[�擾
	float GetStateTimer() { return stateTimer; }
	//�o�g���ҋ@���a�擾
	float GetBattleRange() { return battleRange; }
	//�o�g���ҋ@���a�Z�b�g
	void  SetBattleRange(float BattleRange) { battleRange = BattleRange; }
	//�v���C���[�Ƃ̋����������_���Ō���
	void  RandBattleRange() { battleRange = rand() % battleRangeMax + 1.0f; }

	StateMachine<EnemyDrone>* GetStateMachine() { return stateMachine.get(); }

	Model* GetModel() { return model.get(); }



	float GetMoveRate() { return moveRate; }

	void SetMoveRate(float rate) { moveRate = rate; }

	void SetDamegeRadius(float radius) { damegeRadius = radius; }

	float GetDamegeRadius() { return damegeRadius; }
	//�������U�����擾
	bool GetAttackFireBallFlg() { return attackFireBallFlg; }
	//�������U�����ݒ�
	void SetAttackFireBallFlg(bool attackFireBallFlg) { this->attackFireBallFlg = attackFireBallFlg; }

	float GetBackStepRange() { return backStepRange; }

	int GetFlameTimer() { return enemyTimer / 60; }


	void RootMove();

	float  MovePow();


	void PlaySe(int Index, bool loop) { se[Index]->Play(loop, static_cast<float>(BagSE)); }
	void StopSe(int Index) { se[Index]->Stop(); }
	//�`���[�g���A���t���O�擾
	bool GetTutorialflag() { return tutorialflag; }

	DirectX::XMFLOAT3 SearchNodePos(const char* nodeName);

	void ReMove();

	int GetRootNo() { return rootNo; }
	int GetMaxRootNo() { return MAX_ROOT_POINT; }

	void SetRootNo(int no) { rootNo = no; }
private:
	void SeUpdate(float elapsedTime);

	void CollisionBeemVSTrap();
	void CollisionBeemVSBase();

protected:
	void OnDead();

	void OnDamaged();

	//void UpdateVerticalMove(float elapsedTime);
	//private:
public:
	// TODO 01 �K�v�ȃX�e�[�g��enum class�ŏ������Ȃ����B
	// �iWander�̂݋L�q�ς݁j
	enum class DroneState
	{
		Search,   //���G
		Battle,	  //�퓬
		Recieve,  //�Ăъ�
		ReDamage, //�_���[�W�󂯂���
	};
	enum class Search
	{
		// TODO 03_01 SearchState�Ɋ܂܂��T�u�X�e�[�g���L�q���Ȃ���
		Wander,  //�p�j
		Idle,	 //�ҋ@
	};
	enum class Battle
	{
		// TODO 03_01 BattleState�Ɋ܂܂��T�u�X�e�[�g���L�q���Ȃ���
		Pursuit,   //�ǔ�
		Attack,    //�U��
		Standby,   //�U���ҋ@
	};
	// TODO 05_02 MetaAI���o�R���đ��̃G�l�~�[����Ă΂ꂽ���̏���
	enum class Recieve
	{
		Called,   //�Ăъ�
	};
	enum class ReDamage
	{
		Damege,  //�_���[�W��
		Die,	 //���S
	};
private:
	std::shared_ptr<Model> beem;
	std::shared_ptr<Model> model = nullptr;

	static const int MAX_ROOT_POINT{ 4 };

	int rootNo = 0;
	DirectX::XMFLOAT3 rootPoint[MAX_ROOT_POINT] = { {129,-2.5,41}, {128,-2.5,-8.5} ,{78,-2.5,-5} ,{80,-2.5,-45} };

	//�������U����
	bool attackFireBallFlg = false;
	float				moveSpeed = 4.5f;
	//�������x
	float				moveRate = 3.0f;
	//�o�b�N���x
	float			    workRate = 0.5f;
	//���鑬�x
	float			    runRate = 1.5f;
	//�`���[�g���A���p
	bool                tutorialflag = false;
	float				turnSpeed = DirectX::XMConvertToRadians(360);
	float				stateTimer = 0.0f;
	//�o�g���X�e�[�g�͈�
	float				battleRange = 6.0f;
	//�o�g���X�e�[�g�͈�MAX
	int				battleRangeMax = 6.0f;
	//�o�b�N�X�e�b�v�͈�
	float				backStepRange = 2.0f;
	//�����蔻�蔼�a
	float               damegeRadius = 1.0f;
	//��]�X�s�[�h
	float               rootSpeed = 0.8f;
	//�q�b�g�X�g�b�v
	float               hitStop = 0.0;
	//�o���AHP
	int                 barrierHP = 5;
	DirectX::XMFLOAT4X4 world = {};
	//�^�[�Q�b�gNo
	int targetNo = 0;
	//�q�b�g�A�j���[�V�����ő�
	static const int MAX_HIT_ANIM{ 3 };
	//�q�b�g�A�j���[�V�����i�[
	//int hitAnim[MAX_HIT_ANIM] = { static_cast<int>(EnemyDroneAnimation::Gethit2) ,static_cast<int>(EnemyDroneAnimation::Gethit3) ,static_cast<int>(EnemyDroneAnimation::Gethit4) };

	//�X�e�[�g�}�V�[������
	std::shared_ptr<StateMachine<EnemyDrone>> stateMachine = nullptr;
	//�`����i�[
	std::vector<SkinnedMeshResouurce::constants> renderdata;

	std::unique_ptr<AudioSource> se[(int)EnemyDroneSE::Max];
	//�t�@�C���{�[���}�l�[�W���[
	ObjectManager objectManager;

	float adjustMetalness = -1.0f;	// �����x
	float adjustSmoothness = 0.0f;	// �e��
	float timer = 0.0f;				// �X�V����	
	float glitchSpeed = 50.0f;		// �X�N���[�����x
	float glitchIntensity = 0.0f;	// ���x
	float lerpGlitchIntensity = 0.0f;	// ���x
	float glitchScale = 50.0f;		// �U�ꕝ	
	float deathTimer = 0.0f;		// ���S����

	int coolTime = 0;
};
#pragma once
