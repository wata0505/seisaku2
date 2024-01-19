#include "StatePlayer.h"
#include "InputGame.h"
#include "AudioAll.h"
#include "ParticleSystem.h"
#include "ProjectileStraite.h"
#include "EffectTexAll.h"
#include "ParticleSprite.h"
#include "Afterimage.h"
#define GRAVITY -1.0;

//待機ステートに入った時のメソッド
void IdleState::Enter()
{
    owner->SetState(Player::State::Idle);
	owner->landflag = true;
	owner->slashCombo = -1;//斬撃初期化
	owner->combo = 0;
	owner->rollflag = false;
	owner->shildFlag = true;//縦描画
	//owner->GetWepon()->SetSwordState(SwordState::Nomal);//剣の位置初期化
	owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimIdle), true);//待機アニメーション
	owner->GetModel()->ClearSubAnimation();//サブアニメーション初期化
}
// 待機ステートで実行するメソッド
void IdleState::Execute(float elapsedTime)
{
	// タイマー処理
    if (owner->InputMove(elapsedTime)) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Move));
    }
    if (owner->InputJump())
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::JumpStandby)); // ジャンプ準備ステートへ遷移
    }
    //if (InputGame::InputRoll()) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Roll)); // 回避ステートへ遷移
    //}
    owner->Turn(elapsedTime, owner->dir.x, owner->dir.z, owner->turnSpeed);
    //if (owner->GetWepon()->GetBreakFlag(owner->weponType))return;
    if (owner->InputAttack())
    {
        // 攻撃ステートへ遷移
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Attack));
        //TransitionAttackState(WeponCombo[weponType][combo], WeponComboStatus[weponType][combo], WeponComboPow[weponType][combo]);
    }
    owner->SpecialUpdate();
    //if (InputGame::InputShift() &&owner->weponType != WeponType::Shield) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Shift));// シフトステートへ遷移
    //}
}
// 待機ステートから出ていくときのメソッド
void IdleState::Exit(){}

//移動ステートに入った時のメソッド
void MoveState::Enter()
{
    owner->SetState(Player::State::Move);
    // 走りアニメーション再生武器ごと
   
    owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimWalking), true);
    
  
    owner->slashCombo = -1;//斬撃初期化
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Run)->Play(true, SE);
    owner->GetModel()->ClearSubAnimation();
    
}
// 移動ステートで実行するメソッド
void MoveState::Execute(float elapsedTime)
{
    DirectX::XMFLOAT3 vec = owner->GetVelocity();
    float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
    if (length >= owner->GetMaxMoveSpeed() && owner->GetModel()->GetCurrentAnimationIndex() == static_cast<int>(Player::AnimWalking)) {
        owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimRunning), true);
    }

    // 移動入力処理
    if (!owner->InputMove(elapsedTime))
    {
        // 入力なしで待機ステートへ
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    // ジャンプ入力処理
    if (owner->InputJump())
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::JumpStandby));// ジャンプ準備ステートへ遷移
    }
    //if (InputGame::InputRoll()) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Roll));// 回避ステートへ遷移
    //}
    if (owner->InputAttack())
    {
        // 攻撃ステートへ遷移
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Attack));
    }
    owner->SpecialUpdate();
    //if (InputGame::InputShift() && owner->weponType != WeponType::Shield) {
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Shift));// シフトステートへ遷移
    //}
}
// 移動ステートから出ていくときのメソッド
void MoveState::Exit(){}

//ジャンプ準備ステートに入った時のメソッド
void JumpStandbyState::Enter()
{
    owner->SetState(Player::State::JumpStandby);
    // ジャンプアニメーション再生
    owner->GetModel()->PlayAnimation(Player::AnimJump1, false, 0.0);
}
// ジャンプ準備ステートで実行するメソッド
void JumpStandbyState::Execute(float elapsedTime)
{
    // ジャンプ入力処理
    if (!owner->GetModel()->IsPlayAnimation())
    {   // ジャンプステートへ遷移
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Jump));
    }
}
// ジャンプ準備ステートから出ていくときのメソッド
void JumpStandbyState::Exit() {}

