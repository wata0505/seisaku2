#include "StatePlayer.h"
#include "InputGame.h"
#include "AudioAll.h"
#include "ParticleSystem.h"
#include "ProjectileStraite.h"
#include "EffectTexAll.h"
#include "ParticleSprite.h"
#include "Afterimage.h"
#define GRAVITY -1.0;

//�ҋ@�X�e�[�g�ɓ��������̃��\�b�h
void IdleState::Enter()
{
    owner->SetState(Player::State::Idle);
	owner->landflag = true;
	owner->slashCombo = -1;//�a��������
	owner->combo = 0;
	owner->rollflag = false;
	owner->shildFlag = true;//�c�`��
	//owner->GetWepon()->SetSwordState(SwordState::Nomal);//���̈ʒu������
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimIdle), true);//�ҋ@�A�j���[�V����
	owner->GetModel()->ClearSubAnimation();//�T�u�A�j���[�V����������
}
// �ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
void IdleState::Execute(float elapsedTime)
{
	// �^�C�}�[����
    if (owner->InputMove(elapsedTime)) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Move));
    }
    if (owner->InputJump())
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::JumpStandby)); // �W�����v�����X�e�[�g�֑J��
    }
    //if (InputGame::InputRoll()) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Roll)); // ����X�e�[�g�֑J��
    //}
    owner->Turn(elapsedTime, owner->dir.x, owner->dir.z, owner->turnSpeed);
    //if (owner->GetWepon()->GetBreakFlag(owner->weponType))return;
    if (owner->InputAttack())
    {
        // �U���X�e�[�g�֑J��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Attack));
        //TransitionAttackState(WeponCombo[weponType][combo], WeponComboStatus[weponType][combo], WeponComboPow[weponType][combo]);
    }
    owner->SpecialUpdate();
    //if (InputGame::InputShift() &&owner->weponType != WeponType::Shield) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Shift));// �V�t�g�X�e�[�g�֑J��
    //}
}
// �ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void IdleState::Exit(){}

//�ړ��X�e�[�g�ɓ��������̃��\�b�h
void MoveState::Enter()
{
    owner->SetState(Player::State::Move);
    // ����A�j���[�V�����Đ����킲��
   
    owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimWalking), true);
    
  
    owner->slashCombo = -1;//�a��������
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Run)->Play(true, SE);
    owner->GetModel()->ClearSubAnimation();
    
}
// �ړ��X�e�[�g�Ŏ��s���郁�\�b�h
void MoveState::Execute(float elapsedTime)
{
    DirectX::XMFLOAT3 vec = owner->GetVelocity();
    float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
    if (length >= owner->GetMaxMoveSpeed() && owner->GetModel()->GetCurrentAnimationIndex() == static_cast<int>(Player::AnimWalking)) {
        owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimRunning), true);
    }

    // �ړ����͏���
    if (!owner->InputMove(elapsedTime))
    {
        // ���͂Ȃ��őҋ@�X�e�[�g��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    // �W�����v���͏���
    if (owner->InputJump())
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::JumpStandby));// �W�����v�����X�e�[�g�֑J��
    }
    //if (InputGame::InputRoll()) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Roll));// ����X�e�[�g�֑J��
    //}
    if (owner->InputAttack())
    {
        // �U���X�e�[�g�֑J��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Attack));
    }
    owner->SpecialUpdate();
    //if (InputGame::InputShift() && owner->weponType != WeponType::Shield) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Shift));// �V�t�g�X�e�[�g�֑J��
    //}
}
// �ړ��X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void MoveState::Exit(){}

//�W�����v�����X�e�[�g�ɓ��������̃��\�b�h
void JumpStandbyState::Enter()
{
    owner->SetState(Player::State::JumpStandby);
    // �W�����v�A�j���[�V�����Đ�
    owner->GetModel()->PlayAnimation(Player::AnimJump1, false, 0.0);
}
// �W�����v�����X�e�[�g�Ŏ��s���郁�\�b�h
void JumpStandbyState::Execute(float elapsedTime)
{
    // �W�����v���͏���
    if (!owner->GetModel()->IsPlayAnimation())
    {   // �W�����v�X�e�[�g�֑J��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Jump));
    }
}
// �W�����v�����X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void JumpStandbyState::Exit() {}

