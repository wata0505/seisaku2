#pragma once
#include "EnemyDrone.h"
#include "MetaAI.h"


// �p�j�X�e�[�g�I�u�W�F�N�g
class DroneWanderState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneWanderState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// �f�X�g���N�^
	~DroneWanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class DroneIdleState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneIdleState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// �f�X�g���N�^
	~DroneIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ǐՃX�e�[�g�I�u�W�F�N�g
class DronePursuitState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DronePursuitState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// �f�X�g���N�^
	~DronePursuitState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �U���X�e�[�g�I�u�W�F�N�g
class DroneAttackState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneAttackState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// �f�X�g���N�^
	~DroneAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

//�T�[�`�X�e�[�g
class DroneSearchState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneSearchState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneSearchState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};
class DroneBattleState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneBattleState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneBattleState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};

class DroneRecieveState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneRecieveState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneRecieveState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};

//�_���[�W���̓X�e�[�g
class DroneReDamageState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneReDamageState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneReDamageState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};
//�_���[�W�X�e�[�g
class DroneDamageState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneDamageState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneDamageState() {};
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//���S�X�e�[�g
class DroneDieState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneDieState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneDieState() {};
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	/// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
class DroneCalledState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneCalledState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// �f�X�g���N�^
	~DroneCalledState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �퓬�ҋ@�X�e�[�g�I�u�W�F�N�g
class DroneStandbyState : public HierarchicalState<EnemyDrone>
{
public:
	// �R���X�g���N�^
	DroneStandbyState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// �f�X�g���N�^
	~DroneStandbyState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	bool rightflag = false;
};
