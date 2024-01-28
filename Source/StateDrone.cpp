#include "StateDrone.h"
#include "player.h"
#include "Mathf.h"
#include "Calculation.h"
#include "ParticleSprite.h"
#include "EffectTexAll.h"
#include "Base.h"

// TODO 02_03 State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// Wander�̓T���v���Ƃ��Ă��łɋL�q�ς�
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void DroneWanderState::Enter()
{
	owner->SetRandomTargetPosition();
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
	//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneWanderState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Search::Idle));
	}
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
	// �v���C���[���G
	if (owner->SearchTrap())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Roar));
	}
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneWanderState::Exit()
{
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Walk);
}
//�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void DroneIdleState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
}

// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneIdleState::Execute(float elapsedTime)
{
	// �^�C�}�[����
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	if (owner->GetTutorialflag())return;
	if (owner->GetStateTimer() < 0.0f) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Search::Wander));
	}
	if (owner->SearchTrap()) {
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
	}
}

// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneIdleState::Exit()
{

}

// �ǔ��X�e�[�g�Ŏ��s���郁�\�b�h
void DronePursuitState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	DirectX::XMFLOAT2 dir = owner->ForwardToTarget();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = owner->GetTargetPosition();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	float dot = (dir.x * vx) + (dir.y * vz);
	//�ړI�n���������O�����Ȃ�
	if (1.0 - dot < 0.2) {
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
		owner->SetMoveRate(2.5);
		//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
	}
	else
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
		owner->SetMoveRate(2.5);
		//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
	}
	if (owner->SearchTrap() || owner->GetRootNo() >= owner->GetMaxRootNo()) {
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
}

// �ǔ��X�e�[�g�Ŏ��s���郁�\�b�h
void DronePursuitState::Execute(float elapsedTime)
{
	// �^�C�}�[����
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	if (owner->GetAttackFlg())owner->SetBattleRange(NULL);
	DirectX::XMFLOAT2 dir = owner->ForwardToTarget();
	DirectX::XMFLOAT3 traget = owner->GetTargetPosition();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, owner->GetMoveRate());

	// �^�C�}�[����
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

	// TODO 03 
	// �ǐՎ��Ԃ��o�߂����Ƃ��ҋ@�X�e�[�g�֑J�ڂ��Ȃ���
	if (owner->GetStateTimer() < 0.0f && owner->GetAttackFlg()) {
		//�U���t���O��false�ɐݒ�
		owner->SetAttackFlg(false);
		// �G�l�~�[���烁�^AI�� MsgChangeAttackRight �𑗐M����
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
		owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	}
	DirectX::XMFLOAT2 vec = Vector2::Subset({ owner->GetTargetPosition().x, owner->GetTargetPosition().z }, { owner->GetPosition().x,owner->GetPosition().z });
	float dist = sqrtf(vec.x * vec.x + vec.y * vec.y);;
	if (Vector3::FrameInput(owner->GetEnemyTimer()) && owner->GetAttackFlg()) {
		owner->SetAttackFlg(false);
		// �G�l�~�[���烁�^AI�� MsgChangeAttackRight �𑗐M����
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
	}
	if (dist < 50 && owner->GetAttackFlg() && owner->GetAttackFireBallFlg()) {
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::FireBall));
	}

	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J�ڂ��Ȃ���
	if (dist < owner->GetAttackRange() && owner->GetAttackFlg()) {
		if (owner->SearchTrap() || owner->GetRootNo() >= owner->GetMaxRootNo())
		{
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Attack));
		}
		else
		{
			owner->SetRootNo(owner->GetRootNo() + 1);
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
		}
	}
	else if (dist < owner->GetAttackRange()) {
		if (owner->SearchTrap() || owner->GetRootNo() >= owner->GetMaxRootNo()) {
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Standby));
		}
		else
		{
			owner->SetRootNo(owner->GetRootNo() + 1);
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
		}
	}

}

// �ǔ��X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DronePursuitState::Exit()
{
	owner->SetMoveRate(1.0);
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Walk);
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Run);
}
void DroneAttackState::Enter()
{
	// �U����������΃��[�V�����Đ��J�n
	if (owner->GetAttackFlg())
	{

	}
	owner->SetDamegeRadius(0.2f);

}


//�A�^�b�N�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneAttackState::Execute(float elapsedTime)
{

	owner->FacePlayer(elapsedTime, 1.0);

	// �U����������Ƃ�
	if (owner->GetAttackFlg())
	{


		owner->BeemShoat();

		// �U�����[�V�������I����Ă���Αҋ@�֑J��
		//if (animationTime >= 0.8)
		{
			owner->RandBattleRange();
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
		}
	}
	else
	{

		// �U�������Ȃ��Ƃ��X�e�[�g�ύX
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));

	}



}

// �A�^�b�N�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneAttackState::Exit()
{
	if (owner->GetAttackFlg())
	{
		// TODO 05_07 �U�����I������Ƃ��U�����̕���
		//�U���t���O��false�ɐݒ�
		owner->SetAttackFlg(false);
		// �G�l�~�[���烁�^AI�� MsgChangeAttackRight �𑗐M����
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
	}

}