//�W�����v�X�e�[�g�ɓ��������̃��\�b�h
void JumpState::Enter()
{
    owner->SetState(Player::State::Jump);
    owner->JumpInput(owner->jumpSpeed);
    owner->landflag = true;
    // �W�����v�A�j���[�V�����Đ�
    if (owner->jumpCount < owner->jumpLimit) {
        owner->GetModel()->PlayAnimation(Player::AnimJump2, false);
    }
    else
    {
        owner->GetModel()->PlayAnimation(Player::AnimJump3, false);
        //EffectAll::Instance().jumpEffect->Play({ position.x,(position.y),position.z }, 0.5);
    }
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Jump)->Stop();
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Jump)->Play(false, SE);
}
// �W�����v�X�e�[�g�Ŏ��s���郁�\�b�h
void JumpState::Execute(float elapsedTime)
{
    // �ړ����͏���
    owner->InputMove(elapsedTime);
    // �W�����v���͏���
    if (owner->InputJump())
    {
        // �W�����v�X�e�[�g�֑J��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::JumpStandby));
    }
    if (owner->InputAttack())
    {
        // �U���X�e�[�g�֑J��   
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::FallAttack));
    }
}
// �W�����v�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void JumpState::Exit() {}

//���n�X�e�[�g�ɓ��������̃��\�b�h
void LandState::Enter()
{
    owner->SetState(Player::State::Land);
    owner->combo = 0;
    float g = GRAVITY;
    owner->SetGravity(g);
    if (owner->GetVelocity().y < -50.5 && owner->GetModel()->GetCurrentAnimationIndex() == Player::AnimJumpAttack) {//���ȏ�̗������x�Ńn���o�[�g�Ȃ�
        owner->GetModel()->PlayAnimation(Player::AnimLound, false);
        ParticleSystem::Instance().RubbleEffect({ owner->GetPosition().x,owner->GetPosition().y -5,owner->GetPosition().z }, 20, 100);
        ParticleSystem::Instance().Rubble3Effect({ owner->GetPosition().x,owner->GetPosition().y - 5,owner->GetPosition().z }, 20, 100);
        ParticleSprite* particleSprite = new ParticleSprite(owner->GetPosition(), {0,0,0}, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Sumi), 2000, 1.5, 0, true, 0.005, 0.06, { 1,1,1,1 });
        owner->combo = 3;
    }
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Lund)->Stop();
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Lund)->Play(false, SE);

}
// ���n�X�e�[�g�Ŏ��s���郁�\�b�h
void LandState::Execute(float elapsedTime)
{
    if (owner->combo == 3) {
        if (!owner->GetModel()->IsPlayAnimation()) {
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Stop();
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        }
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Play(false, SE);
        owner->CollisionNodeVsEnemies(15, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
    else
    {
      owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
}
// ���n�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void LandState::Exit() {}

//����X�e�[�g�ɓ��������̃��\�b�h
void RollState::Enter()
{
    owner->SetState(Player::State::Roll);
    owner->quickFlag = false;
    
    // ���n�A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimRoll, false);
    owner->landflag = false;
    DirectX::XMFLOAT3 moveVec = owner->GetMoveVec();
    DirectX::XMFLOAT2 dir;
    //���͂���Ă��Ȃ�������v���C���[�̑O
    if (moveVec.x == 0 && moveVec.z == 0) {
        dir.x = sinf(owner->GetAngle().y);
        dir.y = cosf(owner->GetAngle().y);
    }
    else//���͕���
    {
        dir.x = moveVec.x;
        dir.y = moveVec.z;
        dir = Vector2::Normalize(dir);
    }
    DirectX::XMFLOAT3 velocity = {0,0,0};
    velocity.x = dir.x * owner->rollPow;
    velocity.z = dir.y * owner->rollPow;
    owner->SetVelocity(velocity);
    owner->SetInvincibleTime(owner->rollInvincibleTimer);
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Roll)->Stop();
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Roll)->Play(false, SE);
    owner->rollflag = true;
}
// ����X�e�[�g�Ŏ��s���郁�\�b�h
void RollState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation())
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    if (owner->GetInvincibleTimer() < 0)owner->rollflag = false;
    owner->Turn(elapsedTime, owner->GetVelocity().x, owner->GetVelocity().z, owner->turnSpeed);
}
// ����X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void RollState::Exit() {}

