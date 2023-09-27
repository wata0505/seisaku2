#include "Player.h"
#include "Input.h"
#include "Camera.h"
#include "Collision.h"
#include "Graphics.h"
#include "EnemyManager.h"
#include "Afterimage.h"
#include "ProjectileStraite.h"
//#include "EffectAll.h"
#include "EnemyDragon.h"
#include "ParticleSystem.h"
#include "Calculation.h"
#include "ParticleSpriteManager.h"
#include "AudioAll.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
#include "UIManager.h"
#include "GaugeUI.h"
#include "InputGame.h"
#include "StatePlayer.h"
#define GRAVITY -1.0;
using namespace DirectX;
inline XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
{
    XMFLOAT4 xmfloat4;
    xmfloat4.x = static_cast<float>(fbxdouble4[0]);
    xmfloat4.y = static_cast<float>(fbxdouble4[1]);
    xmfloat4.z = static_cast<float>(fbxdouble4[2]);
    xmfloat4.w = static_cast<float>(fbxdouble4[3]);
    return xmfloat4;
}

static Player* instance = nullptr;

// �C���X�^���X�擾
Player& Player::Instance()
{
    return *instance;
}
Player::Player() {
    // �C���X�^���X�|�C���g�ݒ�
    instance = this;

    player = std::make_unique<Model>(".\\resources\\Knight Errant\\Knight_Errant4.fbx", true);
    player->AppendAnimations(".\\resources\\Knight Errant\\Idle.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_front.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_frontright.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_frontleft.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_back.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_backright.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_backleft.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_right_1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\walk_strafe_left_1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\run_strafe_front2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\AXErun_strafe_front.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\SSrun2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\run_strafe_front_5.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\Combo_03_1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewCombo_03_2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewCombo_03_31.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewCombo_03_4.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\Combo_02_1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewCombo_02_2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewCombo_02_3.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewCombo_02_4.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\2Combo1_1_copy.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\2Combo1_2_copy.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\2Combo1_3_copy.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\Combo_01_4.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewAxeCombo1-1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewAxeCombo2-2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewAxeCombo3-3.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewEXCombo1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewEXCombo2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewEXCombo4_1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewHBCombo1-1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewHBCombo2_1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewHBCombo3-4.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\2Gud_break5.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\2Gud_1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\roll_front.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\new3jump.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\newjump.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\new2jump.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewjumpAttack.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\ransuJumpAttack.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\ransuJumpAttackLoud.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\Newstrike.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\hit_head_back.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\hit_head_front.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\hit_head_left.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\hit_head_right.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\2Gud_hit2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewCounter.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\Newcharge.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\charging.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewchargeAttack1.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewShift_jump.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\SWSF2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\AxeShift.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\HBSF2.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\EXShift.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\NewSftAttack.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\ransuAttack.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\weponChange_copy.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\weponChange_Silb.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\liedown_idle_up.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\liedown_hit.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\rise_01.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\rise_021.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\rise_right_up.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\rise_left_up.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\dead_01.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\dead_02.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\dead_04.fbx", 0);
    player->AppendAnimations(".\\resources\\Knight Errant\\dead_05.fbx", 0);
    player->ModelSerialize(".\\resources\\Knight Errant\\Knight_Errant4.fbx");
   // player->ModelCreate(".\\resources\\AimTest\\GreatSword.fbx");
    player->ModelRegister(".\\resources\\Knight Errant\\Knight_Errant4.fbx");
	player->PlayAnimation(2, true);

    sword = std::make_unique<Model>(".\\resources\\Sword.fbx", true);
    sword->ModelSerialize(".\\resources\\Sword.fbx");
    sword->ModelRegister(".\\resources\\Sword.fbx");
    sword->UpdateBufferDara(transform);
    slash = std::make_unique<Model>(".\\resources\\Textures\\zanngeki4.fbx", true);
    slash->ModelSerialize(".\\resources\\Textures\\zanngeki4.fbx");
    slash->ModelRegister(".\\resources\\Textures\\zanngeki4.fbx");
    slash->UpdateBufferDara(transform);
    wepon = std::make_unique<MainWepon>();
    maxHealth = 20;
    health = maxHealth;
    mp = mpMax;
    UIManager& uiManager = UIManager::Instance();
    const float gaugeWidth = 500.0f;
    const float gaugeHeight = 50.0f;
    uiHp = new GaugeUI(L".\\resources\\HP.png", 32, 608, gaugeWidth, gaugeHeight*0.7, health, maxHealth);
    uiMp = new GaugeUI(L".\\resources\\MP.png", 32, 657, gaugeWidth + 11.0f, gaugeHeight * 0.2, mp, mpMax);
    //UI[UINo::Die] = std::make_unique<Sprite>(L".\\resources\\UI\\Die.png");
    swordTrail = std::make_unique<SwordTrail>(10, L".\\resources\\SwordTrail.jpg");
    cameraController = std::make_unique<CameraController>();
    
    player->UpdateBufferDara(transform);
    renderdata = player->GetBufferData();
    for (int i = 0; i < 13; i++) {
        lowerBody.push_back(lower[i]);
    }
    stateMachine = std::make_unique<StateMachine<Player>>();
    // �X�e�[�g�}�V����1�w�ڂ̃X�e�[�g��o�^(BattleState�����l�̕��@�Ŋe���ǉ����Ă�������)
    stateMachine->RegisterState(new PlayerState(this));
    // �e�e�X�e�[�g�ɃT�u�X�e�[�g��o�^(WanderState�ȊO��2�w�ڂ̃X�e�[�g�����l�̕��@�Ŋe���ǉ����Ă��������B)
    stateMachine->RegisterSubState(0, new IdleState(this));
    stateMachine->RegisterSubState(0, new MoveState(this));
    stateMachine->RegisterSubState(0, new JumpStandbyState(this));
    stateMachine->RegisterSubState(0, new JumpState(this));
    stateMachine->RegisterSubState(0, new LandState(this));
    stateMachine->RegisterSubState(0, new RollState(this));
    stateMachine->RegisterSubState(0, new StingerState(this));
    stateMachine->RegisterSubState(0, new ShiftState(this));
    stateMachine->RegisterSubState(0, new ShiftAttackState(this));
    stateMachine->RegisterSubState(0, new LanceState(this));
    stateMachine->RegisterSubState(0, new ChargeState(this));
    stateMachine->RegisterSubState(0, new ChargeAttackState(this));
    stateMachine->RegisterSubState(0, new AttackState(this));
    stateMachine->RegisterSubState(0, new FallAttackState(this));
    stateMachine->RegisterSubState(0, new GuardState(this));
    stateMachine->RegisterSubState(0, new CounterStandbyState(this));
    stateMachine->RegisterSubState(0, new CounterState(this));
    stateMachine->RegisterSubState(0, new TeleportState(this));
    stateMachine->RegisterSubState(0, new DamageState(this));
    stateMachine->RegisterSubState(0, new DownState(this));
    stateMachine->RegisterSubState(0, new DieState(this));
    // �X�e�[�g���Z�b�g
    stateMachine->SetState(0);
}
Player::~Player()
{
}
void Player::update(float elapsedTime) {
    //DrawDebugPrimitive();
    SetWepon();
    float ElapsedTime = elapsedTime * modelSpeed;
    if (quickFlag) ElapsedTime *= 0.6;
    if (attackHitflag) ElapsedTime *= attackHitPow;
    animeTimer = ElapsedTime;
    //���b�N�I���J����
    ComeTerget(elapsedTime);
    //�X�e�[�g�}�V���X�V
    stateMachine->Update(elapsedTime);
    //�A�j���[�V�����X�V
    player->UpdateAnimation(animeTimer, "root_x");
    player->UpdateSubAnimation(animeTimer, "root_x");
    
    weponRenderUpdate();
    //���͏����X�V
    UpdateVelocity(ElapsedTime);
    UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
    //�`����X�V
    player->UpdateBufferDara(transform);
    //���������
    SetWepon();
    //����X�V
    wepon->Update(elapsedTime);
    // �X�L������
    UpdateSkill(elapsedTime);
    //�e���ˏ���
    InputProjectile();
    //���O�ՍX�V
    swordTrail->Update();
  
    AudioUpdate();
    //�q�b�g�t���O�X�V
    UpdateHitflag(elapsedTime);
    // ���G���ԍX�V
    UpdateInvincibleTimer(ElapsedTime);

    //UI�X�V����
    UpdateUI(elapsedTime);

	//�e�X�V
    objectManager.Update(ElapsedTime);
    //�`�悵�Ă��瓖���蔻��
    if (renderflag) {
        CollisionPlayerVsEnemies();
    }
    //�e�����蔻��
    CollisionProjectilesVsEnemies();
    //�`�������
    renderdata = player->GetBufferData();
    timer++;
    //ui�A�j���[�V�����p�X�V
    //swordEffectColor.w ++;
}
void Player::AudioUpdate() {
    //�ړ��ȊO�Ȃ�_�b�V��SE�X�g�b�v
    if (state != State::Move) AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Run)->Stop();
}
void Player::SearchNode(DirectX::XMFLOAT4X4& Transform, const char* nodeName) {
    Animation::Keyframe::node* leftHandBone = player->FindNode(nodeName); //�m�[�h�擾
    XMStoreFloat4x4(&Transform, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));//�m�[�h���[���h���W�ϊ�
}
DirectX::XMFLOAT3 Player::SearchNodePos(const char* nodeName) {
    DirectX::XMFLOAT4X4 nodeTransform = {};
    Animation::Keyframe::node* leftHandBone = player->FindNode(nodeName); //�m�[�h�擾
    XMStoreFloat4x4(&nodeTransform, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));//�m�[�h���[���h���W�ϊ�
    return { nodeTransform._41,nodeTransform._42,nodeTransform._43 };
}
void Player::SetWepon() {
    if (weponType == WeponType::MainSword) {
        SearchNode(weponTransform, "hand_l");//����m�[�h�擾
        shieldHead = { weponTransform._41,weponTransform._42,weponTransform._43 };//���̍��W����
    }
    SearchNode(weponTransform, "Wp_Sword");//���m�[�h�擾
    head = { weponTransform._41,weponTransform._42,weponTransform._43 };//�r�̍��W����
    sworddir = { weponTransform._31,weponTransform._32,weponTransform._33 };//���̌����擾
    sworddir = Vector3::Normalize(sworddir);//���̌������K��
    pattern = wepon->GetWeaponPPoint();//����̕��擾
    tail = wepon->GetWeaponPoint();//����̐�擾
}
void Player::UpdateHitflag(float elapsedTime) {
    if (hitInvincibleTime > 0.0f)
    {
        hitInvincibleTime -= elapsedTime;
    }
    else
    {
        attackHitflag= false;
    }

}
void Player::weponRenderUpdate() {
    if (wepon->GetDisslve() == 1) {
        wepon->SetSwordState(SwordState::Nomal);
        wepon->SetRnderflag(true);
    }

}
void Player::SetAttackStatus(DirectX::XMFLOAT4 Status, DirectX::XMFLOAT4 Pow){
    //�m�b�N�o�b�N�p���[����
    knockBackpow.x = Pow.x;
    knockBackpow.y = Pow.y;
    attackStart = Status.z;
    attackEnd = Status.w;
    //�㏸�p���[
    velocity.y = Status.y;
    //�U�����̓����p���[
    attackMove = Status.x;
    attackDamage = Pow.z;
    attackInvincibleTime = Pow.w;
}
//�^�[�Q�b�g�̋����ƕ����擾
void Player::DirToLength(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3& dir, float& length) {
    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
    DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
    length = DirectX::XMVectorGetX(D);
    DirectX::XMStoreFloat3(&dir, N);
}
void Player::ComeTerget(float elapsedTime) {
    //�G�l�~�[�ւ̕����x�N�g��  
    DirectX::XMFLOAT3 enemydir = {};
    //�^�[�Q�b�g�ʒu
    DirectX::XMFLOAT3 target = {};
    //��ԋ߂��̓G���^�[�Q�b�g�ɂ���
    EnemyManager& enemyManager = EnemyManager::Instance();
    float dist = FLT_MAX;
    float d = 0;
    //�G�l�~�[��
    int enemyCount = enemyManager.GetEnemyCount();
    if (fallDeath)return;
    if (enemyCount == 0 ) { // ���b�N�I������G�����Ȃ�
        DirectX::XMFLOAT3 camePos = position;
        camePos.y += height;
        cameraController->SetTarget(camePos);
        cameraController->Update(elapsedTime);
        return;
    }
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    int x = 0;
    if (ax > 0.5) x = 1;  //�E�X�e�b�N���͐ݒ�
    if (ax < -0.5) x = -1;//�E�X�e�b�N���͐ݒ�
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_THUMB) {//�E�X�e�b�N��������
        locklength = !locklength;//���b�N�I����ԕω�
        fistlock = true;//�n�߂ă��b�N�I��
    }
    Enemy* enemy = nullptr;
    switch (x)
    {
        //���b�N�I�����̓��Z�b�g
    case 0:
        lockflag = false;
        break;
        //���̃��b�N�I���^�[�Q�b�g��
    case 1:
        if (lockflag)break;//�E�X�e�b�N���͂��ꑱ�Â��Ă���
        if (!locklength)break;//���b�N�I����Ԃ���Ȃ��Ȃ�
        enemyLock++;
        if (enemyLock >= enemyCount) enemyLock = 0;//���b�N�I���Ώۂ��G�l�~�[���𒴂�����0��
        lockflag = true; //�E�X�e�b�N���͂��ꂽ��
        enemy = enemyManager.GetEnemy(enemyLock);//�G�l�~�[�擾
        while (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) {//�G�l�~�[��HP�����邩�A�A�N�e�B�u�ɂȂ��Ă��邩
            enemyLock++;
            if (enemyLock >= enemyCount) enemyLock = 0;//���b�N�I���Ώۂ��G�l�~�[���𒴂�����0��
            enemy = enemyManager.GetEnemy(enemyLock);
        }
        enemyId = enemy->GetId();//ID�擾
        break;
        //�O�̃��b�N�I���^�[�Q�b�g��
    case -1:
        if (lockflag)break;//�E�X�e�b�N���͂��ꑱ�Â��Ă���
        if (!locklength)break;//���b�N�I����Ԃ���Ȃ��Ȃ�
        enemyLock--;
        lockflag = true;//�E�X�e�b�N���͂��ꂽ��
        if (enemyLock < 0) enemyLock = enemyCount - 1;//���b�N�I���Ώۂ�0��艺�Ȃ�ő吔
        enemy = enemyManager.GetEnemy(enemyLock);
        while (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) {//�G�l�~�[��HP�����邩�A�A�N�e�B�u�ɂȂ��Ă��邩
            enemyLock--;
            if (enemyLock < 0) enemyLock = enemyCount - 1;//���b�N�I���Ώۂ�0��艺�Ȃ�ő吔
            enemy = enemyManager.GetEnemy(enemyLock);
        }
        enemyId = enemy->GetId();//ID�擾
    }
    //���b�N�I�����͂����ꂽ��
    if (locklength) {
        //�n�߂ă��b�N�I�����͂��ꂽ��ł��߂��G�����b�N�I��
        if(!fistlock)
        {
            enemy = enemyManager.GetEnemyFromId(enemyId);
            if(enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) {//�G�l�~�[��HP�����邩�A�A�N�e�B�u�ɂȂ��Ă��邩
                fistlock = true;  
            }
            DirToLength(position, { enemy->GetPosition().x,position.y,enemy->GetPosition().z }, enemydir, enemyLength);
            //�����Ŕ���
            if (enemyLength < 6000) {
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5;
                dir = enemydir;
                lockOn = true;
                epos = enemy->GetEfPos();
                dist = enemyLength;
            }
            else
            {
                fistlock = true;
            }
        }
        if(fistlock){//�n�߂Ẵ��b�N�I��
            dist = FLT_MAX;
            for (int i = 0; i < enemyCount; i++) {
                enemy = enemyManager.GetEnemy(i);
                if (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) continue;
                DirToLength(position, { enemy->GetPosition().x,position.y,enemy->GetPosition().z }, enemydir, enemyLength);
                float a = enemyLength;
                if (d == 0) d = a;//�������
                //�����Ŕ���
                if (a < 4000) {
                    if (a < dist)
                    {
                        dist = a;
                        target = enemy->GetPosition();
                        target.y += enemy->GetHeight() * 0.5f;
                        enemyId = enemy->GetId();
                        epos = enemy->GetEfPos();
                        dir = enemydir;
                        lockOn = true;
                    }
                }
            }
            fistlock = false;
            if (dist == FLT_MAX) {
                lockOn = false;
                locklength = false;
                fistlock = true;
            }
        }
    }
    else
    {
        lockOn = false;
        dir.x = sinf(angle.y);
        dir.z = cosf(angle.y);
    }
    //���b�N�I�����Ă�Ȃ�J�����ݒ�
    DirectX::XMFLOAT3 camePos = position;
    if(state == Attack){
      camePos.y = SearchNodePos("root_x").y;
      camePos.y += height / 2;
    }
    else
    {
        camePos.y = position.y;
        camePos.y += height;
    }
    cameraController->SetTarget(camePos);
    if (lockOn) {
        cameraController->SetTarget2(target);
        enemyLength = dist;
        dist *= 0.1;//�����␳
        cameraController->Update2(elapsedTime, { dir.x,NULL,dir.z }, dist);
    }
    else {
        cameraController->Update(elapsedTime);
    }
}


