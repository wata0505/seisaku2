#include "StateDerived.h"
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
void BagWanderState::Enter()
{
	owner->SetRandomTargetPosition();
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	owner->PlaySe((int)EnemyBag::EnemyBagSE::Walk,true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void BagWanderState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	DirectX::XMFLOAT3 targetPosition = Base::Instance().GetPos();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Search::Idle));
	}
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
	// �v���C���[���G
	if (owner->SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Roar));
	}
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagWanderState::Exit()
{
	owner->StopSe((int)EnemyBag::EnemyBagSE::Walk);
}
//�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void BagIdleState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
}

// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void BagIdleState::Execute(float elapsedTime)
{
	// �^�C�}�[����
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	if (owner->GetTutorialflag())return;
	if (owner->GetStateTimer() < 0.0f) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Search::Wander));
	}
	if (owner->SearchPlayer()) {
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
	}
}

// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagIdleState::Exit()
{

}
//���K�X�e�[�g�ɓ��������̃��\�b�h
void BagRoarState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), false);
	owner->PlaySe((int)EnemyBag::EnemyBagSE::Roar, false);
}

// ���K�X�e�[�g�Ŏ��s���郁�\�b�h
void BagRoarState::Execute(float elapsedTime)
{
	// �^�C�}�[����
	if (!owner->GetModel()->IsPlayAnimation()) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
}

// ���K�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagRoarState::Exit()
{

}
// �ǔ��X�e�[�g�Ŏ��s���郁�\�b�h
void BagPursuitState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	DirectX::XMFLOAT2 dir = owner->ForwardToBase();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = Base::Instance().GetPos();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx  + vz * vz);
	vx /= dist;
	vz /= dist;

	float dot = (dir.x * vx) + (dir.y * vz);
	//�ړI�n���������O�����Ȃ�
	if (1.0 - dot < 0.2) {
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
		owner->SetMoveRate(1.5);
		owner->PlaySe((int)EnemyBag::EnemyBagSE::Run, true);
	}
	else
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
		owner->SetMoveRate(0.5);
		owner->PlaySe((int)EnemyBag::EnemyBagSE::Walk, true);
	}
	Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgCallHelp);
}

// �ǔ��X�e�[�g�Ŏ��s���郁�\�b�h
void BagPursuitState::Execute(float elapsedTime)
{
	// �^�C�}�[����
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	if (owner->GetAttackFlg())owner->SetBattleRange(NULL);
	DirectX::XMFLOAT2 dir = owner->ForwardToBase();
	DirectX::XMFLOAT3 traget = Base::Instance().GetPos();
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
	DirectX::XMFLOAT3 vec = Vector3::Subset(owner->GetTargetPosition(), owner->GetPosition());
	float dist = Vector3::Lenght(vec);
	if (Vector3::FrameInput(owner->GetEnemyTimer()) && owner->GetAttackFlg()) {
		owner->SetAttackFlg(false);
		// �G�l�~�[���烁�^AI�� MsgChangeAttackRight �𑗐M����
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
	}
	if (dist < 50 && owner->GetAttackFlg() && owner->GetAttackFireBallFlg()) {
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::FireBall));
	}
	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J�ڂ��Ȃ���
	if (dist < owner->GetAttackRange() && owner->GetAttackFlg()) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Attack));
	}
	else if (dist < owner->GetAttackRange()) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Standby));
	}

}

// �ǔ��X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagPursuitState::Exit()
{
	owner->SetMoveRate(1.0);
	owner->StopSe((int)EnemyBag::EnemyBagSE::Walk);
	owner->StopSe((int)EnemyBag::EnemyBagSE::Run);
}
void BagAttackState::Enter()
{
	// �U����������΃��[�V�����Đ��J�n
	if (owner->GetAttackFlg())
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(owner->GetAttackAnim(rand() % owner->GetAttackAnimMax())), false);
	}
	owner->SetDamegeRadius(0.2);
	ParticleSprite* particleSprite = new ParticleSprite(owner->SearchNodePos("atama"), { NULL,NULL,NULL }, ParticleSprite::ParticleBurst, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::BlackThunder, 1, 0.3);
}