// TODO 03_03 HierarchicalState���p�������e�X�e�[�g��
// BattleState�N���X�̃����o�֐��̒�`���s���Ȃ����B
// �������̗�Ƃ���SearchState�N���X�͋L�q���Ă����܂��B
// �T�[�`�X�e�[�g�f�X�g���N�^
DroneSearchState::~DroneSearchState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �T�[�`�X�e�[�g�ɓ��������̃��\�b�h
void DroneSearchState::Enter()
{
	SetSubState(static_cast<int>(EnemyDrone::Search::Idle));
}
// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneSearchState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneSearchState::Exit()
{
}


// �o�g���X�e�[�g�f�X�g���N�^
DroneBattleState::~DroneBattleState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �o�g���X�e�[�g�ɓ��������̃��\�b�h
void DroneBattleState::Enter()
{
	SetSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
}
// �o�g���X�e�[�g�Ŏ��s���郁�\�b�h
void DroneBattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// �o�g���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneBattleState::Exit()
{
}
// TODO 05_03 ���^AI���烁�b�Z�[�W����M�����Ƃ��ɌĂ΂��X�e�[�g��ǉ�
// �f�X�g���N�^
DroneRecieveState::~DroneRecieveState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �f�[�^��M�����Ƃ��̃X�e�[�g
void DroneRecieveState::Enter()
{
	// �����X�e�[�g��ݒ�
	SetSubState(static_cast<int>(EnemyDrone::Recieve::Called));
}

// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneRecieveState::Execute(float elapsedTime)
{
	// �q�X�e�[�g���s
	subState->Execute(elapsedTime);
	// 
	if (owner->SearchTrap())
	{
		// Battle�X�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
	}
}
// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneRecieveState::Exit()
{
}
// TODO 05_03 ���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
void DroneCalledState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
	owner->SetStateTimer(5.0f);
}
// �R�[���h�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneCalledState::Execute(float elapsedTime)
{
	// �^�C�}�[����
	float timer = owner->GetStateTimer();
	timer -= elapsedTime;
	owner->SetStateTimer(timer);

	if (timer < 0.0f)
	{
		// �p�j�X�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));


	}
	// �Ώۂ��v���C���[�n�_�ɐݒ�
	//owner->SetTargetPosition(Player::Instance().GetPosition());
	//owner->SetTargetPosition(Base::Instance().GetPos());
	owner->MoveToTarget(elapsedTime, 1.0f);
}
// �R�[���h�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneCalledState::Exit()
{
}

// �f�X�g���N�^
DroneReDamageState::~DroneReDamageState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �_���[�W���͂��ꂽ���̃X�e�[�g
void DroneReDamageState::Enter()
{
	owner->SetGravity(-1.0f);
	// �����X�e�[�g��ݒ�
	SetSubState(static_cast<int>(EnemyDrone::ReDamage::Damege));
	if (owner->GetHealth() < 1)
	{
		SetSubState(static_cast<int>(EnemyDrone::ReDamage::Die));
	}
}

// �_���[�W���̓X�e�[�g�Ŏ��s���郁�\�b�h
void DroneReDamageState::Execute(float elapsedTime)
{
	// �q�X�e�[�g���s
	subState->Execute(elapsedTime);

}
// �_���[�W���̓X�e�[�g����o�Ă����Ƃ��̃��\�b�h

void DroneReDamageState::Exit()
{
}
//�_���[�W�X�e�[�g�ɓ��������̃��\�b�h
void DroneDamageState::Enter()
{
	//�_���[�W�A�j���[�V�����Đ�
	owner->GetModel()->PlayAnimation(EnemyDrone::EnemyDroneAnimation::Idle, false);

}


// �_���[�W�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneDamageState::Execute(float elapsedTime)
{


	if (!owner->GetModel()->IsPlayAnimation())
	{

		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
		if (owner->GetTutorialflag()) {
			owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Search));
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Search::Idle));
		}
	}

}

// �_���[�W�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneDamageState::Exit()
{

}

//���S�X�e�[�g�ɓ��������̃��\�b�h
void DroneDieState::Enter()
{
	// 
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), false);
}


// ���S�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneDieState::Execute(float elapsedTime)
{
	if (!owner->GetModel()->IsPlayAnimation()) {
		//owner->Destroy();
	}
}

// ���S�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneDieState::Exit()
{

}
// �퓬�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void DroneStandbyState::Enter()
{
	if (!owner->GetAttackFlg())
	{
		// �U���������^AI���狁�߂�
		// �G�l�~�[���烁�^AI�� MsgAskAttackRight �𑗐M����
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
	owner->SetStateTimer(Mathf::RandomRange(2.0f, 3.0f));
	//if (rand() % 2 == 0) {
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
	//	rightflag = true;
	//}
	//else
	//{
	//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::WalkFWD), true);
	//	rightflag = false;
	//}
	//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
	owner->SetMoveRate(0.5);
	//owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDroneAnimation::IdleBattle), false);
}
// �퓬�ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void DroneStandbyState::Execute(float elapsedTime)
{
	if (!owner->GetAttackFlg())
	{
		// �U���������^AI���狁�߂�
		// �G�l�~�[���烁�^AI�� MsgAskAttackRight �𑗐M����
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	// �U����������Ƃ�
	if (owner->GetAttackFlg())
	{
		// �U����������Ƃ��X�e�[�g�ύX
		owner->SetBattleRange(0);
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
	}
	owner->BattleMove(rightflag, elapsedTime, owner->GetMoveRate());
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//DirectX::XMFLOAT2 dir = owner->ForwardToPlayer();
	DirectX::XMFLOAT2 dir = owner->ForwardToTarget();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = owner->GetTargetPosition();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist > owner->GetAttackRange() && !owner->GetAttackFlg())
	{
		// �U���͈͂���o����ǐՃX�e�[�g�֑J��
		owner->SetBattleRange(static_cast<float>(rand() % 6 + 1));
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));

	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		if (Vector3::Probability(2) && dist < owner->GetBackStepRange()) {
			//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::BackStep));
		}
	}

}
// �퓬�ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DroneStandbyState::Exit()
{
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Walk);
	owner->SetMoveRate(1.0);
}
