#pragma once
#include "EnemyBag.h"
#include "MetaAI.h"


// �p�j�X�e�[�g�I�u�W�F�N�g
class BagWanderState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagWanderState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagWanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class BagIdleState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagIdleState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ǐՃX�e�[�g�I�u�W�F�N�g
class BagPursuitState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagPursuitState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagPursuitState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �U���X�e�[�g�I�u�W�F�N�g
class BagAttackState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagAttackState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//�t�@�C���{�[���X�e�[�g�I�u�W�F�N�g
class BagFireBallState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagFireBallState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagFireBallState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//�T�[�`�X�e�[�g
class BagSearchState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagSearchState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagSearchState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};
class BagBattleState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagBattleState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagBattleState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};
//���K�X�e�[�g
class BagRoarState: public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagRoarState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagRoarState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

class BagRecieveState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagRecieveState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagRecieveState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};

//�_���[�W���̓X�e�[�g
class BagReDamageState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagReDamageState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagReDamageState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};
//�_���[�W�X�e�[�g
class BagDamageState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagDamageState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagDamageState() {};
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//���S�X�e�[�g
class BagDieState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagDieState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagDieState() {};
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	/// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
class BagCalledState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagCalledState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagCalledState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �퓬�ҋ@�X�e�[�g�I�u�W�F�N�g
class BagStandbyState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagStandbyState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagStandbyState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	bool rightflag = false;
};
// �o�b�N�X�e�b�v�X�e�[�g�I�u�W�F�N�g
class BagBackStepState : public HierarchicalState<EnemyBag>
{
public:
	// �R���X�g���N�^
	BagBackStepState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// �f�X�g���N�^
	~BagBackStepState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	bool rightflag = false;
};