//���n�������ɌĂ΂��
void Player::OnLanding()
{
    jumpCount = 0;//�W�����v�J�E���g������
    if (velocity.y < NULL && landflag)
    {
        stateMachine->ChangeSubState(static_cast<int>(Player::State::Land));//���n
    }
}

void Player::OnDamaged()
{
    // �_���[�W�X�e�[�g�֑J��
    if (state != State::Guard) {
        stateMachine->ChangeSubState(static_cast<int>(Player::State::Damage));
    }
}
void Player::OnDead()
{
    // �_���[�W�X�e�[�g�֑J��
   
     stateMachine->ChangeSubState(static_cast<int>(Player::State::Dead));
    
}
void Player::PlayerKnockBack(DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY,float damage)
{
    EffectAll::Instance().stingerEffect->Stop(stingerEffect);//�G�t�F�N�g��~
    DirectX::XMFLOAT3 inpulse = Vector3::Subset(position, mypos);//�x�N�g������
    inpulse = Vector3::Multip(Vector3::Normalize(inpulse),power);//���K�����ăm�b�N�o�b�N�p���[��������
    SetShakeInput(inpulse, damage);//�J�����V�F�C�N
    if (state == State::Guard) {
        if (!guardFlag) {
            wepon->Exhaustion(damage);//�������
        }
        player->PlayAnimation(AnimGuardhit, false);
        if (wepon->GetWeponLife(weponType) < 0) {//����ꂽ��
            player->PlayAnimation(AnimGuardbreak, false);
            invincibleTimer = 2.0;
            guardDir.x = -inpulse.x;
            guardDir.y = -inpulse.z;
            guardDir = Vector2::Normalize(guardDir);
            return;
        }
        inpulse.x *= guardKnockBack;
        inpulse.z *= guardKnockBack;
        AddImpulse(inpulse);//�m�b�N�o�b�N
        //�����������������
        guardDir.x = -inpulse.x;
        guardDir.y = -inpulse.z;
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Gud)->Stop();
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Gud)->Play(false, SE);
        return;
    }
    else if (power > kbpow || health <= 0) {
        player->PlayAnimation(AnimDead1, false);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, SE);
        downflag = true;//�_�E�����
        //�R���{������
        combo = 0;
    }
    else if (downflag) {
        player->PlayAnimation(AnimDownHit, false);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, SE);
        downflag = true;//�_�E�����
    }
    else
    {
        player->PlayAnimation(AnimHit1, false);
        //�R���{������
        combo = 0;
    }
    inpulse.y = powerY;//������̃m�b�N�o�b�N
    AddImpulse(inpulse);//�m�b�N�o�b�N
    //�����������������
    dir.x = -inpulse.x;
    dir.z = -inpulse.z;
}