//ジャンプステートに入った時のメソッド
void JumpState::Enter()
{
    owner->SetState(Player::State::Jump);
    owner->JumpInput(owner->jumpSpeed);
    owner->landflag = true;
    // ジャンプアニメーション再生
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
// ジャンプステートで実行するメソッド
void JumpState::Execute(float elapsedTime)
{
    // 移動入力処理
    owner->InputMove(elapsedTime);
    // ジャンプ入力処理
    if (owner->InputJump())
    {
        // ジャンプステートへ遷移
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::JumpStandby));
    }
    if (owner->InputAttack())
    {
        // 攻撃ステートへ遷移   
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::FallAttack));
    }
}
// ジャンプステートから出ていくときのメソッド
void JumpState::Exit() {}

//着地ステートに入った時のメソッド
void LandState::Enter()
{
    owner->SetState(Player::State::Land);
    owner->combo = 0;
    float g = GRAVITY;
    owner->SetGravity(g);
    if (owner->GetVelocity().y < -50.5 && owner->GetModel()->GetCurrentAnimationIndex() == Player::AnimJumpAttack) {//一定以上の落下速度でハルバートなら
        owner->GetModel()->PlayAnimation(Player::AnimLound, false);
        ParticleSystem::Instance().RubbleEffect({ owner->GetPosition().x,owner->GetPosition().y -5,owner->GetPosition().z }, 20, 100);
        ParticleSystem::Instance().Rubble3Effect({ owner->GetPosition().x,owner->GetPosition().y - 5,owner->GetPosition().z }, 20, 100);
        ParticleSprite* particleSprite = new ParticleSprite(owner->GetPosition(), {0,0,0}, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Sumi), 2000, 1.5, 0, true, 0.005, 0.06, { 1,1,1,1 });
        owner->combo = 3;
    }
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Lund)->Stop();
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Lund)->Play(false, SE);

}
// 着地ステートで実行するメソッド
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
// 着地ステートから出ていくときのメソッド
void LandState::Exit() {}

//回避ステートに入った時のメソッド
void RollState::Enter()
{
    owner->SetState(Player::State::Roll);
    owner->quickFlag = false;
    
    // 着地アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimRoll, false);
    owner->landflag = false;
    DirectX::XMFLOAT3 moveVec = owner->GetMoveVec();
    DirectX::XMFLOAT2 dir;
    //入力されていなかったらプレイヤーの前
    if (moveVec.x == 0 && moveVec.z == 0) {
        dir.x = sinf(owner->GetAngle().y);
        dir.y = cosf(owner->GetAngle().y);
    }
    else//入力方向
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
// 回避ステートで実行するメソッド
void RollState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation())
    {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    if (owner->GetInvincibleTimer() < 0)owner->rollflag = false;
    owner->Turn(elapsedTime, owner->GetVelocity().x, owner->GetVelocity().z, owner->turnSpeed);
}
// 回避ステートから出ていくときのメソッド
void RollState::Exit() {}

//スティンガーステートに入った時のメソッド
void StingerState::Enter()
{
    owner->SetState(Player::State::Stinger);
    // 着地アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimStrikeAttack, false);
    //owner->SetAttackStatus(owner->WSpStatus[owner->weponType], owner->WSpPow[owner->weponType]);
    owner->attackDir.x = owner->dir.x * owner->attackMove;
    owner->attackDir.z = owner->dir.z * owner->attackMove;
    //エフェクト用剣
    for (int i = 0; i < 6; i++) {
        ProjectileStraite* projectile = new ProjectileStraite(&owner->objectManager);
        projectile->Launch(owner->GetSword(),owner->GetHeight(), 1, 1.0 * i, Type::Stinger, (int)EffectTexAll::EfTexAll::Distortion, 0.7, NULL, NULL);
    }
    owner->stingerEffect = EffectAll::Instance().stingerEffect->Play({ 0, -20, 0 }, 0.5);
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Strike)->Play(false, SE);
   
}
// スティンガーステートで実行するメソッド
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
        if (!owner->enemyhitflag) {//敵に当たら停止
            velocity.x = owner->attackDir.x * owner->GetModel()->GetRootPow();
            velocity.z = owner->attackDir.z * owner->GetModel()->GetRootPow();
            owner->SetVelocity(velocity);
        }
        else
        {
            velocity.x = velocity.z = 0;
            owner->SetVelocity(velocity);
        }
        // 左手ノードとエネミーの衝突処理
        owner->CollisionNodeVsEnemies(owner->weponRadius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
        //エフェクト更新
        EffectAll::Instance().stingerEffect->SetPosition(owner->stingerEffect, owner->GetTailPos());
        EffectAll::Instance().stingerEffect->SetRotation(owner->stingerEffect, { 0,owner->GetAngle().y,0 });
    }
    if (animationTime > owner->attackEnd) {
        velocity.x = velocity.z = 0;
        owner->SetVelocity(velocity);
        EffectAll::Instance().stingerEffect->Stop(owner->stingerEffect);
    }
}
// スティンガーステートから出ていくときのメソッド
void StingerState::Exit() {}