//�X�e�B���K�[�X�e�[�g�ɓ��������̃��\�b�h
void StingerState::Enter()
{
    owner->SetState(Player::State::Stinger);
    // ���n�A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimStrikeAttack, false);
    //owner->SetAttackStatus(owner->WSpStatus[owner->weponType], owner->WSpPow[owner->weponType]);
    owner->attackDir.x = owner->dir.x * owner->attackMove;
    owner->attackDir.z = owner->dir.z * owner->attackMove;
    //�G�t�F�N�g�p��
    for (int i = 0; i < 6; i++) {
        ProjectileStraite* projectile = new ProjectileStraite(&owner->objectManager);
        projectile->Launch(owner->GetSword(),owner->GetHeight(), 1, 1.0 * i, Type::Stinger, (int)EffectTexAll::EfTexAll::Distortion, 0.7, NULL, NULL);
    }
    owner->stingerEffect = EffectAll::Instance().stingerEffect->Play({ 0, -20, 0 }, 0.5);
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Strike)->Play(false, SE);
   
}
// �X�e�B���K�[�X�e�[�g�Ŏ��s���郁�\�b�h
void StingerState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation())
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        EffectAll::Instance().stingerEffect->Stop(owner->stingerEffect);
    }
    owner->SwordEffect();
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
    XMFLOAT3 velocity = { 0,0,0 };
    if (owner->attackCollisionFlag)
    {
        //float angle = DirectX::XMConvertToRadians(rand() % 360);
        //XMFLOAT3 trailDir{};
        //trailDir.x = sinf(angle);
        //trailDir.y = cosf(angle);
        //trailDir.z = 0;
        //float length = rand() % 10;
        //XMFLOAT3 position = owner->GetPosition();
        //position.x += trailDir.x * length;
        //position.y = owner->GetEfPos().y;
        //position.z += trailDir.y * length;
        //ParticleSprite* particleSprite = new ParticleSprite(position, trailDir, ParticleSprite::ParticleLine, ParticleSprite::Slash, int(EffectTexAll::EfTexAll::BlueThader), 1, 0.5);
        if (!owner->enemyhitflag) {//�G�ɓ������~
            velocity.x = owner->attackDir.x * owner->GetModel()->GetRootPow();
            velocity.z = owner->attackDir.z * owner->GetModel()->GetRootPow();
            owner->SetVelocity(velocity);
        }
        else
        {
            velocity.x = velocity.z = 0;
            owner->SetVelocity(velocity);
        }
        // ����m�[�h�ƃG�l�~�[�̏Փˏ���
        owner->CollisionNodeVsEnemies(owner->weponRadius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
        //�G�t�F�N�g�X�V
        EffectAll::Instance().stingerEffect->SetPosition(owner->stingerEffect, owner->GetTailPos());
        EffectAll::Instance().stingerEffect->SetRotation(owner->stingerEffect, { 0,owner->GetAngle().y,0 });
    }
    if (animationTime > owner->attackEnd) {
        velocity.x = velocity.z = 0;
        owner->SetVelocity(velocity);
        EffectAll::Instance().stingerEffect->Stop(owner->stingerEffect);
    }
}
// �X�e�B���K�[�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void StingerState::Exit() {}

// �V�t�g�X�e�[�g�ɓ��������̃��\�b�h
void ShiftState::Enter()
{
    owner->SetState(Player::State::Shift);
    // ���킲�Ƃ̃V�t�g�A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(owner->WeponShift[owner->weponType], false);
    //if (!owner->IsGround()) {
    //    owner->GetModel()->PlayAnimation(Player::AnimShift_jump, false);
    //}
    //owner->SetAttackStatus(owner->WSStatus[owner->weponType], owner->ShifPow);
    //owner->attackDir.x = owner->dir.x;
    //owner->attackDir.z = owner->dir.z;
    //owner->attackTime = owner->shiftTime;
    //owner->shiftAttackflag = false;

}
// �V�t�g�X�e�[�g�Ŏ��s���郁�\�b�h
void ShiftState::Execute(float elapsedTime)
{
    owner->SetVelocityY(0);
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    if (animationTime > owner->attackEnd) {//����𓊂���
       
        owner->attackDir.x = owner->dir.x;
        owner->attackDir.z = owner->dir.z;
       
        if (owner->renderflag) {//�G�t�F�N�g����
            ParticleSprite* particleSprite = new ParticleSprite(owner->GetPosition(), owner->GetEfPos(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Metal), 2000, 3, 3, true);
            Afterimage* projectile = new Afterimage(&owner->objectManager);
            projectile->Launch(owner->renderdata);

        }
        owner->renderflag = false;
    }
    if (owner->renderflag == false)owner->attackTime -= elapsedTime;
    //if (owner->GetWepon()->GetSwordState() == SwordState::Shift) {
    //    owner->CollisionNodeVsEnemies(owner->weponRadius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    //    owner->attackCollisionFlag = true;
    //    owner->SwordEffect();
    //}
    //if (owner->attackTime < 0) {//���킪������Ȃ�����
    //    owner->teleportPos = owner->GetWepon()->GetPosition();
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Teleport));
    //}
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
}
// �V�t�g�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void ShiftState::Exit() {}