void Player::SpecialUpdate() {
    if (InputGame::InputSpecial()) {
        switch (weponType)//����ɂ���ē���U��
        {
        case WeponType::MainSword:
            stateMachine->ChangeSubState(static_cast<int>(Player::State::Guard));
            break;
        case WeponType::Axe:
            stateMachine->ChangeSubState(static_cast<int>(Player::State::Charge));
            break;
        case WeponType::Halberd:
            if (mp > 0) {
                mp--;
                stateMachine->ChangeSubState(static_cast<int>(Player::State::Lance));
            }
            break;
        case WeponType::SubSword:
            if (mp > 0) {
                mp--;
                stateMachine->ChangeSubState(static_cast<int>(Player::State::Stinger));
            }
        default:
            break;
        }
    }

}
void Player::BattleMoveUpdate() {
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //�O�����Ɠ��͕����̓��ςŃA�j���[�V�����ω�
    DirectX::XMFLOAT2 moveVec = { GetMoveVec().x,GetMoveVec().z };
    DirectX::XMFLOAT2 Vec;
    Vec.x = sinf(angle.y);
    Vec.y = cosf(angle.y);
    moveVec = Vector2::Normalize(moveVec);
    float dot = Vector2::Dot(moveVec, Vec);
    dot = acos(dot);

    // �����A�j���[�V�����Đ�
    if (ax > 0 && dot > upSens && dot < disUpSens) {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkFrontLeft) {
            player->PlaySubAnimation(AnimWorkFrontLeft, true,lowerBody);        
        }
    }
    else if (ax < 0  && dot >  upSens && dot < disUpSens) {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkFrontRight) {
            player->PlaySubAnimation(AnimWorkFrontRight, true,lowerBody);     
        }
    }
    else if (ax > 0 && dot > disUpSens && dot < disDownSens) {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkLeft) {
            player->PlaySubAnimation(AnimWorkLeft, true, lowerBody);
           
        }
    }
    else if (ax < 0 && dot >  disUpSens && dot < disDownSens) {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkRight) {
            player->PlaySubAnimation(AnimWorkRight, true,lowerBody);  
        }
    }
    else if (ax > 0 && dot > disDownSens && dot < downSens) {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkBackLeft) {
            player->PlaySubAnimation(AnimWorkBackLeft, true, lowerBody);
        }
    }
    else if (ax < 0 && dot >  disDownSens && dot < downSens) {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkBackRight) {
            player->PlaySubAnimation(AnimWorkBackRight, true, lowerBody);
        }
    }
    else if (ay > 0 ) {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkFront) {
            player->PlaySubAnimation(AnimWorkFront, true, lowerBody);
           
        }
    }
    else {
        if (player->GetCurrentSubAnimationIndex() != AnimWorkBack) {
            player->PlaySubAnimation(AnimWorkBack, true, lowerBody);
           
        }
    }
}
void Player::DownUpdate() {
    if (!downflag)return;
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //�O�����Ɠ��͕����̓��ςŃA�j���[�V�����ω�
    DirectX::XMFLOAT2 moveVec = { GetMoveVec().x,GetMoveVec().z };
    moveVec = Vector2::Normalize(moveVec);
    DirectX::XMFLOAT2 Vec = { Camera::Instance().GetFront().x,Camera::Instance().GetFront().z };
    float dot = Vector2::Dot(moveVec, Vec);
    dot = acos(dot);
    // ���A�A�j���[�V�����Đ�
    if (ax > 0 && dot > upSens && dot < disDownSens) {
            player->PlayAnimation(AnimDownUpLeft, false);
            velocity.x = moveVec.x * rollPow*returnPow;
            velocity.z = moveVec.y * rollPow*returnPow;
            downflag = false;
    }
    else if (ax < 0 && dot >  upSens && disDownSens) {
            player->PlayAnimation(AnimDownUpRight, false);
            velocity.x = moveVec.x * rollPow * returnPow;
            velocity.z = moveVec.y * rollPow * returnPow;
            downflag = false;
    }
    else if (ay > 0) {
            player->PlayAnimation(AnimDownCUp, false);
            downflag = false;
    }
    else if (ay < 0 ){
         player->PlayAnimation(AnimDownUp, false);
         downflag = false;
    }
}
DirectX::XMFLOAT3 Player::GetSlashDir() {
    DirectX::XMFLOAT2 trailPos = Vector3::C3Dor2D(swordTrail->GetTrail(swordFlame));//�ϐ��{�P�t���[���O�̌���
    DirectX::XMFLOAT2 weaponPoint = Vector3::C3Dor2D(wepon->GetWeaponEFPoint());//���݂̌���
    trailPos = Vector2::Subset(weaponPoint,trailPos);
    trailPos = Vector2::Normalize(trailPos);
    DirectX::XMFLOAT3 trailDir = { trailPos.x,trailPos.y,NULL };
    return trailDir;
}
void Player::SlashInput() {
    if (slashCombo != combo ) {
        wepon->Exhaustion(attackDamage);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::SlashFly)->Stop();
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::SlashFly)->Play(false, SE);
        //�񌳂Ŏa�������Ə�����œ��ς�����Ă��̕��p�x���Ƃ�
        DirectX::XMFLOAT3 hitdir = GetSlashDir();
        DirectX::XMFLOAT3 n = { 0, 1, 0 };
        float dot = Vector3::Dot(hitdir,n);
        dot = acosf(dot);
        if (hitdir.x < 0)dot *= -1;//���@�����Ȃ�p�x���]
        ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
        projectile->Launch(slash, height / 2, 4.0, angle.y, Type::Straight, (int)EffectTexAll::EfTexAll::Metal, 2, 3, 0.0);
        projectile->SetScale(slashScale);
        projectile->SetDirectionUp({-attackDir.z,dot,attackDir.x});
        if (combo == WeponComboMax[weponType]) {//�R���{�Ō�ǉ��a���{�P
            ProjectileStraite* projectile2 = new ProjectileStraite(&objectManager);
            projectile2->Launch(slash, height / 2, 4.0, angle.y, Type::Straight, (int)EffectTexAll::EfTexAll::Metal, 2, 3, 0.0);
            projectile2->SetScale(slashScale);
            projectile2->SetDirectionUp({ attackDir.z,dot,-attackDir.x });
        }
        slashCombo = combo;
    }
}
void Player::SwingInput() {
    //�X�C���OSE
    if (swingSe) {
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Swing1)->Play(false, SE);
        swingSe = false;
    }
    if (attackInvincibleTime < 0.2) {//�A���X�C���OSE
        if (Vector3::FrameInput(swingTimer)) {
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Swing1)->Stop();
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Swing1)->Play(false, SE);
        }
        swingTimer += swingSpeed;
    }
}

