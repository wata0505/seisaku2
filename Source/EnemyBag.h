#pragma once
#pragma once
#include "Model.h"
#include "Enemy.h"
#include "StateMachine.h"
#include "AudioSource.h"
#include "ObjectManager.h"
#define BagSE 0.01



class EnemyBag :public Enemy
{
public:
	enum  EnemyBagSE
	{
		Walk,
		Run,
		Max
	};
	// �G�l�~�[�A�j���[�V����
	enum  EnemyBagAnimation
	{
		NULLANI,
		WalkFWD,
		Attack01,
		Attack02,
		Attack03,
		Damage,
		Die1,
	};
	//�G�l�~�[�^�[�Q�b�g
	enum  EnemyTarget
	{
		BaseTarget,
		PlayerTarget,
		TrapTarget,
	};
public:
	EnemyBag(bool tutorial = false,int stag = 0,int enemyType= 0);
	~EnemyBag()override;

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
	void BattleMove(bool leftflag,float elapsedTime, float speedRate);
	//�v���C���[�ւ̕����Ɍ���
	void FacePlayer(float elapsedTime, float speedRate);

	//�v���C���[���G
	bool SearchPlayer();
	//�g���b�v���G
	bool SearchTrap();
	//�t�@�C���{�[���V���b�g
	void FireBallShoat();

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

	StateMachine<EnemyBag>* GetStateMachine() { return stateMachine.get(); }

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

	int GetAttackNodeNo() { return attackNodeNo; }

	void SetAttackNodeNo(int no) {  if(no < 3)attackNodeNo = no; }

	void RootMove();

	float  MovePow();


	//int GetHitAnim(int i) { return hitAnim[i]; }
	//int GetHitAnimMax() { return MAX_HIT_ANIM; };
	int GetAttackAnim(int i) { return attackAnim[i]; }
	int GetAttackAnimMax() { return MAX_ATTACK_ANIM; };

	const char* GetAttackNode(int no) { return attackNode[no]; }

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

	void CollisionFireBallVSPlayer();
protected:
	void OnDead();

	void OnDamaged();

	//void UpdateVerticalMove(float elapsedTime);
	//private:
public:
	// TODO 01 �K�v�ȃX�e�[�g��enum class�ŏ������Ȃ����B
	// �iWander�̂݋L�q�ς݁j
	enum class BagState
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
		FireBall,  //�t�@�C���{�[��
		Standby,   //�U���ҋ@
		Roar,      //����
		BackStep   //�o�b�N�X�e�b�v
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
	std::shared_ptr<Model> model = nullptr;

	static const int MAX_ROOT_POINT{ 5 };

	int rootNo = 0;
	DirectX::XMFLOAT3 rootPoint[3][MAX_ROOT_POINT] = { { {140,-1.5,41}, {140,-1.5,-14} ,{78,-1.5,-5} ,{80,-1.5,-45}, {83,-1.5,-103} },
		                                             { {140,-1.5,40}, {143,-1.5,0} ,{147,-1.5,-30} ,{144,-1.5,-60}, {151,-1.5,-115} },
		                                             { {60,-1.5,40}, {58,-1.5,0} ,{55,-1.5,-30} ,{53,-1.5,-60}, {51,-1.5,-115} } };

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
	int				battleRangeMax = 6;
	//�o�b�N�X�e�b�v�͈�
	float				backStepRange = 2.0f;
	//�����蔻�蔼�a
	float               damegeRadius = 1.8f;
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
	//int hitAnim[MAX_HIT_ANIM] = { static_cast<int>(EnemyBagAnimation::Gethit2) ,static_cast<int>(EnemyBagAnimation::Gethit3) ,static_cast<int>(EnemyBagAnimation::Gethit4) };
	//�U���A�j���[�V�����ő�
	static const int MAX_ATTACK_ANIM{ 3 };
	//�U���A�j���[�V�����i�[
	int attackAnim[MAX_ATTACK_ANIM] = { static_cast<int>(EnemyBagAnimation::Attack01) ,static_cast<int>(EnemyBagAnimation::Attack02) ,static_cast<int>(EnemyBagAnimation::Attack03)};
	//�U���A�j���[�V�����m�[�h�i�[
	const char* attackNode[MAX_ATTACK_ANIM] = { "L_arm" ,"R_arm","R_arm" };
	//�U���A�j���[�V�����m�[�h�i�[
    int attackNodeNo = 0;

	//�X�e�[�g�}�V�[������
	std::shared_ptr<StateMachine<EnemyBag>> stateMachine = nullptr;
	//�`����i�[
	std::vector<SkinnedMeshResouurce::constants> renderdata;

	std::unique_ptr<AudioSource> se[(int)EnemyBagSE::Max];
	//�t�@�C���{�[���}�l�[�W���[
	ObjectManager objectManager;

	float adjustMetalness = 0.0f;	// �����x
	float adjustSmoothness = 0.0f;	// �e��
	float timer = 0.0f;				// �X�V����	
	float glitchSpeed = 50.0f;		// �X�N���[�����x
	float glitchIntensity = 0.0f;	// ���x
	float lerpGlitchIntensity = 0.0f;	// ���x
	float glitchScale = 50.0f;		// �U�ꕝ	
	float deathTimer = 0.0f;		// ���S����

	int colorType = 0;

	int stagNo = 0;
};
