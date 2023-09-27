#pragma once
#include "Player.h"
#include "MetaAI.h"


// �ҋ@�X�e�[�g�I�u�W�F�N�g
class IdleState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	IdleState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~IdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ړ��X�e�[�g�I�u�W�F�N�g
class MoveState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	MoveState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~MoveState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �W�����v�����X�e�[�g�I�u�W�F�N�g
class JumpStandbyState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	JumpStandbyState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~JumpStandbyState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �W�����v�X�e�[�g�I�u�W�F�N�g
class JumpState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	JumpState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~JumpState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// ���n�X�e�[�g�I�u�W�F�N�g
class LandState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	LandState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~LandState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};


// ����X�e�[�g�I�u�W�F�N�g
class RollState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	RollState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~RollState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �X�e�B���K�[�X�e�[�g�I�u�W�F�N�g
class StingerState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	StingerState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~StingerState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �V�t�g�X�e�[�g�I�u�W�F�N�g
class ShiftState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	ShiftState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~ShiftState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �V�t�g�U���X�e�[�g�I�u�W�F�N�g
class ShiftAttackState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	ShiftAttackState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~ShiftAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �n���o�[�g����X�e�[�g�I�u�W�F�N�g
class LanceState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	LanceState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~LanceState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �`���[�W�X�e�[�g�I�u�W�F�N�g
class ChargeState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	ChargeState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~ChargeState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �`���[�W�U���X�e�[�g�I�u�W�F�N�g
class ChargeAttackState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	ChargeAttackState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~ChargeAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �U���X�e�[�g�I�u�W�F�N�g
class AttackState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	AttackState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~AttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �����U���X�e�[�g�I�u�W�F�N�g
class FallAttackState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	FallAttackState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~FallAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �K�[�h�X�e�[�g�I�u�W�F�N�g
class GuardState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	GuardState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~GuardState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
// �J�E���^�[�����X�e�[�g�I�u�W�F�N�g
class CounterStandbyState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	CounterStandbyState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~CounterStandbyState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �J�E���^�[�X�e�[�g�I�u�W�F�N�g
class CounterState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	CounterState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~CounterState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �e���|�[�g�X�e�[�g�I�u�W�F�N�g
class TeleportState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	TeleportState(Player* player) :HierarchicalState<Player>(player) {};
	// �f�X�g���N�^
	~TeleportState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

//���@�X�e�[�g
class PlayerState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	PlayerState(Player* player) :HierarchicalState<Player>(player) {}
	~PlayerState();
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit();
};
//�_���[�W�X�e�[�g
class DamageState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	DamageState(Player* player) :HierarchicalState<Player>(player) {}
	~DamageState() {};
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//�_�E���X�e�[�g
class DownState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	DownState(Player* player) :HierarchicalState<Player>(player) {}
	~DownState() {};
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//���S�X�e�[�g
class DieState : public HierarchicalState<Player>
{
public:
	// �R���X�g���N�^
	DieState(Player* player) :HierarchicalState<Player>(player) {}
	~DieState() {};
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	/// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