void Player::ChangeWepon() {
    if (weponType == weponChangeType && wepon->GetDisslve() == 1) {//���킪�`�悳��Œ�����킪�\�����ꂽ��
        return;
    }
    if (timer % weponChangeTimer == 0) {
        float hi = 0;
        if (weponChangeType == WeponType::Shield)hi = 1;// ���␳�l
        ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
        projectile->Launch(wepon->GetWeponModel(weponChangeType), hi, 1.0, angle.y, Type::ChangeWp, (int)EffectTexAll::EfTexAll::Thunder, 0.3, 0, 0.1);
        weponChangeType++;//�o������؂�ւ�
    }
    if (weponChangeType > 4) weponChangeType = 0;
}
void Player::ButtonWeponChange(int type) {
    if (!wepon->GetBreakFlag(type) && weponType != type) {
        wepon->SetRnderflag(false);
        skillCoolTime[WeponChange] = skillCoolTimeDefault[WeponChange];
        skillTime[WeponChange] = 0.3f;//�p������
        nextWepon = type;
        weponChangeType = weponType;
        weponChange = true;
        if (state == State::Idle) {
            player->PlayAnimation(AnimWeponChange, false, 0.2);
        }
        ParticleSprite* particleSprite = new ParticleSprite(wepon->GetPosition(), wepon->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Thunder), 5000, 2.0);
        particleSprite = new ParticleSprite(wepon->GetWeaponPPoint(), wepon->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::WeponConvergence, int(EffectTexAll::EfTexAll::Thunder), 5000, 2, 5);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Change)->Stop();
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Change)->Play(false, SE);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Break)->Stop();
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Break)->Play(false, SE);
    }
}
void Player::SetShakeInput(DirectX::XMFLOAT3 dir,float damage) {
    //�J�����V�F�C�N������
    cameraController->SetHitPow(damage);
    if(lockOn)cameraController->SetHitPow(damage*1.5);
    cameraController->SetHitTimer(damage*0.5);
    cameraController->SetHitDir(dir);
}
void Player::HitInput(float damage, float invincibleTime) {
    if (wepon->GetSwordState() == SwordState::Shift) {//����̏�Ԃ��V�t�g�Ȃ�
        teleportPos = wepon->GetPosition();//�e���|�[�g�|�C���g�ݒ�
        shiftAttackflag = true;
        stateMachine->ChangeSubState(static_cast<int>(Player::State::Teleport));//�e���|�[�g�X�e�[�g
        Afterimage* projectile = new Afterimage(&objectManager);//�c���G�t�F�N�g
        projectile->Launch(renderdata);
        if (wepon->GetBroken()&&mp < mpMax) mp++;//����̑ϋv�l���񊄈ȉ��Ȃ�MP��

    }
    attackHitflag = true;
    wepon->Exhaustion(damage);//����
    hitInvincibleTime = damage*0.05;
    //�U�������ɃJ�����V�F�C�N
    DirectX::XMFLOAT3 hitdir = Vector3::Subset(swordTrail->GetTrail(swordFlame), wepon->GetWeaponEFPoint());
    hitdir = Vector3::Normalize(hitdir);
    SetShakeInput(hitdir, damage * shakePow);
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash1)->Stop();
    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash1)->Play(false, SE);
}
Enemy* Player::CloseEnemy(float lenght){
    Enemy* enemy = nullptr;
    Enemy* returnEnemy = nullptr;
    XMFLOAT3 enemydir{};
    EnemyManager& enemyManager = EnemyManager::Instance();
    float dist = FLT_MAX;
    for (int i = 0; i < enemyManager.GetEnemyCount(); i++) {
        enemy = enemyManager.GetEnemy(i);
        if (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) continue;
        DirToLength(position, { enemy->GetPosition().x,position.y,enemy->GetPosition().z }, enemydir, enemyLength);
        //�����Ŕ���
        if (enemyLength < lenght) {
            if (enemyLength < dist) {
                dist = enemyLength;
                returnEnemy = enemy;
            }
        }
    }
    return returnEnemy;
}
//���̋O��
void Player::SwordEffect() {
    head = wepon->GetWeaponEPoint();  //�O�Ղ̌�
    tail = wepon->GetWeaponEFPoint(); //�O�Ղ̐�
    swordTrail->SetPos(head, tail);   //�O�Ղ̏�����
}
//�W�����v���͏���
bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount < jumpLimit) {
            jumpCount++;
            return true;
        }
    }
    return false;
}
// �U�����͏���
bool Player::InputAttack()
{
    if(wepon->GetBreakFlag(weponType))return false;
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        return true;
    }
    return false;
}
//�e���͏���
void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // ���i�e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_X && mp > swordMp && skillCoolTime[ProjectileColumn] <= 0.0f)
    {
        skillCoolTime[ProjectileColumn] = skillCoolTimeDefault[ProjectileColumn];
        mp-= swordMp;
    		// �O����   		
        float h = 0;
    		// ����
            for (int i = 1; i < 5; i++) {
                h = 0.4*i;
                ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
                projectile->Launch(sword,h,2.5 - 0.5 * i, angle.y, Type::Column, (int)EffectTexAll::EfTexAll::BlueThader, 2 + 0.1 * i,1,0.0f);
            }
            for (int i = 1; i < 5; i++) {
                h = 0.4 * i;
                ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
                projectile->Launch(sword, h,2.5 -0.5 * i, angle.y, Type::Column, (int)EffectTexAll::EfTexAll::BlueThader,2 + 0.1* i, 1, 0.0f,true);
            }
    }
    //if (gamePad.GetButtonDown() & GamePad::BTN_B && mp > swordMp && skillCoolTime[ProjectileRotate] <= 0.0f)
    //{
    //    skillCoolTime[ProjectileRotate] = skillCoolTimeDefault[ProjectileRotate];
    //    mp-= swordMp;
    //    DirectX::XMFLOAT3 pos;
    //    for (int i = 0; i < 16; i++) {
    //        // �O����
    //        DirectX::XMFLOAT3 dir;
    //        // ����
    //        ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
    //        projectile->Launch(sword,height,0.8, angle.y+0.4 * i, Type::Rotate, (int)EffectTexAll::EfTexAll::Thunder,3,1,0.5f);
    //    }   
    //}
    if (gamePad.GetButtonDown() & GamePad::BTN_B && mp > swordMp && skillCoolTime[ProjectileRotate] <= 0.0f)
    {
        skillCoolTime[ProjectileRotate] = skillCoolTimeDefault[ProjectileRotate];
        mp -= swordMp;
        DirectX::XMFLOAT3 pos;
        int rnd = 0;
        for (int i = 0; i < 30; i++) {
            // �O����
            DirectX::XMFLOAT3 dir;
            rnd = rand() % 4;
            // ����
            ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
            projectile->Launch(wepon->GetWeponModel(rnd), height, 0.8, angle.y + 0.4 * i, Type::Sword, (int)EffectTexAll::EfTexAll::Thunder, 6, 1, 0.5f);
        }
    }
}
// �ړ����͏���
bool Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����
    MoveInput(moveVec.x, moveVec.z, moveSpeed);
    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}