// シフトステートに入った時のメソッド
void ShiftState::Enter()
{
    owner->SetState(Player::State::Shift);
    // 武器ごとのシフトアニメーション再生
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
// シフトステートで実行するメソッド
void ShiftState::Execute(float elapsedTime)
{
    owner->SetVelocityY(0);
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    if (animationTime > owner->attackEnd) {//武器を投げる
       
        owner->attackDir.x = owner->dir.x;
        owner->attackDir.z = owner->dir.z;
       
        if (owner->renderflag) {//エフェクト発生
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
    //if (owner->attackTime < 0) {//武器が当たらなかった
    //    owner->teleportPos = owner->GetWepon()->GetPosition();
    //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Teleport));
    //}
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
}
// シフトステートから出ていくときのメソッド
void ShiftState::Exit() {}


// シフト攻撃ステートに入った時のメソッド
void ShiftAttackState::Enter()
{
    owner->quickFlag = false;
    owner->SetState(Player::State::ShiftAttack);
    int weponCount = 0;
    //武器チェンジ
    while (weponCount < WeponType::Shield)
    {
        owner->weponType++;
        if (owner->weponType > 3) owner->weponType = 0;
        //if (!owner->GetWepon()->GetBreakFlag(owner->weponType)) {//壊れるか盾ならスキップ
        //    break;
        //}
        weponCount++;
    }
    //owner->GetWepon()->SetWeponType(owner->weponType);
    // シフト攻撃アニメーション再生
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
// シフト攻撃ステートで実行するメソッド
void ShiftAttackState::Execute(float elapsedTime)
{
    XMFLOAT3 velocity{};
    velocity.x = owner->attackDir.x * owner->GetModel()->GetRootPow();
    velocity.z = owner->attackDir.z * owner->GetModel()->GetRootPow();
    if (!owner->GetModel()->IsPlayAnimation())
    {
        owner->shiftAttackflag = false;//シフトアタックフラグ初期化
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    owner->SwordEffect();
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)
    {
        // 左手ノードとエネミーの衝突処理
        owner->CollisionNodeVsEnemies(owner->weponRadius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
    if (owner->enemyhitflag) {//敵に当たら停止
        velocity.x = 0;
        velocity.z = 0;
    }
    owner->SetVelocity(velocity);
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
}
// シフト攻撃ステートから出ていくときのメソッド
void ShiftAttackState::Exit() {}

// ハルバート特殊ステートに入った時のメソッド
void LanceState::Enter()
{
    owner->SetState(Player::State::Lance);
    // アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimStamp, false);
    //DirectX::XMFLOAT2 dir;
    //owner->SetAttackStatus(owner->WSpStatus[owner->weponType], owner->WSpPow[owner->weponType]);
    owner->attackTime = owner->lanceTimer;
    owner->stingerEffect = EffectAll::Instance().stampEffect->Play(owner->GetPosition(), 1.0);
    EffectAll::Instance().stampEffect->SetSpeed(owner->stingerEffect, owner->effectSpeed);
    ParticleSystem::Instance().LanceEffect(owner->GetPosition());
}
// ハルバート特殊ステートで実行するメソッド
void LanceState::Execute(float elapsedTime)
{
    owner->attackTime -= elapsedTime;
    owner->SwordEffect();

    owner->CollisionNodeVsEnemies(owner->weponRadius + owner->lanceRudius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    if (owner->attackTime < 0) {
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
}
// ハルバート特殊ステートから出ていくときのメソッド
void LanceState::Exit() {}

// チャージステートに入った時のメソッド
void ChargeState::Enter()
{
    owner->SetState(Player::State::Charge);
    // アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimCharge, false);
    owner->chargeCount = NULL;
}
// チャージステートで実行するメソッド
void ChargeState::Execute(float elapsedTime)
{
    //if (!owner->GetModel()->IsPlayAnimation() && owner->chargeCount < owner->chargeMax && owner->GetWepon()->GetWeponLife(owner->weponType) > 1) {//武器の耐久値を犠牲にしてチャージする
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
// チャージステートから出ていくときのメソッド
void ChargeState::Exit() {}

// チャージ攻撃ステートに入った時のメソッド
void ChargeAttackState::Enter()
{
    owner->SetState(Player::State::ChargeAttack);
    // アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimChargeAttack, false);
    //owner->SetAttackStatus(owner->WSpStatus[owner->weponType], owner->WSpPow[owner->weponType]);
    owner->attackTime = owner->chargeAttackTimer;
    owner->effectflag = false;
    owner->attackDir = owner->dir;
    owner->swingSe = true;
}
// チャージ攻撃ステートで実行するメソッド
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
    if (owner->attackCollisionFlag)//チャージ量によってダメージ、攻撃範囲を変える
    {
        owner->CollisionNodeVsEnemies(owner->weponRadius + (owner->chargeAttackRudius * owner->chargeCount), owner->knockBackpow, owner->attackDamage + (owner->chargeCount * owner->chargeDamage), owner->attackInvincibleTime);
    }

    if (owner->attackTime < 0 && !owner->effectflag) {//エフェクト
        //ParticleSprite* particleSprite = new ParticleSprite(owner->GetPosition(), owner->GetWepon()->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 3000, 3, NULL, true);
        //ParticleSystem::Instance().BoomEffect(owner->GetWepon()->GetWeaponPoint(), 1, int(EffectTexAll::EfTexAll::Thunder), 7 * owner->chargeCount, 1,{ 1,NULL,NULL,1 });
        //ParticleSystem::Instance().RubbleEffect(owner->GetWepon()->GetWeaponPoint());
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Rock)->Play(false, SE);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom2)->Play(false, SE);
        owner->effectflag = true;
    }
}
// チャージ攻撃ステートから出ていくときのメソッド
void ChargeAttackState::Exit() {}

// 攻撃ステートに入った時のメソッド
void AttackState::Enter()
{
    owner->SetState(Player::State::Attack);
    // アニメーション再生
    owner->GetModel()->PlayAnimation(owner->WeponCombo[owner->combo], false);
    owner->SetAttackStatus(owner->WeponComboStatus[0][owner->combo], owner->WeponComboPow[0][owner->combo]);
    owner->landflag = true;//着地ステートに入らない
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
// 攻撃ステートで実行するメソッド
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
        // 待機ステートへ
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        owner->combo = 0;
        return;
    }
    
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed * 10);
    // 任意のアニメーション再生区間でのみ衝突判定処理をする
    if (animationTime >= owner->attackStart && animationTime <= owner->attackEnd)owner->SwingInput();//スウィングSE
    if (owner->InputAttack() && owner->combo < owner->WeponComboMax[0]) {
        owner->comboflag = true;//コンボ先行入力
    }
    if (animationTime >= owner->attackEnd && owner->combo == 3  && owner->attackStart >= 0.6) {
        owner->correctionSpeed += 10;
        owner->cameraRange += animationTime * 20;
        owner->SlashInput();//飛ぶ斬撃
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
    
    if (owner->comboflag && animationTime >= owner->attackEnd) {//攻撃判定が終わったら次のコンボへ
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
        
        // 武器とエネミーの衝突処理
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
// 攻撃ステートから出ていくときのメソッド
void AttackState::Exit() {
    owner->cameraRange = owner->cameraRangeMax;
    owner->correctionSpeed = owner->correctionSpeedMax;
}

// 落下攻撃ステートに入った時のメソッド
void FallAttackState::Enter()
{
    owner->SetState(Player::State::FallAttack);
    // アニメーション再生
   // 落下攻撃アニメーション
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
// 落下攻撃ステートで実行するメソッド
void FallAttackState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation())
    {
       
        // 待機ステートへ
        //owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        owner->SetGravity(-1.0f);
    }
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
    // 任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
    owner->animeTimer *= 0.6;
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)
    {
        // 武器とエネミーの衝突処理
        owner->CollisionNodeVsEnemies(owner->weponRadius * 2, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
    if (owner->IsGround() && !owner->effectflag) {
        //ParticleSprite* particleSprite = new ParticleSprite(owner->GetWepon()->GetPosition(), owner->GetWepon()->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Flame), 100, 0.2, 0, true, 0.2);
        owner->SetShakeInput({ 0,1,0 }, 5);
        //owner->GetWepon()->SetSwordState(SwordState::Stop);

        owner->effectflag = true;
    }
    owner->SwordEffect();
    if (animationTime >= owner->attackStart) {//当たり判定が始まったら移動値入力
        owner->SetVelocityY(owner->GetVelocity().y + owner->attackMove);
        owner->SetGravity(-1.0f);

    }
}
// 落下攻撃ステートから出ていくときのメソッド
void FallAttackState::Exit() {}

// ガードステートに入った時のメソッド
void GuardState::Enter()
{
    owner->SetState(Player::State::Guard);
    // アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimGuard, false, 0.0);
    owner->attackTime = 1.5;
    owner->pGuardFlag = true;
    owner->guardFlag = true;
    owner->guardDir.x = owner->dir.x;
    owner->guardDir.y = owner->dir.z;
    owner->counterFlag = false;
}
// ガードステートで実行するメソッド
void GuardState::Execute(float elapsedTime)
{
    owner->BattleMoveUpdate();
    if (!owner->InputBattleMove(elapsedTime))owner->GetModel()->ClearSubAnimation();
    owner->attackTime -= elapsedTime;
    if (owner->attackTime < 0) {
        owner->guardFlag = false;
    }
    if (!InputGame::InputSpecial() && !owner->GetModel()->IsPlayAnimation())//押されている間ガード
    {
        owner->counterFlag = false;
        owner->guardFlag = false;
        // 待機ステートへ
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    if (owner->counterFlag) {
        //combo = 3;
        ParticleSystem::Instance().BarrierEffect(owner->GetPosition(), 10, (int)EffectTexAll::EfTexAll::Barrier, { 0,0,1,0.5 });
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::CounterStandby));
        owner->guardFlag = false;
    }
    if (!owner->GetModel()->IsPlayAnimation()) {//ロックオン対象の方向に向く
        owner->guardDir.x = owner->dir.x;
        owner->guardDir.y = owner->dir.z;
    }
   

    owner->Turn(elapsedTime, owner->guardDir.x, owner->guardDir.y, owner->turnSpeed);
}
// ガードステートから出ていくときのメソッド
void GuardState::Exit() {}

// カウンター準備ステートに入った時のメソッド
void CounterStandbyState::Enter()
{
    owner->SetState(Player::State::CounterStandby);
    // アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimGuardbreak, false, 0.0);
    //owner->SetAttackStatus(owner->CounterStatus, owner->CounterPow);
    owner->shildFlag = false;
    Enemy* enemy = owner->CloseEnemy(1000);
    owner->attackMoveID = enemy->GetId();
}
// カウンター準備ステートで実行するメソッド
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
    if (owner->enemyhitflag) {//敵に当たら停止
        velocity.x = 0;
        velocity.z = 0;
    }
    owner->SetVelocity(velocity);
}
// カウンター準備ステートから出ていくときのメソッド
void CounterStandbyState::Exit() {}

// カウンターステートに入った時のメソッド
void CounterState::Enter()
{
    owner->SetState(Player::State::Counter);
    // アニメーション再生
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
// カウンターステートで実行するメソッド
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
        // 待機ステートへ
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
    }
    owner->SwordEffect();
    owner->Turn(elapsedTime, owner->attackDir.x, owner->attackDir.z, owner->turnSpeed);
    // 任意のアニメーション再生区間でのみ衝突判定処理をする
    owner->attackCollisionFlag = animationTime >= owner->attackStart && animationTime <= owner->attackEnd;
    if (owner->attackCollisionFlag)
    {
        // 左手ノードとエネミーの衝突処理
        owner->CollisionNodeVsEnemies(owner->weponRadius, owner->knockBackpow, owner->attackDamage, owner->attackInvincibleTime);
    }
}
// カウンターステートから出ていくときのメソッド
void CounterState::Exit() {}

// テレポートステートに入った時のメソッド
void TeleportState::Enter()
{
    owner->SetState(Player::State::Teleport);
    // アニメーション再生
   // owner->GetWepon()->SetSwordState(SwordState::Stop);
    owner->landflag = false;
    owner->renderflag = false;
    ParticleSprite* particleSprite = new ParticleSprite(owner->teleportPos, { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Convergence, int(EffectTexAll::EfTexAll::Distortion), 2000, 3, 5, true, 0.05,0.05, { 0.5,0.5,0.5,1.0 });
    owner->quickFlag = true;
}
// テレポートステートで実行するメソッド
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
        //if (!owner->shiftAttackflag) {//シフト攻撃が当たっているか
        //    owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Roll));
        //    return;
        //}
        //owner->GetWepon()->SetSwordState(SwordState::Nomal);
        //owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::ShiftAttack));
    }
    if (owner->shiftAttackflag)owner->ChangeWepon();

}
// テレポートステートから出ていくときのメソッド
void TeleportState::Exit() {}

// ダメージステートに入った時のメソッド
void DamageState::Enter()
{
    owner->SetState(Player::State::Damage);
    // アニメーション再生
    owner->GetModel()->PlayAnimation(static_cast<int>(Player::AnimHit), false,0.0f);
    owner->landflag = false;
    owner->SetGravity(-1.0f);
}
// ダメージステートで実行するメソッド
void DamageState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation())
    {
        // 待機ステートへ
        if (!owner->downflag) {//ダウン状態
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        }
        else {
            owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Down));
        }
    }
    owner->Turn(elapsedTime, owner->dir.x, owner->dir.z, owner->turnSpeed);
}
// ダメージステートから出ていくときのメソッド
void DamageState::Exit() {}