// �V�t�g�U���X�e�[�g�ɓ��������̃��\�b�h
void ShiftAttackState::Enter()
{
    owner->quickFlag = false;
    owner->SetState(Player::State::ShiftAttack);
    int weponCount = 0;
    //����`�F���W
    while (weponCount < WeponType::Shield)
    {
        owner->weponType++;
        if (owner->weponType > 3) owner->weponType = 0;
        //if (!owner->GetWepon()->GetBreakFlag(owner->weponType)) {//���邩���Ȃ�X�L�b�v
        //    break;
        //}
        weponCount++;
    }
    //owner->GetWepon()->SetWeponType(owner->weponType);
    // �V�t�g�U���A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(owner->WeponShiftAttack[owner->weponType], false);
    //owner->SetAttackStatus(owner->WSAStatus[owner->weponType], owner->WSAPow[owner->weponType]);
    Enemy* enemy = owner->CloseEnemy(100);
    XMFLOAT3 enemydir = owner->dir;
    if (enemy) {
        owner->DirToLength(owner->GetPosition(), { enemy->GetPosition().x,owner->GetPosition().y,enemy->GetPosition().z },enemydir, owner->enemyLength);
    }
    owner->attackDir = enemydir;
    owner->landflag = false;
    owner->shildFlag = false;

}
// �V�t�g�U���X�e�[�g�Ŏ��s���郁�\�b�h
void ShiftAttackState::Execute(float elapsedTime)
{
    XMFLOAT3 velocity{};
    velocity.x = owner->attackDir.x * owner->GetModel()->GetRootPow();
    velocity.z = owner->attackDir.z * owner->GetModel()->GetRootPow();
    if (!owner->GetModel()->IsPlayAnimation())
    {
        owner->shiftAttackflag = false;//�V�t�g�A�^�b�N�t���O������
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    owner->SwordEffect();
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)
    {
        // ����m�[�h�ƃG�l�~�[�̏Փˏ���
        owner->CollisionNodeVsEnemies(owner->weponRadius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
    if (owner->enemyhitflag) {//�G�ɓ������~
        velocity.x = 0;
        velocity.z = 0;
    }
    owner->SetVelocity(velocity);
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
}
// �V�t�g�U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void ShiftAttackState::Exit() {}

// �n���o�[�g����X�e�[�g�ɓ��������̃��\�b�h
void LanceState::Enter()
{
    owner->SetState(Player::State::Lance);
    // �A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimStamp, false);
    //DirectX::XMFLOAT2 dir;
    //owner->SetAttackStatus(owner->WSpStatus[owner->weponType], owner->WSpPow[owner->weponType]);
    owner->attackTime = owner->lanceTimer;
    owner->stingerEffect = EffectAll::Instance().stampEffect->Play(owner->GetPosition(), 1.0);
    EffectAll::Instance().stampEffect->SetSpeed(owner->stingerEffect, owner->effectSpeed);
    ParticleSystem::Instance().LanceEffect(owner->GetPosition());
}
// �n���o�[�g����X�e�[�g�Ŏ��s���郁�\�b�h
void LanceState::Execute(float elapsedTime)
{
    owner->attackTime -= elapsedTime;
    owner->SwordEffect();

    owner->CollisionNodeVsEnemies(owner->weponRadius + owner->lanceRudius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    if (owner->attackTime < 0) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
}
// �n���o�[�g����X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void LanceState::Exit() {}

// �`���[�W�X�e�[�g�ɓ��������̃��\�b�h
void ChargeState::Enter()
{
    owner->SetState(Player::State::Charge);
    // �A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimCharge, false);
    owner->chargeCount = NULL;
}
// �`���[�W�X�e�[�g�Ŏ��s���郁�\�b�h
void ChargeState::Execute(float elapsedTime)
{
    //if (!owner->GetModel()->IsPlayAnimation() && owner->chargeCount < owner->chargeMax && owner->GetWepon()->GetWeponLife(owner->weponType) > 1) {//����̑ϋv�l���]���ɂ��ă`���[�W����
    //    ParticleSprite* particleSprite = new ParticleSprite(owner->GetWepon()->GetWeaponPoint(), owner->GetWepon()->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Convergence, int(EffectTexAll::EfTexAll::Flame), 2000, 3, 3, true);
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Chage)->Stop();
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Chage)->Play(false, SE);
    //    owner->chargeCount++;
    //   
    //}
    if (!InputGame::InputSpecial() && !owner->chargeCount) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    else if (!InputGame::InputSpecial()) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::ChargeAttack));
    }
    owner->Turn(elapsedTime, owner->dir.x, owner->dir.z, owner->turnSpeed);
}
// �`���[�W�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void ChargeState::Exit() {}