//�X�e�b�N���͏���
bool Player::InputStick(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}
//�����ړ�
bool Player::InputBattleMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����
    MoveInput(moveVec.x, moveVec.z, moveSpeed * battelMove);
    //���񏈗�
    Turn(elapsedTime, dir.x, dir.z, turnSpeed);
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}
//�X�e�B�b�N���͂���ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //�J���������ƃX�e�B�b�N�̓��͒l�̂���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX + cameraRightZ * cameraRightX + cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;

    }
    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX + cameraFrontZ * cameraFrontX + cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {   //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
    //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;
    return vec;
}
void Player::UpdateSkill(float elapsedTime)
{
    // ����
    SkillQuick(elapsedTime);
    //����`�F���W
    SkillWeponChange(elapsedTime);
    for (int i = 0; i < SkillMax; ++i)
    {
        if (skillCoolTime[i] >= 0)skillCoolTime[i] -= elapsedTime;
    }
}
void Player::SkillQuick(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    UpdateQuick(elapsedTime);
}
void Player::UpdateQuick(float elapsedTime)
{
  
}
void Player::SkillWeponChange(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN) {
        ButtonWeponChange(WeponType::SubSword);
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_UP) {
        ButtonWeponChange(WeponType::MainSword);
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT) {
        ButtonWeponChange(WeponType::Halberd);
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT) {
        ButtonWeponChange(WeponType::Axe);
    }
    UpdateWeponChange(elapsedTime);
}
void Player::UpdateWeponChange(float elapsedTime)
{
    // ���߃t���O�������ĂȂ������烊�^�[��
    if (!weponChange)return;
    if (skillTime[WeponChange] < 0) {
        weponType = nextWepon;//���̕����ݒ�
        wepon->SetWeponType(weponType);
        if (InputMove(elapsedTime)) {
            wepon->SetRnderflag(true);
            stateMachine->ChangeSubState(static_cast<int>(Player::State::Move));
        }
        else
        {
            wepon->SetRnderflag(true);
            stateMachine->ChangeSubState(static_cast<int>(Player::State::Idle));
        }
        weponChange = false;
    }
    
    skillTime[WeponChange] -= elapsedTime;
    ChangeWepon();
}
void Player::render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader) {
    if (renderflag) {
        shader->Draw(immediate_context.Get(), player.get(), renderdata);
    }
    wepon->Render(immediate_context.Get(), shader);
}
void Player::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader) {

    objectManager.Render(immediate_context.Get(),shader);

}
void Player::SwordTrailrender(RenderContext& rc, SwordTrailShader* shader) {
    
    shader->Draw(rc, swordTrail.get(), {1,1,1,1});

    objectManager.TrailRender(rc, shader);
    
}
//�f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("Position", &position.x);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &angle.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //�X�P�[��
            ImGui::InputFloat3("Scale", &scale.x);
            ImGui::InputInt("health", &health);
            ImGui::SliderFloat("skillcool", &skillCoolTime[0], 0.0f, 1.0f);

            ImGui::SliderFloat("kbpow", &kbpow, 0.0f, 50.0f);
            ImGui::SliderFloat("rollpow", &rollPow, 0.0f, 50.0f);
            ImGui::SliderFloat("swordLength", &swordLength, 0.0f, 500.0f);
            
        }
        if (ImGui::TreeNode("MinComboStatus::movePow,movePowY,attackStart,attackEnd")) {
            ImGui::InputFloat4("MinCombo1Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::MainSword][Combo1].x);
            ImGui::InputFloat4("MinCombo2Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::MainSword][Combo2].x);
            ImGui::InputFloat4("MinCombo3Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::MainSword][Combo3].x);
            ImGui::InputFloat4("MinCombo4Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::MainSword][Combo4].x);
            ImGui::TreePop();
           
        }
        if (ImGui::TreeNode("MinComboPow::knockPow,knockPowY,damage,InvincibleTime")) {
            ImGui::InputFloat4("MinCombo1Pow", &WeponComboPow[WeponType::MainSword][Combo1].x);
            ImGui::InputFloat4("MinCombo2Pow", &WeponComboPow[WeponType::MainSword][Combo2].x);
            ImGui::InputFloat4("MinCombo3Pow", &WeponComboPow[WeponType::MainSword][Combo3].x);
            ImGui::InputFloat4("MinCombo4Pow", &WeponComboPow[WeponType::MainSword][Combo4].x);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("AxeComboStatus::movePow,movePowY,attackStart,attackEnd")) {
            ImGui::InputFloat4("AxeCombo1Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::Axe][Combo1].x);
            ImGui::InputFloat4("AxeCombo2Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::Axe][Combo2].x);
            ImGui::InputFloat4("AxeCombo3Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::Axe][Combo3].x);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("AxeComboPow::knockPow,knockPowY,damage,InvincibleTime")) {
            ImGui::InputFloat4("AxeCombo1Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::Axe][Combo1].x);
            ImGui::InputFloat4("AxeCombo2Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::Axe][Combo2].x);
            ImGui::InputFloat4("AxeCombo3Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::Axe][Combo3].x);
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNode("HBComboStatus::movePow,movePowY,attackStart,attackEnd")) {
            ImGui::InputFloat4("HBCombo1Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::Halberd][Combo1].x);
            ImGui::InputFloat4("HBCombo2Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::Halberd][Combo2].x);
            ImGui::InputFloat4("HBCombo3Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::Halberd][Combo3].x);
            ImGui::TreePop();
           
        }
        if (ImGui::TreeNode("HBComboPow::knockPow,knockPowY,damage,InvincibleTime")) {
            ImGui::InputFloat4("HBCombo1Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::Halberd][Combo1].x);
            ImGui::InputFloat4("HBCombo2Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::Halberd][Combo2].x);
            ImGui::InputFloat4("HBCombo3Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::Halberd][Combo3].x);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("EXComboStatus::movePow,movePowY,attackStart,attackEnd")) {
            ImGui::InputFloat4("EXCombo1Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::SubSword][Combo1].x);
            ImGui::InputFloat4("EXCombo2Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::SubSword][Combo2].x);
            ImGui::InputFloat4("EXCombo3Status,movePow,movePowY,attackStart,attackEnd", &WeponComboStatus[WeponType::SubSword][Combo3].x);
            ImGui::TreePop();
           
        }
        if (ImGui::TreeNode("EXComboPow::knockPow,knockPowY,damage,InvincibleTime")) {
            ImGui::InputFloat4("EXCombo1Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::SubSword][Combo1].x);
            ImGui::InputFloat4("EXCombo2Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::SubSword][Combo2].x);
            ImGui::InputFloat4("EXCombo3Pow,knockPow,knockY,damage,InvincibleTime", &WeponComboPow[WeponType::SubSword][Combo3].x);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�N����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1, 1, 1, 1));
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));
    debugRenderer->DrawSphere(wepon->GetWeaponEPoint(), 0.2, DirectX::XMFLOAT4(0, 1, 0, 1));

    Animation::Keyframe::node* leftHandBone = player->FindNode("Wp_Sword");
    //leftHandBone->globalTransform * transform;
    if (leftHandBone) {
        XMStoreFloat4x4(&world, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));
        if (attackCollisionFlag)
            debugRenderer->DrawSphere(DirectX::XMFLOAT3(
                world._41 + world._31 * swordLength,
                world._42 + world._32 * swordLength,
                world._43 + world._33 * swordLength),
                wepon->GetRadius()*weponRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1)
            );
    }
    //debugRenderer->DrawSphere(wepon->GetWeaponPoint(),
    //    weponRadius,
    //    DirectX::XMFLOAT4(1, 0, 0, 1));
}

//�v���C���[�ƓG�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyhitflag = false;
    //�S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++) {
        Enemy* enemy = enemyManager.GetEnemy(i);
        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (enemy->GetEnemyKeyframe().nodes.size() > 0) {//�����蔻��p�̃m�[�h�������
            for (Animation::Keyframe::node& node : enemy->GetEnemyKeyframe().nodes)
            {
                if (Collision::IntersectSphereVsCylinder(node.translation,
                    enemy->GetNodeRudius(),
                    position,
                    radius,
                    height,
                    outPosition)) {
                    enemyhitflag = true;
                         {
                             //	 �����o����̈ʒu�ݒ�
                             position.x = outPosition.x;
                             position.y = outPosition.y;
                             position.z = outPosition.z;
                         }
                 }
            }    
        }
        else
        {

            if (Collision::IntersectCylinderVsCylinder(
                enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
                position, radius, height,
                outPosition
            )) {
                enemyhitflag = true;
                {
                    //	 �����o����̈ʒu�ݒ�
                    position.x = outPosition.x;
                    position.y = outPosition.y;
                    position.z = outPosition.z;
                }
            }
        }
    }
}
void Player::CollisionNodeVsEnemies(float nodeRadius,DirectX::XMFLOAT2 pow, float Damage, float InvincibleTime)
{
    //�w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int enemyindex = 0; enemyindex < enemyCount; enemyindex++)
    {
        Enemy* enemy = enemyManager.GetEnemy(enemyindex);
        DirectX::XMFLOAT3 enemyPosition, impulse;
        if (enemy->GetEnemyKeyframe().nodes.size() > 0) {
            for (Animation::Keyframe::node& node : enemy->GetEnemyKeyframe().nodes)
            {
                if (Collision::IntersectCapsuleVsSphere(
                    wepon->GetWeaponEFPoint(),
                    wepon->GetWeaponEPoint(),
                    wepon->GetRadius() * nodeRadius,
                    node.translation,
                    1.0))
                {
                    if (enemy->ApplyDamage(Damage, InvincibleTime) && wepon->GetSwordState() != SwordState::Stop) {
                        //�m�b�N�o�b�N
                        KnockBack(*enemy, this->position, enemy->GetPosition(), pow.x, pow.y);
                        //�q�b�g�����͏���
                        HitInput(Damage, InvincibleTime);
                        DirectX::XMFLOAT3 trailDir = GetSlashDir();
                        //�q�b�g�G�t�F�N�g
                        ParticleSprite* particleSprite = new ParticleSprite(wepon->GetWeaponEFPoint(), trailDir, ParticleSprite::ParticleLine, ParticleSprite::Slash, int(EffectTexAll::EfTexAll::Distortion), 1, 0.5);
                        particleSprite = new ParticleSprite(wepon->GetWeaponEFPoint(), trailDir, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, int(EffectTexAll::EfTexAll::Impact), 1, 0.3);
                        particleSprite = new ParticleSprite(enemyPosition, wepon->GetWeaponEFPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 1000, 0.5);
                       
                    }
                }
                
            }
        }
        else
        {
            enemyPosition = { enemy->GetPosition().x,enemy->GetPosition().y + enemy->GetHeight() / 2,enemy->GetPosition().z };
            if (Collision::IntersectCapsuleVsSphere(
                wepon->GetWeaponEFPoint(),
                wepon->GetWeaponHPoint(),
                wepon->GetRadius() * nodeRadius,
                enemyPosition,
                enemy->GetRadius()))
            {
                if (enemy->ApplyDamage(Damage, InvincibleTime) && wepon->GetSwordState() != SwordState::Stop) {
                    //�m�b�N�o�b�N
                    KnockBack(*enemy, this->position, enemy->GetPosition(), pow.x, pow.y);
                    //�q�b�g�����͏���
                    HitInput(Damage, InvincibleTime);
                    DirectX::XMFLOAT3 trailDir = GetSlashDir();
                    //�q�b�g�G�t�F�N�g
                    ParticleSprite* particleSprite = new ParticleSprite(enemyPosition, trailDir, ParticleSprite::ParticleLine, ParticleSprite::Slash, int(EffectTexAll::EfTexAll::Distortion), 1, 0.5);
                    particleSprite = new ParticleSprite(enemyPosition, trailDir, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, int(EffectTexAll::EfTexAll::Impact), 1, 0.3);
                    particleSprite = new ParticleSprite(enemyPosition, wepon->GetWeaponEFPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 1000, 0.5);
                    if (enemy->GetBarrierFlag()) {
                        //particleSprite = new ParticleSprite(enemyPosition, wepon->GetWeaponEFPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 1000, 0.5);
                        particleSprite = new ParticleSprite(enemyPosition, {NULL,NULL,NULL}, ParticleSprite::ParticleTriangle, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 1000, 0.5,1);
                    }
                }
            }
        }
    }
}

void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //�S�Ă̒e�ۂƂ��ׂĂ̓G�𑍓�����ŏՓ˔���
    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = objectManager.GetObjectCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        Object* object = objectManager.GetObjectW(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);
            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                object->GetPosition(),
                object->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                // �_���[�W��^����
                
                  if (enemy->ApplyDamage(object->GetDamage(),object->GetInvincibleTime()))
                  {
                      
                      KnockBack(*enemy, this->position, enemy->GetPosition(), 1, 0);
                    
                      SetShakeInput({ NULL,1,NULL }, object->GetDamage());//�J�����V�F�C�N
                      if (object->GetType() == Type::Straight) {
                          //ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::Sumi), 2, { 1,1,1,1 });
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Stop();
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Play(false, SE);
                          object->Destroy();
                          XMFLOAT3 start = Vector3::PosDir(object->GetPosition(), object->GetDirectionUp(), object->GetScale().x*2);
                          XMFLOAT3 end = Vector3::PosDir(object->GetPosition(), object->GetDirectionUp(), -object->GetScale().x*2);
                          ParticleSprite* particleSprite = new ParticleSprite(start, end, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Sumi), 2000, 1.5, 0, true, 0.05, { 1,1,1,1 });
                          SetShakeInput({ 0,1,0 }, 4);
                          continue;
                      }
                      if (object->GetType() == Type::Column) {
                          ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::BlueThader), 1, { NULL,NULL,2,1 });
                         // ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), object->GetPosition(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 1000, 1.5);
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Stop();
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Play(false, SE);
                          ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 1000, 1.5, 0, true, 0.05, {0,0,1,1});
                          SetShakeInput({0,1,0}, 2);
                          object->Destroy();
                          continue;
                      }
                      //�����_���a���G�t�F�N�g
                      DirectX::XMFLOAT3 trailDir;
                      float angle = DirectX::XMConvertToRadians(rand() % 360);
                      trailDir.x = sinf(angle);
                      trailDir.y = cosf(angle);
                      trailDir.z = 0;
                      //EffectAll::Instance().hitEffect->Play(object->GetPosition(), 0.1);
                      ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), trailDir, ParticleSprite::ParticleLine, ParticleSprite::Slash, int(EffectTexAll::EfTexAll::Distortion), 1, 0.5);
                      AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash1)->Stop();
                      AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash1)->Play(false, SE);
                      //�e�۔j��
                     
                  }
               
                
            }
        }
    }
}
void Player::Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader) {
    //UI�n�`��
    //wepon->DrawUI(dc,rc,shader);
}
void Player::UpdateUI(float elapsedTime) {
    hpTimer += 2;
    uiHp->SetGauge(health);
    uiMp->SetGauge(mp);
}
