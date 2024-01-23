#include "Player.h"
#include "Input.h"
#include "Camera.h"
#include "Collision.h"
#include "Graphics.h"
#include "EnemyManager.h"
#include "Afterimage.h"
#include "ProjectileStraite.h"
//#include "EffectAll.h"
//#include "EnemyDragon.h"
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
#include "ParticleManager.h"
#include "Mathf.h"
#include "EnemySystem.h"
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

// インスタンス取得
Player& Player::Instance()
{
    return *instance;
}
Player::Player() {
    // インスタンスポイント設定
    instance = this;

    player = std::make_unique<Model>(".\\resources\\Player\\playermode5l.fbx", true);
    player->AppendAnimations(".\\resources\\Player\\taiki1_2.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\dash.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\Dash2.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\jump4.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\jump_6.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\jump2.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\jumpAttack.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\Lond.fbx", 0);
     player->AppendAnimations(".\\resources\\Player\\ATK1_4.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\ATK2_2.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\ATK4_2.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\ATK3_2.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\hit_02.fbx", 0);
    player->AppendAnimations(".\\resources\\Player\\dead_01.fbx", 0);
    player->ModelSerialize(".\\resources\\Player\\playermode5l.fbx");
   // player->ModelCreate(".\\resources\\AimTest\\GreatSword.fbx");
    player->ModelRegister(".\\resources\\Player\\playermode5l.fbx", "Texture\\textureSet_BaseColor.png");

	player->PlayAnimation(0, true);

    sword = std::make_unique<Model>(".\\resources\\Sword.fbx", true);
    sword->ModelSerialize(".\\resources\\Sword.fbx");
    sword->ModelRegister(".\\resources\\Sword.fbx");
    sword->UpdateBufferData(transform);
    slash = std::make_unique<Model>(".\\resources\\Textures\\zanngeki4.fbx", true);
    slash->ModelSerialize(".\\resources\\Textures\\zanngeki4.fbx");
    slash->ModelRegister(".\\resources\\Textures\\zanngeki4.fbx");
    slash->UpdateBufferData(transform);
    beem = std::make_unique<Model>(".\\resources\\Cube.fbx", true);
    beem->ModelSerialize(".\\resources\\Cube.fbx");
    beem->ModelRegister(".\\resources\\Cube.fbx");
    beem->UpdateBufferData(transform);
    //wepon = std::make_unique<MainWepon>();
    maxHealth = 20;
    health = maxHealth;
    mp = mpMax;
    UIManager& uiManager = UIManager::Instance();
    const float gaugeWidth = 500.0f;
    const float gaugeHeight = 500.0f;
    uiHp = new GaugeUI(L".\\resources\\HP2.png", 10.0f, 395.0f, gaugeWidth * 0.9f, gaugeHeight * 0.9f, health, maxHealth);
    //uiMp = new GaugeUI(L".\\resources\\MP.png", 32, 657, gaugeWidth + 11.0f, gaugeHeight * 0.2, mp, mpMax);
    //UI[UINo::Die] = std::make_unique<Sprite>(L".\\resources\\UI\\Die.png");
    swordTrail = std::make_unique<SwordTrail>(10, L".\\resources\\SwordTrail.jpg");
    cameraController = std::make_unique<CameraController>();
    moveSpeed = 20;
    maxMoveSpeed = 20;
    friction = 0.5f;
    player->UpdateBufferData(transform);
    //renderdata = player->GetBufferData();
    for (int i = 0; i < 13; i++) {
        lowerBody.push_back(lower[i]);
    }
    stateMachine = std::make_unique<StateMachine<Player>>();
    // ステートマシンに1層目のステートを登録(BattleStateも同様の方法で各自追加してください)
    stateMachine->RegisterState(new PlayerState(this));
    // 各親ステートにサブステートを登録(WanderState以外の2層目のステートも同様の方法で各自追加してください。)
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
    // ステートをセット
    stateMachine->SetState(0);
    scale.x = scale.y = scale.z = 1.5;
    position = { 100,0,-130 };

    // ホログラムシェーダー情報初期化
    HologramShaderDataInitialize(0.0f, 200.0f);
    nextStateTimer = 0.5f;
}
Player::~Player()
{
}
void Player::update(float elapsedTime) 
{
    timer += elapsedTime;
    if (stateMachine->GetStateNum() == static_cast<int>(State::Damage) || stateMachine->GetStateNum() == static_cast<int>(State::Down) || stateMachine->GetStateNum() == static_cast<int>(State::Dead))
    {
        lerpGlitchIntensity = 1.0f;
    }
    else
    {
        lerpGlitchIntensity = 0.0f;
    }
    glitchIntensity = Mathf::Lerp(glitchIntensity, lerpGlitchIntensity, elapsedTime * 20.0f);
    player->ShaderAdjustment(glitchScale, timer, maxHeight, hologramColor);

    if (health <= 0)
    {
        hologramTimer = 0.0f;
    }

    float ElapsedTime = elapsedTime * modelSpeed;
    if (quickFlag) ElapsedTime *= 0.6f;
    if (attackHitflag) ElapsedTime *= attackHitPow;
    animeTimer = ElapsedTime;
    //ロックオンカメラ
    ComeTerget(elapsedTime);
    
    // ホログラムシェーダー実行中フラグが付いていれば
    if (!isActiveStart)
    {
        // ホログラムシェーダー更新処理
        isActiveStart = UpdateHologramShader(elapsedTime);

        animeTimer = 0.0f;
    }
    else
    {
        //ステートマシン更新
        stateMachine->Update(elapsedTime);
    }
    //player->PlayAnimation(-1,false);
    //アニメーション更新
    player->UpdateAnimation(animeTimer, "koshi");
    player->UpdateSubAnimation(animeTimer, "koshi");
    
    //weponRenderUpdate();
    //速力処理更新
    UpdateVelocity(ElapsedTime);
    UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
    //描画情報更新
    player->UpdateBufferData(transform);

    if (!isActiveStart)
    {
        return;
    }
    //武器情報入力
    //SetWepon();
    //武器更新
    //wepon->Update(elapsedTime);
    // スキル効果
    UpdateSkill(elapsedTime);
    //弾発射処理
    //InputProjectile();
    //剣軌跡更新
    swordTrail->Update();
  
    AudioUpdate();
    //ヒットフラグ更新
    UpdateHitflag(elapsedTime);
    // 無敵時間更新
    UpdateInvincibleTimer(ElapsedTime);

    //UI更新処理
    UpdateUI(elapsedTime);

	//弾更新
    objectManager.Update(ElapsedTime);
    //描画してたら当たり判定
    if (renderflag) {
        CollisionPlayerVsEnemies();
    }
    //弾当たり判定
    CollisionProjectilesVsEnemies();

    CollisionBoomVsEnemies();
    //描画情報入力
    //renderdata = player->GetBufferData();
    frameTimer++;
    //uiアニメーション用更新
    //swordEffectColor.w ++;
}
void Player::TitleUpdate(float elapsedTime)
{
    switch (titleState)
    {
    case TitleState::TitleDefault:
        break;
    case TitleState::TitlePunchStart:
    {
        stateMachine->ChangeSubState(static_cast<int>(Player::State::Attack));//着地
        player->PlayAnimation(WeponCombo[3], false);
        titleState = TitleState::TitlePunchNow;
    }
    case TitleState::TitlePunchNow:
    {
        animationTimer = player->GetCurrentAnimationSeconds();
        float nextStateTimer = 0.8f;
        if (animationTimer >= nextStateTimer)
        {
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::SlashFly)->Play(false, 0.3f);
            titleState = TitleState::TitlePunchReverberation;
        }
    }
    break;
    case TitleState::TitlePunchReverberation:
        break;    
    }

    timer += elapsedTime;
    player->ShaderAdjustment(glitchScale, timer, maxHeight, hologramColor);
    float ElapsedTime = elapsedTime * modelSpeed;
    animeTimer = ElapsedTime;

    // ホログラムシェーダー実行中フラグが付いていれば
    if (!isActiveStart)
    {
        // ホログラムシェーダー更新処理
        isActiveStart = UpdateHologramShader(elapsedTime * 2.0f);

        animeTimer = 0.0f;
    }

    //アニメーション更新
    player->UpdateAnimation(animeTimer, "koshi");
    player->UpdateSubAnimation(animeTimer, "koshi");

    //速力処理更新
    UpdateVelocity(ElapsedTime);
    UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
    //描画情報更新
    player->UpdateBufferData(transform);
}
void Player::ClearUpdate(float elapsedTime)
{
    timer += elapsedTime;
    player->ShaderAdjustment(glitchScale, timer, maxHeight, hologramColor);
    float ElapsedTime = elapsedTime * modelSpeed;
    animeTimer = ElapsedTime;
    dir = { 0.0f, 0.0f, 1.0f };
    switch (clearState)
    {
    case ClearState::ClearDefault:
        position = { 100.0f, -2.5f, -130.0f };
        comboflag = true;//コンボ先行入力
        farPosition = position.z + 5.0f;
        combo = 3;
        stateMachine->ChangeSubState(static_cast<int>(Player::State::Attack));//着地
        clearState = ClearState::ClearPunchStart;
        break;
    case ClearState::ClearPunchStart:
    {
        attackStart = 0.6f;
        float animationTime = player->GetCurrentAnimationSeconds();
        if (animationTime > attackStart) 
        {            
            clearState = ClearState::ClearPunchNow;
            farPosition = Base::Instance().GetPosition().z + 5.0f;
        }
        else if (animationTime > 0.25f && animationTime < 0.253f)
        {
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Clear)->Play(false, 0.3f);
        }
    }
        break;
    case ClearState::ClearPunchNow:
    {
        attackEnd = 0.8f;
        float animationTime = player->GetCurrentAnimationSeconds();        
        if (animationTime >= attackStart && animationTime <= attackEnd)
        {
            position.z = Mathf::Lerp(position.z, Base::Instance().GetPosition().z + 30.0f, elapsedTime * 15.0f);
        }
        else
        {
            clearState = ClearState::ClearPunchReverberation;
        }              
    }
        break;
    case ClearState::ClearPunchReverberation:
        break;
    }

    
    ComeTerget(elapsedTime);

    //ステートマシン更新
    if(clearState < ClearState::ClearPunchNow)
    stateMachine->Update(elapsedTime);

    //アニメーション更新
    player->UpdateAnimation(animeTimer, "koshi");
    player->UpdateSubAnimation(animeTimer, "koshi");

    //速力処理更新
    UpdateVelocity(ElapsedTime);
    UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
    //描画情報更新
    player->UpdateBufferData(transform);
}
void Player::AudioUpdate() {
    //移動以外ならダッシュSEストップ
    if (state != State::Move) AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Run)->Stop();
}
void Player::SearchNode(DirectX::XMFLOAT4X4& Transform, const char* nodeName) {
    Animation::Keyframe::node* leftHandBone = player->FindNode(nodeName); //ノード取得
    XMStoreFloat4x4(&Transform, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));//ノードワールド座標変換
}
DirectX::XMFLOAT3 Player::SearchNodePos(const char* nodeName) {
    DirectX::XMFLOAT4X4 nodeTransform = {};
    Animation::Keyframe::node* leftHandBone = player->FindNode(nodeName); //ノード取得
    XMStoreFloat4x4(&nodeTransform, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));//ノードワールド座標変換
    return { nodeTransform._41,nodeTransform._42,nodeTransform._43 };
}
void Player::SetWepon() {
    if (weponType == WeponType::MainSword) {
        SearchNode(weponTransform, "hand_l");//左手ノード取得
        shieldHead = { weponTransform._41,weponTransform._42,weponTransform._43 };//盾の座標入力
    }
    //SearchNode(weponTransform, "Wp_Sword");//剣ノード取得
    head = { weponTransform._41,weponTransform._42,weponTransform._43 };//腕の座標入力
    sworddir = { weponTransform._31,weponTransform._32,weponTransform._33 };//剣の向き取得
    sworddir = Vector3::Normalize(sworddir);//剣の向き正規化
    //pattern = wepon->GetWeaponPPoint();//武器の柄取得
    //tail = wepon->GetWeaponPoint();//武器の先取得
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
    //if (wepon->GetDisslve() == 1) {
    //    wepon->SetSwordState(SwordState::Nomal);
    //    wepon->SetRnderflag(true);
    //}

}
void Player::SetAttackStatus(DirectX::XMFLOAT4 Status, DirectX::XMFLOAT4 Pow){
    //ノックバックパワー入力
    knockBackpow.x = Pow.x;
    knockBackpow.y = Pow.y;
    attackStart = Status.z;
    attackEnd = Status.w;
    //上昇パワー
    velocity.y = Status.y;
    //攻撃時の動きパワー
    attackMove = Status.x;
    attackDamage = Pow.z;
    attackInvincibleTime = Pow.w;
}
//ターゲットの距離と方向取得
void Player::DirToLength(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3& dir, float& length) {
    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
    DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
    length = DirectX::XMVectorGetX(D);
    DirectX::XMStoreFloat3(&dir, N);
}
void Player::ComeTerget(float elapsedTime) 
{
    if (!isActiveStart)
    {
        DirectX::XMFLOAT3 camePos = position;
        camePos.y += height;
        cameraController->SetTarget(camePos);
        cameraController->EntryUpdate(elapsedTime);
    }
    else if (health <= 0 || Base::Instance().GetHP() <= 0)
    {
        if (explosionTimer <= 0.0f)
        {
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Bgm)->Stop();
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Lose)->Play(false, 0.6f);
        }
        explosionTimer += elapsedTime;
        if (explosionTimer < 3.0f)
        {
            if (static_cast<int>(explosionTimer * 10) % 5 == 2)
            {
                DirectX::XMFLOAT3 spawnPosition = Base::Instance().GetPosition();
                spawnPosition.x += static_cast<float>(rand() % 40) - 20.0f;
                spawnPosition.y += static_cast<float>(rand() % 20);
                spawnPosition.z += static_cast<float>(rand() % 4) - 2.0f;
                ParticleSystem::Instance().BoomEffect(spawnPosition, 3, int(EffectTexAll::EfTexAll::Flame), 10, 0.5f, { NULL, NULL, 2, 1 });                
            }
            if (static_cast<int>(explosionTimer * 10) % 5 == randomValue)
            {
                SetShakeInput({ 0.0f, 1.0f, 0.0f }, 5.0f);
                randomValue = rand() % 5;
                AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Stop();
                AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Play(false, 0.5f);
            }
        }
        DirectX::XMFLOAT3 cameraPosision = Base::Instance().GetPosition();
        cameraPosision.y += 6.5f;
        cameraPosision.z += 30.0f;
        cameraController->SetTarget(cameraPosision);
        cameraController->GameAfterUpdate(elapsedTime);
        
        loseDirectingTimer += elapsedTime;
    }
    else if (EnemySystem::Instance().GetWave() == 3)
    {
        DirectX::XMFLOAT3 cameraPosision = position;
        cameraPosision.y += height;
        cameraPosision.z = Mathf::Lerp(cameraPosision.z, farPosition, elapsedTime * 8.0f);        
        clearDirectingTimer += elapsedTime;
        loseDirectingTimer += elapsedTime;
        if (clearState < ClearState::ClearPunchReverberation) 
        {
            cameraController->SetTarget(cameraPosision);
            cameraController->GameAfterUpdate(elapsedTime);
        }
    }
    else
    {
        //エネミーへの方向ベクトル  
        DirectX::XMFLOAT3 enemydir = {};
        //ターゲット位置
        DirectX::XMFLOAT3 target = {};
        //一番近くの敵をターゲットにする
        EnemyManager& enemyManager = EnemyManager::Instance();
        float dist = FLT_MAX;
        float d = 0;
        //エネミー数
        int enemyCount = enemyManager.GetEnemyCount();
        if (fallDeath)return;
        if (enemyCount == 0) { // ロックオンする敵がいない
            DirectX::XMFLOAT3 camePos = position;
            camePos.y += height;
            cameraController->SetTarget(camePos);
            cameraController->Update(elapsedTime);
            return;
        }
        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        int x = 0;
        if (ax > 0.5) x = 1;  //右ステック入力設定
        if (ax < -0.5) x = -1;//右ステック入力設定
        if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_THUMB) {//右ステック押し込み
            locklength = !locklength;//ロックオン状態変化
            fistlock = true;//始めてロックオン
        }
        Enemy* enemy = nullptr;
        switch (x)
        {
            //ロックオン入力リセット
        case 0:
            lockflag = false;
            break;
            //次のロックオンターゲットへ
        case 1:
            if (lockflag)break;//右ステック入力され続づけてたら
            if (!locklength)break;//ロックオン状態じゃないなら
            enemyLock++;
            if (enemyLock >= enemyCount) enemyLock = 0;//ロックオン対象がエネミー数を超えたら0に
            lockflag = true; //右ステック入力されたか
            enemy = enemyManager.GetEnemy(enemyLock);//エネミー取得
            while (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) {//エネミーのHPがあるか、アクティブになっているか
                enemyLock++;
                if (enemyLock >= enemyCount) enemyLock = 0;//ロックオン対象がエネミー数を超えたら0に
                enemy = enemyManager.GetEnemy(enemyLock);
            }
            enemyId = enemy->GetId();//ID取得
            break;
            //前のロックオンターゲットへ
        case -1:
            if (lockflag)break;//右ステック入力され続づけてたら
            if (!locklength)break;//ロックオン状態じゃないなら
            enemyLock--;
            lockflag = true;//右ステック入力されたか
            if (enemyLock < 0) enemyLock = enemyCount - 1;//ロックオン対象が0より下なら最大数
            enemy = enemyManager.GetEnemy(enemyLock);
            while (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) {//エネミーのHPがあるか、アクティブになっているか
                enemyLock--;
                if (enemyLock < 0) enemyLock = enemyCount - 1;//ロックオン対象が0より下なら最大数
                enemy = enemyManager.GetEnemy(enemyLock);
            }
            enemyId = enemy->GetId();//ID取得
        }
        //ロックオン入力がされたら
        if (locklength) {
            //始めてロックオン入力されたら最も近い敵をロックオン
            if (!fistlock)
            {
                enemy = enemyManager.GetEnemyFromId(enemyId);
                if (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) {//エネミーのHPがあるか、アクティブになっているか
                    fistlock = true;
                }
                DirToLength(position, { enemy->GetPosition().x,position.y,enemy->GetPosition().z }, enemydir, enemyLength);
                //距離で判定
                if (enemyLength < 6000) {
                    target = enemy->GetPosition();
                    //target.y += enemy->GetHeight() * 0.5;
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
            if (fistlock) {//始めてのロックオン
                dist = FLT_MAX;
                for (int i = 0; i < enemyCount; i++) {
                    enemy = enemyManager.GetEnemy(i);
                    if (enemy->GetHealth() <= 0 || !enemy->GetActiveflag()) continue;
                    DirToLength(position, { enemy->GetPosition().x,position.y,enemy->GetPosition().z }, enemydir, enemyLength);
                    float a = enemyLength;
                    if (d == 0) d = a;//距離代入
                    //距離で判定
                    if (a < 4000) {
                        if (a < dist)
                        {
                            dist = a;
                            target = enemy->GetPosition();
                            //target.y += enemy->GetHeight() * 0.5f;
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
        //ロックオンしてるならカメラ設定
        DirectX::XMFLOAT3 camePos = position;
        if (state == Attack) {
            //camePos.y = SearchNodePos("root_x").y;
            camePos.y += height;
        }
        else
        {
            camePos.y = position.y;
            camePos.y += height;
        }
        cameraController->SetRangeMax(cameraRange);
        cameraController->SetCorrectionSpeed(correctionSpeed);
        cameraController->SetTarget(camePos);
        if (lockOn) {
            cameraController->SetTarget2(target);
            enemyLength = dist;
            dist *= 0.1f;//距離補正
            cameraController->Update2(elapsedTime, { dir.x,NULL,dir.z }, dist);
        }
        else {
            cameraController->Update(elapsedTime);
        }
    }   
}


//着地した時に呼ばれる
void Player::OnLanding()
{
    jumpCount = 0;//ジャンプカウント初期化
    if (velocity.y < NULL && landflag)
    {
        stateMachine->ChangeSubState(static_cast<int>(Player::State::Land));//着地
    }
}

void Player::OnDamaged()
{
    jumpCount = 0;//ジャンプカウント初期化
    // ダメージステートへ遷移
    if (state != State::Attack) {
        stateMachine->ChangeSubState(static_cast<int>(Player::State::Damage));
    }
}
void Player::OnDead()
{
    // ダメージステートへ遷移
   
     stateMachine->ChangeSubState(static_cast<int>(Player::State::Dead));
    
}
void Player::PlayerKnockBack(DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY,float damage)
{
    EffectAll::Instance().stingerEffect->Stop(stingerEffect);//エフェクト停止
    DirectX::XMFLOAT3 inpulse = Vector3::Subset(position, mypos);//ベクトル生成
    inpulse = Vector3::Multip(Vector3::Normalize(inpulse),power);//正規化してノックバックパワーをかける
    SetShakeInput(inpulse, damage);//カメラシェイク
    //if (state == State::Guard) {
    //    if (!guardFlag) {
    //        //wepon->Exhaustion(damage);//武器消耗
    //    }
    //    //player->PlayAnimation(AnimGuardhit, false);
    //    //if (wepon->GetWeponLife(weponType) < 0) {//盾壊れたら
    //    //    player->PlayAnimation(AnimGuardbreak, false);
    //    //    invincibleTimer = 2.0;
    //    //    guardDir.x = -inpulse.x;
    //    //    guardDir.y = -inpulse.z;
    //    //    guardDir = Vector2::Normalize(guardDir);
    //    //    return;
    //    //}
    //    inpulse.x *= guardKnockBack;
    //    inpulse.z *= guardKnockBack;
    //    AddImpulse(inpulse);//ノックバック
    //    //くらった方向を向く
    //    guardDir.x = -inpulse.x;
    //    guardDir.y = -inpulse.z;
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Gud)->Stop();
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Gud)->Play(false, SE);
    //    return;
    //}
    if (power > kbpow || health <= 0) {
        //player->PlayAnimation(0, false);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, SE);
        downflag = true;//ダウン状態
        //コンボ初期化
        combo = 0;
    }
    else if (downflag) {
        //player->PlayAnimation(0, false);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, SE);
        downflag = true;//ダウン状態
    }
    else
    {
        //player->PlayAnimation(0, false);
        //コンボ初期化
        combo = 0;
    }
    inpulse.y = powerY;//上方向のノックバック
    AddImpulse(inpulse);//ノックバック
    //くらった方向を向く
    dir.x = -inpulse.x;
    dir.z = -inpulse.z;
}

void Player::SpecialUpdate() {
    //if (InputGame::InputSpecial()) {
    //    switch (weponType)//武器によって特殊攻撃
    //    {
    //    case WeponType::MainSword:
    //        stateMachine->ChangeSubState(static_cast<int>(Player::State::Guard));
    //        break;
    //    case WeponType::Axe:
    //        stateMachine->ChangeSubState(static_cast<int>(Player::State::Charge));
    //        break;
    //    case WeponType::Halberd:
    //        if (mp > 0) {
    //            mp--;
    //            stateMachine->ChangeSubState(static_cast<int>(Player::State::Lance));
    //        }
    //        break;
    //    case WeponType::SubSword:
    //        if (mp > 0) {
    //            mp--;
    //            stateMachine->ChangeSubState(static_cast<int>(Player::State::Stinger));
    //        }
    //    default:
    //        break;
    //    }
    //}

}
void Player::BattleMoveUpdate() {
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //前方向と入力方向の内積でアニメーション変化
    DirectX::XMFLOAT2 moveVec = { GetMoveVec().x,GetMoveVec().z };
    DirectX::XMFLOAT2 Vec;
    Vec.x = sinf(angle.y);
    Vec.y = cosf(angle.y);
    moveVec = Vector2::Normalize(moveVec);
    float dot = Vector2::Dot(moveVec, Vec);
    dot = acos(dot);

    // 歩きアニメーション再生
    //if (ax > 0 && dot > upSens && dot < disUpSens) {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkFrontLeft) {
    //        player->PlaySubAnimation(AnimWorkFrontLeft, true,lowerBody);        
    //    }
    //}
    //else if (ax < 0  && dot >  upSens && dot < disUpSens) {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkFrontRight) {
    //        player->PlaySubAnimation(AnimWorkFrontRight, true,lowerBody);     
    //    }
    //}
    //else if (ax > 0 && dot > disUpSens && dot < disDownSens) {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkLeft) {
    //        player->PlaySubAnimation(AnimWorkLeft, true, lowerBody);
    //       
    //    }
    //}
    //else if (ax < 0 && dot >  disUpSens && dot < disDownSens) {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkRight) {
    //        player->PlaySubAnimation(AnimWorkRight, true,lowerBody);  
    //    }
    //}
    //else if (ax > 0 && dot > disDownSens && dot < downSens) {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkBackLeft) {
    //        player->PlaySubAnimation(AnimWorkBackLeft, true, lowerBody);
    //    }
    //}
    //else if (ax < 0 && dot >  disDownSens && dot < downSens) {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkBackRight) {
    //        player->PlaySubAnimation(AnimWorkBackRight, true, lowerBody);
    //    }
    //}
    //else if (ay > 0 ) {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkFront) {
    //        player->PlaySubAnimation(AnimWorkFront, true, lowerBody);
    //       
    //    }
    //}
    //else {
    //    if (player->GetCurrentSubAnimationIndex() != AnimWorkBack) {
    //        player->PlaySubAnimation(AnimWorkBack, true, lowerBody);
    //       
    //    }
    //}
}
void Player::DownUpdate() {
    if (!downflag)return;
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //前方向と入力方向の内積でアニメーション変化
    DirectX::XMFLOAT2 moveVec = { GetMoveVec().x,GetMoveVec().z };
    moveVec = Vector2::Normalize(moveVec);
    DirectX::XMFLOAT2 Vec = { Camera::Instance().GetFront().x,Camera::Instance().GetFront().z };
    float dot = Vector2::Dot(moveVec, Vec);
    dot = acos(dot);
    // 復帰アニメーション再生
    //if (ax > 0 && dot > upSens && dot < disDownSens) {
    //        player->PlayAnimation(AnimDownUpLeft, false);
    //        velocity.x = moveVec.x * rollPow*returnPow;
    //        velocity.z = moveVec.y * rollPow*returnPow;
    //        downflag = false;
    //}
    //else if (ax < 0 && dot >  upSens && disDownSens) {
    //        player->PlayAnimation(AnimDownUpRight, false);
    //        velocity.x = moveVec.x * rollPow * returnPow;
    //        velocity.z = moveVec.y * rollPow * returnPow;
    //        downflag = false;
    //}
    //else if (ay > 0) {
    //        player->PlayAnimation(AnimDownCUp, false);
    //        downflag = false;
    //}
    //else if (ay < 0 ){
    //     player->PlayAnimation(AnimDownUp, false);
    //     downflag = false;
    //}
}
//DirectX::XMFLOAT3 Player::GetSlashDir() {
//    //DirectX::XMFLOAT2 trailPos = Vector3::C3Dor2D(swordTrail->GetTrail(swordFlame));//変数＋１フレーム前の剣先
//    //DirectX::XMFLOAT2 weaponPoint = Vector3::C3Dor2D(wepon->GetWeaponEFPoint());//現在の剣先
//    //trailPos = Vector2::Subset(weaponPoint,trailPos);
//    //trailPos = Vector2::Normalize(trailPos);
//    //DirectX::XMFLOAT3 trailDir = { trailPos.x,trailPos.y,NULL };
//    //return trailDir;
//}
void Player::SlashInput() {
    if (slashCombo != combo ) {
        //wepon->Exhaustion(attackDamage);
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::SlashFly)->Stop();
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::SlashFly)->Play(false, SE);
        //二元で斬撃方向と上方向で内積を取ってその分角度をとる
       // DirectX::XMFLOAT3 hitdir = GetSlashDir();
        DirectX::XMFLOAT3 n = { 0, 1, 0 };
        //float dot = Vector3::Dot(hitdir,n);
        //dot = acosf(dot);
        //if (hitdir.x < 0)dot *= -1;//方法が左なら角度反転
        ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
        projectile->Launch(slash, height * 0.5f, 4.0f, 0.2f, Type::Straight, (int)EffectTexAll::EfTexAll::Metal, 2, 5, 0.5f);
        projectile->SetScale(slashScale);
        //projectile->SetDirectionUp({-attackDir.z,dot,attackDir.x});
        //if (combo == WeponComboMax[weponType]) {//コンボ最後追加斬撃＋１
        //    ProjectileStraite* projectile2 = new ProjectileStraite(&objectManager);
        //    projectile2->Launch(slash, height / 2, 4.0, angle.y, Type::Straight, (int)EffectTexAll::EfTexAll::Metal, 1, 3, 0.5);
        //    projectile2->SetScale(slashScale);
        //    //projectile2->SetDirectionUp({ attackDir.z,dot,-attackDir.x });
        //}
        slashCombo = combo;
    }
}
void Player::SwingInput() {
    //スイングSE
    if (swingSe) {
        AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Swing1)->Play(false, SE);
        swingSe = false;
    }
    if (attackInvincibleTime < 0.2) {//連続スイングSE
        if (Vector3::FrameInput(swingTimer)) {
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Swing1)->Stop();
            AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Swing1)->Play(false, SE);
        }
        swingTimer += swingSpeed;
    }
}