//�A�^�b�N�X�e�[�g�Ŏ��s���郁�\�b�h
void BagAttackState::Execute(float elapsedTime)
{

	// �U����������Ƃ�
	if (owner->GetAttackFlg())
	{
		owner->CollisionNodeVsPlayer("R_arm",owner->GetDamegeRadius(), { 10,0 },1);
		owner->CollisionNodeVsBase("R_arm", owner->GetDamegeRadius(), { 10,0 }, 1);
		// �U�����[�V�������I����Ă���Αҋ@�֑J��
		if (!owner->GetModel()->IsPlayAnimation())
		{

			owner->RandBattleRange();
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
			//if (Vector3::Probability(2)) {//�m���Ńo�b�N�X�e�b�v
			//	owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::BackStep));
			//}
		}
	}
	else
	{
		
		// �U�������Ȃ��Ƃ��X�e�[�g�ύX
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	
	}


}

// �A�^�b�N�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagAttackState::Exit()
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

void BagFireBallState::Enter()
{
	// �U����������΃��[�V�����Đ��J�n
	if (owner->GetAttackFlg())
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), false);
	}
	ParticleSprite* particleSprite = new ParticleSprite(owner->SearchNodePos("atama"), { NULL,NULL,NULL }, ParticleSprite::ParticleBurst, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::BlackThunder, 1, 0.3);
}


//�A�^�b�N�X�e�[�g�Ŏ��s���郁�\�b�h
void BagFireBallState::Execute(float elapsedTime)
{
	owner->FacePlayer(elapsedTime,1.0);

	// �U����������Ƃ�
	if (owner->GetAttackFlg())
	{
		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		bool attackCollisionFlag = animationTime >= 0.68 && animationTime <= 0.69;
		if (attackCollisionFlag)
		{
			owner->FireBallShoat();
		}
		// �U�����[�V�������I����Ă���Αҋ@�֑J��
		if (animationTime >= 0.8)
		{
			owner->RandBattleRange();
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
		}
	}
	else
	{

		// �U�������Ȃ��Ƃ��X�e�[�g�ύX
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));

	}


}