// �`���[�W�U���X�e�[�g�ɓ��������̃��\�b�h
void ChargeAttackState::Enter()
{
    owner->SetState(Player::State::ChargeAttack);
    // �A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimChargeAttack, false);
    //owner->SetAttackStatus(owner->WSpStatus[owner->weponType], owner->WSpPow[owner->weponType]);
    owner->attackTime = owner->chargeAttackTimer;
    owner->effectflag = false;
    owner->attackDir = owner->dir;
    owner->swingSe = true;
}
// �`���[�W�U���X�e�[�g�Ŏ��s���郁�\�b�h
void ChargeAttackState::Execute(float elapsedTime)
{
    XMFLOAT3 velocity{};
    velocity.x = owner->attackDir.x * owner->GetModel()->GetRootPow();
    velocity.z = owner->attackDir.z * owner->GetModel()->GetRootPow();
    owner->SetVelocity(velocity);
    if (!owner->GetModel()->IsPlayAnimation()) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    owner->attackTime -= elapsedTime;
    owner->SwordEffect();
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    if (animationTime >= owner->attackStart)owner->SwingInput();
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)//�`���[�W�ʂɂ���ă_���[�W�A�U���͈͂�ς���
    {
        owner->CollisionNodeVsEnemies(owner->weponRadius + (owner->chargeAttackRudius * owner->chargeCount), owner->knockBackpow, owner->attackDamage + (owner->chargeCount * owner->chargeDamage), owner->attackInvincibleTime);
    }

    if (owner->attackTime < 0 && !owner->effectflag) {//�G�t�F�N�g
        //ParticleSprite* particleSprite = new ParticleSprite(owner->GetPosition(), owner->GetWepon()->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 3000, 3, NULL, true);
        //ParticleSystem::Instance().BoomEffect(owner->GetWepon()->GetWeaponPoint(), 1, int(EffectTexAll::EfTexAll::Thunder), 7 * owner->chargeCount, 1,{ 1,NULL,NULL,1 });
        //ParticleSystem::Instance().RubbleEffect(owner->GetWepon()->GetWeaponPoint());
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Rock)->Play(false, SE);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom2)->Play(false, SE);
        owner->effectflag = true;
    }
}
// �`���[�W�U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void ChargeAttackState::Exit() {}

// �U���X�e�[�g�ɓ��������̃��\�b�h
void AttackState::Enter()
{
    owner->SetState(Player::State::Attack);
    // �A�j���[�V�����Đ�
    owner->GetModel()->PlayAnimation(owner->WeponCombo[owner->combo], false);
    owner->SetAttackStatus(owner->WeponComboStatus[0][owner->combo], owner->WeponComboPow[0][owner->combo]);
    owner->landflag = true;//���n�X�e�[�g�ɓ���Ȃ�
    owner->attackDir = owner->dir;
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Swing1)->Stop();
    owner->swingSe = true;
    owner->swingTimer = 0;
    DirectX::XMFLOAT3 enemydir = {};
    float dist = FLT_MAX;
    if (!owner->lockOn || owner->counterFlag) {
        Enemy* enemy = owner->CloseEnemy(100);
        XMFLOAT3 enemydir = owner->dir;
        if (enemy) {
            owner->DirToLength(owner->GetPosition(), { enemy->GetPosition().x,owner->GetPosition().y,enemy->GetPosition().z }, enemydir, owner->enemyLength);
        }
        owner->attackDir = enemydir;

    }
    owner->counterFlag = false;
    owner->effectflag = false;
    owner->shildFlag = false;
}
// �U���X�e�[�g�Ŏ��s���郁�\�b�h
void AttackState::Execute(float elapsedTime)
{
    owner->cameraRange = owner->cameraRangeMax;
    XMFLOAT3 velocity = owner->GetVelocity();
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    if (animationTime <= owner->attackStart) {
        owner->animeTimer *= owner->attackMove;
        velocity.x = owner->attackDir.x * (owner->GetModel()->GetRootPow() * owner->attackMove);
        velocity.z = owner->attackDir.z * (owner->GetModel()->GetRootPow() * owner->attackMove);
    }
    else {
        velocity.x = owner->attackDir.x * (owner->GetModel()->GetRootPow() * owner->attackMove);
        velocity.z = owner->attackDir.z * (owner->GetModel()->GetRootPow() * owner->attackMove);
    }
    owner->SetVelocity(velocity);
    if (!owner->GetModel()->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        owner->combo = 0;
        return;
    }
    
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed * 10);
    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    if (animationTime >= owner->attackStart && animationTime <= owner->attackEnd)owner->SwingInput();//�X�E�B���OSE
    if (owner->InputAttack() && owner->combo < owner->WeponComboMax[0]) {
        owner->comboflag = true;//�R���{��s����
    }
    if (animationTime >= owner->attackEnd && owner->combo == 3  && owner->attackStart >= 0.6) {
        owner->correctionSpeed += 10;
        owner->cameraRange += animationTime * 20;
        owner->SlashInput();//��Ԏa��
    }
    if (animationTime >= 0.1 &&owner->combo == 3 && owner->InputAttack()) {
        owner->attackStart = 0.6;
        //if (owner->attackStart > 0.6)owner->attackStart = 0.6;
    }
    if (animationTime >= 0.2 && animationTime <= owner->attackStart*0.8 && owner->combo == 3 ) {
        ParticleSystem::Instance().SeirlConvergenceEffect(owner->SearchNodePos(owner->attackNode[owner->combo]), 90);
        ParticleSystem::Instance().SeirlConvergenceEffect(owner->SearchNodePos(owner->attackNode[owner->combo]), 180);
        ParticleSystem::Instance().SeirlConvergenceEffect(owner->SearchNodePos(owner->attackNode[owner->combo]), 270);
        
    }
    if (animationTime >= 0.2 && animationTime <= owner->attackStart&& owner->combo == 3) {
        owner->cameraRange -= animationTime * 10;
    }
    
    if (owner->comboflag && animationTime >= owner->attackEnd) {//�U�����肪�I������玟�̃R���{��
        owner->combo++;
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Attack));
        if (owner->combo == owner->WeponComboMax[owner->weponType]) owner->landflag = false;
        owner->comboflag = false;
        return;
    }
    else if (owner->InputStick(elapsedTime) && animationTime >= owner->attackEnd) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Move));
        owner->combo = 0;
        return;
    }
    if (animationTime <= owner->attackEnd)owner->SwordEffect();
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)
    {
        
        // ����ƃG�l�~�[�̏Փˏ���
        owner->CollisionNodeVsEnemies(2.0f, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
    if (animationTime <= owner->attackEnd + 0.2 && animationTime >= owner->attackEnd  && !owner->effectflag) {
        if (owner->attackDamage > 1) {
            //ParticleSprite* particleSprite = new ParticleSprite(owner->GetWepon()->GetPosition(), owner->GetWepon()->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Flame), 100, 0.2, 0, true, 0.2);
            owner->SetShakeInput({ 0,1,0 }, owner->attackDamage);
        }
        owner->effectflag = true;
    }
   
}
// �U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void AttackState::Exit() {
    owner->cameraRange = owner->cameraRangeMax;
    owner->correctionSpeed = owner->correctionSpeedMax;
}