// ダウンステートに入った時のメソッド
void DownState::Enter()
{
    owner->SetState(Player::State::Down);
    // アニメーション再生
    //owner->GetModel()->PlayAnimation(Player::AnimDown, true);
    owner->landflag = false;
}
// ダウンステートで実行するメソッド
void DownState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation()){
        // 待機ステートへ
        owner->GetStateMachine()->ChangeSubState(static_cast<int>(Player::State::Idle));
        owner->downflag = false;
    }
    owner->DownUpdate();
    owner->shildFlag = false;
}
// ダウンステートから出ていくときのメソッド
void DownState::Exit() {}

// 死亡ステートに入った時のメソッド
void DieState::Enter()
{
    owner->SetState(Player::State::Dead);
    owner->GetModel()->PlayAnimation(Player::AnimDie, false, 0.0);
    // アニメーション再生
    owner->landflag = false;
}
// 死亡ステートで実行するメソッド
void DieState::Execute(float elapsedTime)
{
    if (!owner->GetModel()->IsPlayAnimation() || owner->GetFallDeath()){
        owner->disslveTimer -= elapsedTime;//ディゾルブによる演出
    }
}
// 死亡ステートから出ていくときのメソッド
void DieState::Exit() {}

PlayerState::~PlayerState()
{
    for (State* state : subStatePool)
    {
        delete state;
    }
    subStatePool.clear();
}
// サーチステートに入った時のメソッド
void PlayerState::Enter()
{
    SetSubState(static_cast<int>(Player::State::Idle));
}
// サーチステートで実行するメソッド
void PlayerState::Execute(float elapsedTime)
{
    subState->Execute(elapsedTime);
}
// サーチステートから出ていくときのメソッド
void PlayerState::Exit()
{
}