void Player::ChangeWepon() {
    //if (weponType == weponChangeType && wepon->GetDisslve() == 1) {//武器が描画され最低限武器が表現されたら
    //    return;
    //}
    //if (timer % weponChangeTimer == 0) {
    //    float hi = 0;
    //    if (weponChangeType == WeponType::Shield)hi = 1;// 盾補正値
    //    ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
    //    projectile->Launch(wepon->GetWeponModel(weponChangeType), hi, 1.0, angle.y, Type::ChangeWp, (int)EffectTexAll::EfTexAll::Thunder, 0.3, 0, 0.1);
    //    weponChangeType++;//出現武器切り替え
    //}
    //if (weponChangeType > 4) weponChangeType = 0;
}
void Player::ButtonWeponChange(int type) {
    //if (!wepon->GetBreakFlag(type) && weponType != type) {
    //    wepon->SetRnderflag(false);
    //    skillCoolTime[WeponChange] = skillCoolTimeDefault[WeponChange];
    //    skillTime[WeponChange] = 0.3f;//継続時間
    //    nextWepon = type;
    //    weponChangeType = weponType;
    //    weponChange = true;
    //    if (state == State::Idle) {
    //        player->PlayAnimation(AnimWeponChange, false, 0.2);
    //    }
    //    ParticleSprite* particleSprite = new ParticleSprite(wepon->GetPosition(), wepon->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Thunder), 5000, 2.0);
    //    particleSprite = new ParticleSprite(wepon->GetWeaponPPoint(), wepon->GetWeaponPoint(), ParticleSprite::ParticleSoft, ParticleSprite::WeponConvergence, int(EffectTexAll::EfTexAll::Thunder), 5000, 2, 5);
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Change)->Stop();
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Change)->Play(false, SE);
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Break)->Stop();
    //    AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Break)->Play(false, SE);
    //}
}
void Player::SetShakeInput(DirectX::XMFLOAT3 dir,float damage) {
    //カメラシェイク情報入力
    cameraController->SetHitPow(damage);
    if(lockOn)cameraController->SetHitPow(damage*1.5f);
    cameraController->SetHitTimer(damage*0.5f);
    cameraController->SetHitDir(dir);
}
void Player::HitInput(float damage, float invincibleTime) {
    //if (wepon->GetSwordState() == SwordState::Shift) {//武器の状態がシフトなら
    //    teleportPos = wepon->GetPosition();//テレポートポイント設定
    //    shiftAttackflag = true;
    //    stateMachine->ChangeSubState(static_cast<int>(Player::State::Teleport));//テレポートステート
    //    Afterimage* projectile = new Afterimage(&objectManager);//残像エフェクト
    //    projectile->Launch(renderdata);
    //    if (wepon->GetBroken()&&mp < mpMax) mp++;//武器の耐久値が二割以下ならMP回復
    //
    //}
    attackHitflag = true;
    //wepon->Exhaustion(damage);//消耗
    hitInvincibleTime = damage*0.05f;
    //攻撃方向にカメラシェイク
    //DirectX::XMFLOAT3 hitdir = Vector3::Subset(swordTrail->GetTrail(swordFlame), wepon->GetWeaponEFPoint());
    //hitdir = Vector3::Normalize(hitdir);
    SetShakeInput({0,1,0}, damage * shakePow);
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
        //距離で判定
        if (enemyLength < lenght) {
            if (enemyLength < dist) {
                dist = enemyLength;
                returnEnemy = enemy;
            }
        }
    }
    return returnEnemy;
}
//剣の軌跡
void Player::SwordEffect() {
    head = SearchNodePos(attackNode[combo]);  //軌跡の後
    tail = SearchNodePos(attackENode[combo]); //軌跡の先
    swordTrail->SetPos(head, tail);   //軌跡の情報入力
}
//ジャンプ入力処理
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
// 攻撃入力処理
bool Player::InputAttack()
{
    //if(wepon->GetBreakFlag(weponType))return false;
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        return true;
    }
    return false;
}
//弾入力処理
void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // 直進弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_X && mp > swordMp && skillCoolTime[ProjectileColumn] <= 0.0f)
    {
        skillCoolTime[ProjectileColumn] = skillCoolTimeDefault[ProjectileColumn];
       // mp-= swordMp;
    		// 前方向   		
        float h = 0;
    		// 発射
            for (int i = 1; i < 5; i++) {
                h = 0.4f*i;
                ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
                projectile->Launch(beem,h,2.5f - 0.5f * i, angle.y, Type::Beem, (int)EffectTexAll::EfTexAll::BlueThader, 2 + 0.1f * i,1,0.0f);
            }
            for (int i = 1; i < 5; i++) {
                h = 0.4f * i;
                ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
                projectile->Launch(beem, h,2.5f -0.5f * i, angle.y, Type::Beem, (int)EffectTexAll::EfTexAll::BlueThader,2 + 0.1f* i, 1, 0.0f,true);
            }
    }
    //if (gamePad.GetButtonDown() & GamePad::BTN_B && mp > swordMp && skillCoolTime[ProjectileRotate] <= 0.0f)
    //{
    //    skillCoolTime[ProjectileRotate] = skillCoolTimeDefault[ProjectileRotate];
    //    mp-= swordMp;
    //    DirectX::XMFLOAT3 pos;
    //    for (int i = 0; i < 16; i++) {
    //        // 前方向
    //        DirectX::XMFLOAT3 dir;
    //        // 発射
    //        ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
    //        projectile->Launch(sword,height,0.8, angle.y+0.4 * i, Type::Rotate, (int)EffectTexAll::EfTexAll::Thunder,3,1,0.5f);
    //    }   
    //}
    if (gamePad.GetButtonDown() & GamePad::BTN_B && mp > swordMp && skillCoolTime[ProjectileRotate] <= 0.0f)
    {
        skillCoolTime[ProjectileRotate] = skillCoolTimeDefault[ProjectileRotate];
        mp -= swordMp;
        int rnd = 0;
        for (int i = 0; i < 30; i++) {
            rnd = rand() % 4;
            // 発射
            ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
            //projectile->Launch(wepon->GetWeponModel(rnd), height, 0.8, angle.y + 0.4 * i, Type::Sword, (int)EffectTexAll::EfTexAll::Thunder, 6, 1, 0.5f);
        }
    }
}
// 移動入力処理
bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //移動処理
    MoveInput(moveVec.x, moveVec.z, moveSpeed);
    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}