// �����U���X�e�[�g�ɓ��������̃��\�b�h
void FallAttackState::Enter()
{
    owner->SetState(Player::State::FallAttack);
    // �A�j���[�V�����Đ�
   // �����U���A�j���[�V����
    //if (owner->weponType != WeponType::Halberd) {
    //    owner->GetModel()->PlayAnimation(Player::AnimJumpAttack, false, 0.0);
    //    owner->SetAttackStatus(owner->FallAttackStatus[owner->FallMain], owner->FallAttackPow[owner->FallMain]);
    //    owner->landflag = false;
    //}
    //else {
        owner->GetModel()->PlayAnimation(Player::AnimJumpAttack, false, 0.0);
        owner->SetAttackStatus(owner->FallAttackStatus, owner->FallAttackPow);
        owner->landflag = true;
    //
    //}
    Enemy* enemy = owner->CloseEnemy(10);
    XMFLOAT3 enemydir = owner->dir;
    if (enemy) {
        owner->DirToLength(owner->GetPosition(), { enemy->GetPosition().x,owner->GetPosition().y,enemy->GetPosition().z }, enemydir, owner->enemyLength);
    }
    owner->attackDir = enemydir;
    owner->SetGravity(0);
    owner->effectflag = false;
    owner->shildFlag = false;
    owner->combo = 3;
}
// �����U���X�e�[�g�Ŏ��s���郁�\�b�h
void FallAttackState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation())
    {
       
        // �ҋ@�X�e�[�g��
        //owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        owner->SetGravity(-1.0f);
    }
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    owner->animeTimer *= 0.6;
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)
    {
        // ����ƃG�l�~�[�̏Փˏ���
        owner->CollisionNodeVsEnemies(owner->weponRadius * 2, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
    if (owner->IsGround() && !owner->effectflag) {
        //ParticleSprite* particleSprite = new ParticleSprite(owner->GetWepon()->GetPosition(), owner->GetWepon()->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Flame), 100, 0.2, 0, true, 0.2);
        owner->SetShakeInput({ 0,1,0 }, 5);
        //owner->GetWepon()->SetSwordState(SwordState::Stop);

        owner->effectflag = true;
    }
    owner->SwordEffect();
    if (animationTime >= owner->attackStart) {//�����蔻�肪�n�܂�����ړ��l����
        owner->SetVelocityY(owner->GetVelocity().y + owner->attackMove);
        owner->SetGravity(-1.0f);

    }
}
// �����U���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void FallAttackState::Exit() {}

// �K�[�h�X�e�[�g�ɓ��������̃��\�b�h
void GuardState::Enter()
{
    owner->SetState(Player::State::Guard);
    // �A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimGuard, false, 0.0);
    owner->attackTime = 1.5;
    owner->pGuardFlag = true;
    owner->guardFlag = true;
    owner->guardDir.x = owner->dir.x;
    owner->guardDir.y = owner->dir.z;
    owner->counterFlag = false;
}
// �K�[�h�X�e�[�g�Ŏ��s���郁�\�b�h
void GuardState::Execute(float elapsedTime)
{
    owner->BattleMoveUpdate();
    if (!owner->InputBattleMove(elapsedTime))owner->GetModel()->ClearSubAnimation();
    owner->attackTime -= elapsedTime;
    if (owner->attackTime < 0) {
        owner->guardFlag = false;
    }
    if (!InputGame::InputSpecial() && !owner->GetModel()->IsPlayAnimation())//������Ă���ԃK�[�h
    {
        owner->counterFlag = false;
        owner->guardFlag = false;
        // �ҋ@�X�e�[�g��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    if (owner->counterFlag) {
        //combo = 3;
        ParticleSystem::Instance().BarrierEffect(owner->GetPosition(), 10, (int)EffectTexAll::EfTexAll::Barrier, { 0,0,1,0.5 });
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::CounterStandby));
        owner->guardFlag = false;
    }
    if (!owner->GetModel()->IsPlayAnimation()) {//���b�N�I���Ώۂ̕����Ɍ���
        owner->guardDir.x = owner->dir.x;
        owner->guardDir.y = owner->dir.z;
    }
   

    owner->Turn(elapsedTime, owner->guardDir.x, owner->guardDir.y, owner->turnSpeed);
}
// �K�[�h�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void GuardState::Exit() {}