// �A�^�b�N�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagFireBallState::Exit()
{
	owner->SetAttackFireBallFlg(false);
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
BagSearchState::~BagSearchState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �T�[�`�X�e�[�g�ɓ��������̃��\�b�h
void BagSearchState::Enter()
{
	SetSubState(static_cast<int>(EnemyBag::Search::Idle));
}
// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void BagSearchState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagSearchState::Exit()
{
}


// �o�g���X�e�[�g�f�X�g���N�^
BagBattleState::~BagBattleState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �o�g���X�e�[�g�ɓ��������̃��\�b�h
void BagBattleState::Enter()
{
		SetSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
}
// �o�g���X�e�[�g�Ŏ��s���郁�\�b�h
void BagBattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// �o�g���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagBattleState::Exit()
{
}
// TODO 05_03 ���^AI���烁�b�Z�[�W����M�����Ƃ��ɌĂ΂��X�e�[�g��ǉ�
// �f�X�g���N�^
BagRecieveState::~BagRecieveState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �f�[�^��M�����Ƃ��̃X�e�[�g
void BagRecieveState::Enter()
{
	// �����X�e�[�g��ݒ�
	SetSubState(static_cast<int>(EnemyBag::Recieve::Called));
}

// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void BagRecieveState::Execute(float elapsedTime)
{
	// �q�X�e�[�g���s
	subState->Execute(elapsedTime);
	// 
	if (owner->SearchPlayer())
	{
		// Battle�X�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
}
// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagRecieveState::Exit()
{
}
// TODO 05_03 ���̃G�l�~�[����Ă΂ꂽ�Ƃ��̃X�e�[�g��ǉ�
void BagCalledState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	owner->SetStateTimer(5.0f);
}
// �R�[���h�X�e�[�g�Ŏ��s���郁�\�b�h
void BagCalledState::Execute(float elapsedTime)
{
	// �^�C�}�[����
	float timer = owner->GetStateTimer();
	timer -= elapsedTime;
	owner->SetStateTimer(timer);

	if (timer < 0.0f)
	{
		// �p�j�X�e�[�g�֑J��
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
	

	}
	// �Ώۂ��v���C���[�n�_�ɐݒ�
	//owner->SetTargetPosition(Player::Instance().GetPosition());
	owner->SetTargetPosition(Base::Instance().GetPos());
	owner->MoveToTarget(elapsedTime, 1.0f);
}
// �R�[���h�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagCalledState::Exit()
{
}

// �f�X�g���N�^
BagReDamageState::~BagReDamageState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// �_���[�W���͂��ꂽ���̃X�e�[�g
void BagReDamageState::Enter()
{
	owner->SetGravity(-1.0f);
	// �����X�e�[�g��ݒ�
	SetSubState(static_cast<int>(EnemyBag::ReDamage::Damege));
	if (owner->GetHealth() < 1) {
		SetSubState(static_cast<int>(EnemyBag::ReDamage::Die));
	}
}

// �_���[�W���̓X�e�[�g�Ŏ��s���郁�\�b�h
void BagReDamageState::Execute(float elapsedTime)
{
	// �q�X�e�[�g���s
	subState->Execute(elapsedTime);

}
// �_���[�W���̓X�e�[�g����o�Ă����Ƃ��̃��\�b�h

void BagReDamageState::Exit()
{
}
//�_���[�W�X�e�[�g�ɓ��������̃��\�b�h
void BagDamageState::Enter()
{
	{
		owner->GetModel()->PlayAnimation(EnemyBag::Damage, false);
	}
}


// �_���[�W�X�e�[�g�Ŏ��s���郁�\�b�h
void BagDamageState::Execute(float elapsedTime)
{

	// �U����������Ƃ�
	owner->FacePlayer(elapsedTime,2.0f);
		// �U�����[�V�������I����Ă���Αҋ@�֑J��
	if (!owner->GetModel()->IsPlayAnimation())
	{
		
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
		if (owner->GetTutorialflag()) {
			owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Search));
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Search::Idle));
		}
	}

}

// �_���[�W�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagDamageState::Exit()
{

}

//���S�X�e�[�g�ɓ��������̃��\�b�h
void BagDieState::Enter()
{
	// 
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::Die1), false);
}


// ���S�X�e�[�g�Ŏ��s���郁�\�b�h
void BagDieState::Execute(float elapsedTime)
{
	if (!owner->GetModel()->IsPlayAnimation()) {
		//owner->Destroy();
	}
}

// ���S�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagDieState::Exit()
{

}
// �퓬�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void BagStandbyState::Enter()
{
	if (!owner->GetAttackFlg())
	{
		// �U���������^AI���狁�߂�
		// �G�l�~�[���烁�^AI�� MsgAskAttackRight �𑗐M����
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
	owner->SetStateTimer(Mathf::RandomRange(2.0f, 3.0f));
	//if (rand() % 2 == 0) {
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	//	rightflag = true;
	//}
	//else
	//{
	//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	//	rightflag = false;
	//}
	//owner->PlaySe((int)EnemyBag::EnemyBagSE::Walk, true);
	owner->SetMoveRate(0.5);
	//owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBagAnimation::IdleBattle), false);
}
// �퓬�ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void BagStandbyState::Execute(float elapsedTime)
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
	owner->BattleMove(rightflag, elapsedTime, owner->GetMoveRate());
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//DirectX::XMFLOAT2 dir = owner->ForwardToPlayer();
	DirectX::XMFLOAT2 dir = owner->ForwardToBase();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = Base::Instance().GetPos();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist > owner->GetAttackRange()&& !owner->GetAttackFlg())
	{
		// �U���͈͂���o����ǐՃX�e�[�g�֑J��
		//owner->SetBattleRange(rand() % 6 + 1);
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));

	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		if (Vector3::Probability(2) && dist < owner->GetBackStepRange()) {
			//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::BackStep));
		}
	}
		
}
// �퓬�ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagStandbyState::Exit()
{
	owner->StopSe((int)EnemyBag::EnemyBagSE::Walk);
	owner->SetMoveRate(1.0);
}
// �퓬�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void BagBackStepState::Enter()
{
	
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), false);
	
}
// �퓬�ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void BagBackStepState::Execute(float elapsedTime)
{
	owner->RootMove();
	//���[�V�������I�������
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
	
}
// �퓬�ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void BagBackStepState::Exit()
{
	owner->SetMoveRate(1.0);
}