//ステック入力処理
bool Player::InputStick(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}
//注視移動
bool Player::InputBattleMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //移動処理
    MoveInput(moveVec.x, moveVec.z, moveSpeed * battelMove);
    //旋回処理
    Turn(elapsedTime, dir.x, dir.z, turnSpeed);
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}
//スティック入力から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //カメラ方向とスティックの入力値のよって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX + cameraRightZ * cameraRightX + cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;

    }
    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX + cameraFrontZ * cameraFrontX + cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {   //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
    //スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;
    return vec;
}
void Player::UpdateSkill(float elapsedTime)
{
    // 加速
    SkillQuick(elapsedTime);
    //武器チェンジ
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
    // 透過フラグが立ってなかったらリターン
    if (!weponChange)return;
    if (skillTime[WeponChange] < 0) {
        weponType = nextWepon;//次の武器を設定
        //wepon->SetWeponType(weponType);
        if (InputMove(elapsedTime)) {
            //wepon->SetRnderflag(true);
            stateMachine->ChangeSubState(static_cast<int>(Player::State::Move));
        }
        else
        {
            //wepon->SetRnderflag(true);
            stateMachine->ChangeSubState(static_cast<int>(Player::State::Idle));
        }
        weponChange = false;
    }
    
    skillTime[WeponChange] -= elapsedTime;
    ChangeWepon();
}
void Player::render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
    if (renderflag)
    {
        shader->Draw(immediate_context.Get(), player.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { 3.0f, 0.0f, 0.0f, 1.0f });
    }
    //wepon->Render(immediate_context.Get(), shader);
}
void Player::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader) {

    objectManager.Render(immediate_context.Get(),shader);

}
void Player::SwordTrailrender(RenderContext& rc, SwordTrailShader* shader) {
    
    shader->Draw(rc, swordTrail.get(), {1,1,1,1});

    objectManager.TrailRender(rc, shader);
    
}
//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    cameraController->DrawDebugGUI();
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        ImGui::InputInt("combo", &combo);
        ImGui::InputFloat("farPosition", &farPosition);
        ImGui::InputFloat("animeTimer", &animeTimer);
        ImGui::InputFloat("attackStart", &attackStart);
        ImGui::InputFloat("attackEnd", &attackEnd);
        ImGui::InputInt("clearState", &clearState);
        ImGui::SliderFloat("nextStateTimer", &nextStateTimer, 0.0f, 5.0f);
        if (ImGui::Button(u8"Start"))
        {
            // �z���O�����V�F�[�_�[��񏉊���
            HologramShaderDataInitialize(minHeight, maxHeight);
        }
        if (ImGui::TreeNode("PBR"))
        {
            ImGui::SliderFloat("adjustMetalness", &adjustMetalness, -1.0f, 1.0f);
            ImGui::SliderFloat("adjustSmoothness", &adjustSmoothness, -1.0f, 1.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Hologram"))
        {
#if 1
            ImGui::SliderFloat("scanBorder", &scanBorder, -200.0f, 0.0f);
            ImGui::SliderFloat("glowBorder", &glowBorder, -200.0f, 0.0f);
            ImGui::SliderFloat("hologramBorder", &hologramBorder, -200.0f, 0.0f);
#else
            ImGui::SliderFloat("scanBorder", &scanBorder, -20.0f, 0.0f);
            ImGui::SliderFloat("glowBorder", &glowBorder, -20.0f, 0.0f);
            ImGui::SliderFloat("hologramBorder", &hologramBorder, -20.0f, 0.0f);
#endif
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Glitch"))
        {
            ImGui::SliderFloat("glitchIntensity", &glitchIntensity, 0.0f, 1.0f);
            ImGui::SliderFloat("glitchScale", &glitchScale, 1.0f, 50.0f);
            ImGui::TreePop();
        }
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat3("Position", &position.x);
            int i = GetState();
            ImGui::InputInt("stat", &i);
            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &angle.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //スケール
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

    //衝突判定用のデバック球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1, 1, 1, 1));
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));
    //debugRenderer->DrawSphere(wepon->GetWeaponEPoint(), 0.2, DirectX::XMFLOAT4(0, 1, 0, 1));

    Animation::Keyframe::node* leftHandBone = player->FindNode("Wp_Sword");
    //leftHandBone->globalTransform * transform;
    //if (leftHandBone) {
    //    XMStoreFloat4x4(&world, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));
    //    if (attackCollisionFlag)
    //        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
    //            world._41 + world._31 * swordLength,
    //            world._42 + world._32 * swordLength,
    //            world._43 + world._33 * swordLength),
    //            wepon->GetRadius()*weponRadius,
    //            DirectX::XMFLOAT4(1, 0, 0, 1)
    //        );
    //}
    //debugRenderer->DrawSphere(wepon->GetWeaponPoint(),
    //    weponRadius,
    //    DirectX::XMFLOAT4(1, 0, 0, 1));
}