// �J�E���^�[�����X�e�[�g�ɓ��������̃��\�b�h
void CounterStandbyState::Enter()
{
    owner->SetState(Player::State::CounterStandby);
    // �A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimGuardbreak, false, 0.0);
    //owner->SetAttackStatus(owner->CounterStatus, owner->CounterPow);
    owner->shildFlag = false;
    Enemy* enemy = owner->CloseEnemy(1000);
    owner->attackMoveID = enemy->GetId();
}
// �J�E���^�[�����X�e�[�g�Ŏ��s���郁�\�b�h
void CounterStandbyState::Execute(float elapsedTime)
{
    XMFLOAT3 velocity{};
    velocity.x = owner->guardDir.x * owner->GetModel()->GetRootPow();
    velocity.z = owner->guardDir.y * owner->GetModel()->GetRootPow();
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    if (animationTime >= owner->attackStart)
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Counter));
    }
    owner->Turn(elapsedTime, owner->guardDir.x, owner->guardDir.y, owner->turnSpeed);
    if (owner->enemyhitflag) {//�G�ɓ������~
        velocity.x = 0;
        velocity.z = 0;
    }
    owner->SetVelocity(velocity);
}
// �J�E���^�[�����X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void CounterStandbyState::Exit() {}

// �J�E���^�[�X�e�[�g�ɓ��������̃��\�b�h
void CounterState::Enter()
{
    owner->SetState(Player::State::Counter);
    // �A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimCounter, false, 0.4);
    //owner->SetAttackStatus(owner->CounterStatus, owner->CounterPow);
    owner->attackTime = 0.1;
    EnemyManager& enemyManager = EnemyManager::Instance();
    DirectX::XMFLOAT3 enemydir = {};
    Enemy* enemy = enemyManager.GetEnemyFromId(owner->attackMoveID);
    owner->DirToLength(owner->GetPosition(), { enemy->GetPosition().x,owner->GetPosition().y,enemy->GetPosition().z }, enemydir, owner->enemyLength);
    float attackMoveRate = owner->enemyLength / 50;
    if (attackMoveRate > 1)attackMoveRate = 1;
    owner->attackMove *= attackMoveRate;
    owner->attackDir = enemydir;
}
// �J�E���^�[�X�e�[�g�Ŏ��s���郁�\�b�h
void CounterState::Execute(float elapsedTime)
{
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    XMFLOAT3 velocity = owner->GetVelocity();
    if (animationTime <= owner->attackStart) {
        velocity.x = owner->attackDir.x * (owner->GetModel()->GetRootPow() * owner->attackMove);
        velocity.z = owner->attackDir.z * (owner->GetModel()->GetRootPow() * owner->attackMove);
    }
    else {
        velocity.x = owner->attackDir.x * (owner->GetModel()->GetRootPow());
        velocity.z = owner->attackDir.z * (owner->GetModel()->GetRootPow());
    }
    owner->SetVelocity(velocity);
    if (!owner->GetModel()->IsPlayAnimation())
    {
        owner->counterFlag = false;
        // �ҋ@�X�e�[�g��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    owner->SwordEffect();
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)
    {
        // ����m�[�h�ƃG�l�~�[�̏Փˏ���
        owner->CollisionNodeVsEnemies(owner->weponRadius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
}
// �J�E���^�[�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void CounterState::Exit() {}

// �e���|�[�g�X�e�[�g�ɓ��������̃��\�b�h
void TeleportState::Enter()
{
    owner->SetState(Player::State::Teleport);
    // �A�j���[�V�����Đ�
   // owner->GetWepon()->SetSwordState(SwordState::Stop);
    owner->landflag = false;
    owner->renderflag = false;
    ParticleSprite* particleSprite = new ParticleSprite(owner->teleportPos, { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Convergence, int(EffectTexAll::EfTexAll::Distortion), 2000, 3, 5, true, 0.05,0.05, { 0.5,0.5,0.5,1.0 });
    owner->quickFlag = true;
}
// �e���|�[�g�X�e�[�g�Ŏ��s���郁�\�b�h
void TeleportState::Execute(float elapsedTime)
{
    XMFLOAT3 velocity = owner->GetVelocity();
    owner->telportDir = Vector3::Subset(owner->teleportPos, owner->GetPosition());
    float Length = Vector3::Lenght(owner->telportDir);
    owner->telportDir = Vector3::Normalize(owner->telportDir);
    if (Length > 1) {
        velocity = Vector3::Multip(owner->telportDir, owner->telportSpeed);
        owner->SetVelocity(velocity);
    }
    else
    {
        owner->renderflag = true;
        velocity.x = velocity.z = velocity.y = 0;
        owner->SetVelocity(velocity);
        //if (!owner->shiftAttackflag) {//�V�t�g�U�����������Ă��邩
        //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Roll));
        //    return;
        //}
        //owner->GetWepon()->SetSwordState(SwordState::Nomal);
        //owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::ShiftAttack));
    }
    if (owner->shiftAttackflag)owner->ChangeWepon();

}
// �e���|�[�g�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void TeleportState::Exit() {}

// �_���[�W�X�e�[�g�ɓ��������̃��\�b�h
void DamageState::Enter()
{
    owner->SetState(Player::State::Damage);
    // �A�j���[�V�����Đ�
    owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimHit), false,0.0f);
    owner->landflag = false;
    owner->SetGravity(-1.0f);
}
// �_���[�W�X�e�[�g�Ŏ��s���郁�\�b�h
void DamageState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g��
        if (!owner->downflag) {//�_�E�����
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        }
        else {
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Down));
        }
    }
    owner->Turn(elapsedTime, owner->dir.x, owner->dir.z, owner->turnSpeed);
}
// �_���[�W�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DamageState::Exit() {}