//プレイヤーと敵との衝突処理
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyhitflag = false;
    //全ての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++) {
        Enemy* enemy = enemyManager.GetEnemy(i);
        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (enemy->GetEnemyKeyframe().nodes.size() > 0) {//当たり判定用のノードがあれば
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
                             //	 押し出し後の位置設定
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
                    //	 押し出し後の位置設定
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
    //指定のノードと全ての敵を総当たりで衝突処理
    EnemyManager& enemyManager = EnemyManager::Instance();
    DirectX::XMFLOAT3 node = SearchNodePos(attackNode[combo]);
    int enemyCount = enemyManager.GetEnemyCount();
    for (int enemyindex = 0; enemyindex < enemyCount; enemyindex++)
    {
        Enemy* enemy = enemyManager.GetEnemy(enemyindex);
        DirectX::XMFLOAT3 enemyPosition, impulse;
            enemyPosition = { enemy->GetPosition().x,enemy->GetPosition().y,enemy->GetPosition().z };
            if (Collision::IntersectSphereVsSphere(
                node,
                nodeRadius,
                enemyPosition,
                enemy->GetRadius(),impulse))
            {
                if (enemy->ApplyDamage(static_cast<int>(Damage), InvincibleTime)) {
                    //ノックバック
                    KnockBack(*enemy, this->position, enemy->GetPosition(), pow.x, pow.y);
                    //ヒット情報入力処理
                    HitInput(Damage, InvincibleTime);
                    //DirectX::XMFLOAT3 trailDir = GetSlashDir();
                    //ヒットエフェクト
                    ParticleSprite* particleSprite = new ParticleSprite(node, {0,0,0}, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Sumi),500, 0.4f, 0, true, 0.002f,0.03f, { 1,1,1,1 });
                    particleSprite = new ParticleSprite(node, {0,0,0}, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, int(EffectTexAll::EfTexAll::Impact), 1, 0.3f, 0.1f);
                    if (enemy->GetBarrierFlag()) {
                        //particleSprite = new ParticleSprite(enemyPosition, wepon->GetWeaponEFPoint(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 1000, 0.5);
                        particleSprite = new ParticleSprite(enemyPosition, {NULL,NULL,NULL}, ParticleSprite::ParticleTriangle, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Distortion), 1000, 0.5,1);
                    }
                }
            }
        
    }
}