// �_�E���X�e�[�g�ɓ��������̃��\�b�h
void DownState::Enter()
{
    owner->SetState(Player::State::Down);
    // �A�j���[�V�����Đ�
    //owner->GetModel()->PlayAnimation(Player::AnimDown, true);
    owner->landflag = false;
}
// �_�E���X�e�[�g�Ŏ��s���郁�\�b�h
void DownState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation()){
        // �ҋ@�X�e�[�g��
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        owner->downflag = false;
    }
    owner->DownUpdate();
    owner->shildFlag = false;
}
// �_�E���X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DownState::Exit() {}

// ���S�X�e�[�g�ɓ��������̃��\�b�h
void DieState::Enter()
{
    owner->SetState(Player::State::Dead);
    owner->GetModel()->PlayAnimation(Player::AnimDie, false, 0.0);
    // �A�j���[�V�����Đ�
    owner->landflag = false;
}
// ���S�X�e�[�g�Ŏ��s���郁�\�b�h
void DieState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation() || owner->GetFallDeath()){
        owner->disslveTimer -= elapsedTime;//�f�B�]���u�ɂ�鉉�o
    }
}
// ���S�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void DieState::Exit() {}

PlayerState::~PlayerState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}
// �T�[�`�X�e�[�g�ɓ��������̃��\�b�h
void PlayerState::Enter()
{
    SetSubState(static_cast<int>(Player::State::Idle));
}
// �T�[�`�X�e�[�g�Ŏ��s���郁�\�b�h
void PlayerState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}
// �T�[�`�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void PlayerState::Exit()
{
}