void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //全ての弾丸とすべての敵を総当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = objectManager.GetObjectCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        Object* object = objectManager.GetObjectW(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);
            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                object->GetPosition(),
                object->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                // ダメージを与える
                
                  if (enemy->ApplyDamage(object->GetDamage(),object->GetInvincibleTime()))
                  {
                      
                      KnockBack(*enemy, this->position, enemy->GetPosition(), 1, 0);
                    
                      SetShakeInput({ NULL,1,NULL }, static_cast<float>(object->GetDamage()));//カメラシェイク
                      if (object->GetType() == Type::Straight) {
                          //ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::Sumi), 2, { 1,1,1,1 });
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Stop();
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Play(false, SE);
                          //object->Destroy();
                          XMFLOAT3 start = Vector3::PosDir(object->GetPosition(), object->GetDirectionUp(), object->GetScale().x*2);
                          XMFLOAT3 end = Vector3::PosDir(object->GetPosition(), object->GetDirectionUp(), -object->GetScale().x*2);
                          ParticleSprite* particleSprite = new ParticleSprite(start, end, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Sumi), 2000, 1.5f, 0, true, 0.005f,0.06f, { 1,1,1,1 });
                          SetShakeInput({ 0,1,0 }, 4);
                          continue;
                      }
                      if (object->GetType() == Type::Beem) {
                          ParticleSystem::Instance().BoomEffect(object->GetPosition(), 5, int(EffectTexAll::EfTexAll::BlueThader), 3, 0.5,{ NULL,NULL,2,1 });
                         // ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), object->GetPosition(), ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 1000, 1.5);
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Stop();
                          AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::boom1)->Play(false, SE);
                          ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 200, 1.5f, 0, true, 0.015f,0.05f, {0,0,1,1});
                          SetShakeInput({0,1,0}, 2);
                          //object->Destroy();
                          continue;
                      }
                      //ランダム斬撃エフェクト
                      DirectX::XMFLOAT3 trailDir;
                      float angle = DirectX::XMConvertToRadians(static_cast<float>(rand() % 360));
                      trailDir.x = sinf(angle);
                      trailDir.y = cosf(angle);
                      trailDir.z = 0;
                      //EffectAll::Instance().hitEffect->Play(object->GetPosition(), 0.1);
                      ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), trailDir, ParticleSprite::ParticleLine, ParticleSprite::Slash, int(EffectTexAll::EfTexAll::Distortion), 1, 0.5);
                      AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash1)->Stop();
                      AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash1)->Play(false, SE);                                           
                  }                            
            }
        }
    }
}

void Player::CollisionBoomVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    ParticleManager& particleManager = ParticleManager::Instance();
    //全ての弾丸とすべての敵を総当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = particleManager.GetParticleCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        if (particleManager.GetParticle(i)->GetType() != ParticleType::Boom) continue;
        Particle* object = particleManager.GetParticle(i);
        for (Particle::MoveConstants& move : object->GetMove())
        {
            for (int j = 0; j < enemyCount; ++j)
            {
                Enemy* enemy = enemyManager.GetEnemy(j);
                //衝突処理
                DirectX::XMFLOAT3 outPosition;
                if (Collision::IntersectSphereVsCylinder(
                    move.position,
                    object->GetScale().x,
                    enemy->GetPosition(),
                    enemy->GetRadius(),
                    enemy->GetHeight(),
                    outPosition))
                {
                    // ダメージを与える
                    enemy->ApplyDamage(1, 0.5f);
                }
            }
        }
    }
}
void Player::Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader) {
    //UI系描画
    //wepon->DrawUI(dc,rc,shader);
}
void Player::UpdateUI(float elapsedTime) {
    hpTimer += 2;
    uiHp->SetGauge(health);
    //uiMp->SetGauge(mp);
}
