#pragma once
#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <d3d11.h>
#include <memory>
#include <DirectXMath.h>
#include "SkinnedMesh.h"
#include <functional>
#include "shader.h"
#include "Model.h"
#include "Character.h"
#include "DebugRenderer.h"
#include "CameraController.h"
#include "ObjectManager.h"
#include "SwordTrail.h"
#include "SwordTrailShader.h"
#include <wrl.h>
#include "EffectAll.h"
#include"MainWepon.h"
#include"GaugeUI.h"
#include"Enemy.h"
#include"StateMachine.h"


class Player : public Character
{
public:
    enum  State
    {
        Idle,              //待機
        Move,              //移動
        JumpStandby,       //ジャンプ準備
        Jump,              //ジャンプ
        Land,              //着地
        Roll,              //回避
        Stinger,           //長剣特殊
        Shift,             //シフト
        ShiftAttack,       //シフト攻撃
        Lance,             //ハルバート特殊
        Charge,            //アックス特殊チャージ
        ChargeAttack,      //チャージアタック
        Attack,            //武器ごとの攻撃
        FallAttack,        //落下攻撃
        Guard,             //ガード
        CounterStandby,    //カウンタースタンバイ
        Counter,           //カウンター
        Teleport,          //テレポート
        Damage,            //ダメージ
        Down,              //ダウン
        Dead               //死亡
    };
    enum Animations
    {
        AnimIdle,
        AnimWalking,
        AnimRunning,
        AnimJump1,
        AnimJump2,
        AnimJump3,
        AnimJumpAttack,
        AnimLound,
        Anim1Combo1,
        Anim1Combo2,
        Anim1Combo3,
        Anim1Combo4,
        AnimHit,
        AnimDie
    };
    
    enum AttackNo
    {
        Combo1,
        Combo2,
        Combo3,
        Combo4
    };
    enum FallAttackNo
    {
        FallMain,
        FallHB
    };
    enum UINo
    {
        HP,
        HPWAKU,
        MP,
        MPWAKU,
        LockonMark,
        Die,


        UIMax
    };
    enum Skill
    {
        Quick,
        WeponChange,
        ProjectileColumn,
        ProjectileRotate,
        SkillMax
    };
    enum TitleState
    {
        TitleDefault,            // ������
        TitleSelect,             // �^�C�g���ŃQ�[�����[�h�I��
        TitlePunchStart,         // ����A�j���[�V�����J�n
        TitlePunchNow,           // ����A�j���[�V������
        TitlePunchReverberation, // ����A�j���[�V�����]�C
        TitleKickStart,          // �R���Ƃ��A�j���[�V�����J�n
        TitleKickNow,            // �R���Ƃ��A�j���[�V������
        TitleKickReverberation,  // �R���Ƃ��A�j���[�V�����]�C
    };
public:
    Player();
    ~Player() override;

    static Player& Instance();

    // マテリアルカラー
    DirectX::XMFLOAT4 materialColor{ 1 ,1, 1, 1 };



    void update(float elapsed_time);
    void TitleUpdate(float elapsedTime);
    void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader);
    //残像エフェクト描画
    void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader);
    //ソード残像エフェクト描画
    void SwordTrailrender(RenderContext& rc, SwordTrailShader* shader);

    //ジャンプ入力処理
    bool InputJump();

    // 攻撃入力処理
    bool InputAttack();
   
    //弾発射処理
    void InputProjectile();

    void DrawDebugPrimitive();

    Model* GetModel() { return player.get(); }

    std::shared_ptr<Model> GetSword() { return sword; }

    //デバッグ用GUI描画
    void DrawDebugGUI();
    //ロックオン
    void Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);

    bool GetQuickflag() { return quickFlag; };

    bool GetGuardflag() { return guardFlag; };

    bool GetCounterflag() { return counterFlag; };

    State GetState() { return state; };
    
    void SetState(State index) { state = index; };

    void SetCounterflag(bool Counter) { counterFlag = Counter; };
    //ロックオンフラグ取得
    bool GetLockOnflag() { return lockOn; };

    void PlayerKnockBack(DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY,float damage);
    //ロックオンエネミー位置取得
    DirectX::XMFLOAT3 GetLockEnemyPos() { return epos; };
    //剣の手元取得
    DirectX::XMFLOAT3 GetHeadPos() { return head; };
    
    //盾の手元取得
    DirectX::XMFLOAT3 GetShieldHeadPos() { return  shieldHead; };
    //剣先取得
    DirectX::XMFLOAT3 GetTailPos() { return tail; };
    //剣柄取得
    DirectX::XMFLOAT3 GetPatternPos() { return pattern; };
    //剣の向き取得
    DirectX::XMFLOAT3 GetSworddir() { return sworddir; };

    DirectX::XMFLOAT3 GetDir() { return dir; };
    //攻撃方向
    DirectX::XMFLOAT3 GetAttackDir() { return attackDir; };
    //武器のトランスフォーム
    DirectX::XMFLOAT4X4 GetWeponTransform() { return weponTransform; }

    bool GetAttackHitflag() { return attackHitflag; }
    //武器切り替え状態取得
    bool GetWeponChange() { return weponChange; }
    //使ってる武器が壊れているか
    //bool GetUseWeponBreak() { return wepon->GetBreakFlag(weponType); }
    //描画しているか
    bool GetRenderflag() { return renderflag; }
    //シフト攻撃しているか
    bool GetShiftAttackflag() { return shiftAttackflag; }
    //カメラシェイク入力
    void SetShakeInput(DirectX::XMFLOAT3 dir, float Damage);
    //死亡タイマー
    float GetDidTimer() { return disslveTimer; }
    //プレイヤー用の当たり判定位置
    DirectX::XMFLOAT3 GetHitPos() { return { position.x,(position.y - (height / 2)),position.z }; }
    //盾描画フラグ
    bool GetShildFlag() { return shildFlag; };
    //ロックオンエネミーIｄ
    int GetLockEnemyId() { return enemyId; }
    //敵との距離
    float GetEnemyLength() { return enemyLength; }
    //武器取得
   // MainWepon* GetWepon() { return wepon.get(); };

    //移動入力処理
    bool InputMove(float elapsedTime);

    StateMachine<Player>* GetStateMachine() { return stateMachine.get(); }
    int GetTitleState() { return titleState; }
    void SetTitleState(int index) { titleState = index; }

    // ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(float nodeRadius, DirectX::XMFLOAT2 pow, float Damage, float InvincibleTime);

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    //攻撃情報入力
    void SetAttackStatus(DirectX::XMFLOAT4 Status, DirectX::XMFLOAT4 Pow);
    //剣の軌跡
    void SwordEffect();
    //最も近い敵の情報
    Enemy* CloseEnemy(float lenght);
    //ターゲットの距離と方向取得
    void DirToLength(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3& dir, float& length);
    //振る入力
    void SwingInput();
    //飛ぶ斬撃入力
    void SlashInput();
    //ステック方向に向く
    bool InputStick(float elapsedTime);

    //戦闘行動入力アニメーション更新
    void BattleMoveUpdate();
    //復帰モーションアニメーション更新
    void DownUpdate();
    //注視している時動き
    bool InputBattleMove(float elapsedTime);
    //武器切り替え
    void ChangeWepon();
private:
    //プレイヤーとエネミーとの衝突判定処理
    void CollisionPlayerVsEnemies();

    //弾とエネミーの衝突処理
    void CollisionProjectilesVsEnemies();

    void CollisionBoomVsEnemies();

    //ロックオンカメラ
    void  ComeTerget(float elapsedTime);

    // スキル効果
    void UpdateSkill(float elapsedTime);
    //加速スキル
    void SkillQuick(float elapsedTime);

    void UpdateQuick(float elapsedTime);
    //武器チェンジスキル
    void SkillWeponChange(float elapsedTime);
    //武器チェンジ更新
    void UpdateWeponChange(float elapsedTime);
    //入力武器チェンジ
    void ButtonWeponChange(int type);
   
    //ロックオン描画
    void DrawLockOn(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);
    //UI描画
    void DrawUI(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);
    //UI更新
    void UpdateUI(float elapsedTime);
    //武器情報入力
    void SetWepon();
    //ノード検索
    void SearchNode(DirectX::XMFLOAT4X4& Transform, const char* nodeName);
   
    
    //ヒットフラグ更新
    void UpdateHitflag(float elapsedTime);
    //SE更新
    void AudioUpdate();
    //ヒット斬撃方向取得
    //DirectX::XMFLOAT3 GetSlashDir();
    
    //ヒット情報入力
    void HitInput(float damge,float invincibleTime);
    //武器描画情報
    void weponRenderUpdate();
   
public:
    //特殊なアクション入力更新
    void SpecialUpdate();
    //ノードの位置
    DirectX::XMFLOAT3 SearchNodePos(const char* nodeName);

    int GetFrameTimer() { return frameTimer; }

protected:
    //着地した時に呼ばれる
    void OnLanding() override;

    // 死亡した時に呼ばれる
    void OnDead() override;

    // ダメージを受けた時に呼ばれる
    void OnDamaged()override;
public://変更又取得が多い変数
    //着地フラグ
    bool  landflag = true;
    //ダウンフラグ
    bool  downflag = false;
    //飛ぶ斬撃コンボ
    int slashCombo = -1;
    //コンボ数
    int combo = 0;
    //回避時間
    bool rollflag = false;
    //シールド描画判定
    bool shildFlag = true;
    //前方向
    DirectX::XMFLOAT3 dir;
    //旋回速度
    float turnSpeed = DirectX::XMConvertToRadians(720);
    //武器タイプ
    int weponType = 0;
    //ジャンプパワー
    float jumpSpeed = 30.0f;
    //ジャンプ回数
    int   jumpCount = 0;
    //ジャンプ限界数
    int   jumpLimit = 2;

    //回避距離
    float rollPow = 25;
    //回避無敵時間
    float rollInvincibleTimer = 0.3;

    Effekseer::Handle stingerEffect;

    float quickFlag = false;

    float effectSpeed = 5.0f;//エフェクトスピード

    //攻撃ステータス
    //{
    DirectX::XMFLOAT3 attackDir = { 0,0,0 };
    //攻撃半径
    float weponRadius = 0.5;

    //コンボ先行入力
    bool comboflag = false;
    //アタック判定
    bool attackCollisionFlag = false;
    //攻撃判定始まり
    float  attackStart = 0;
    //攻撃判定終わり
    float  attackEnd = 0;
    //攻撃タイマー
    float attackTime = 0;
    //シフトタイマー
    float shiftTime = 0.8;
    //攻撃ダメージ
    float attackDamage = 0;
    //攻撃無敵時間
    float attackInvincibleTime = 0;
    //ハルバート特殊タイマー
    float lanceTimer = 2.0;
    //ハルバート特殊当たり判定
    float lanceRudius = 10.0;
    //チャージアタックタイマー
    float chargeAttackTimer = 0.55;
    //チャージアタック当たり判定
    float chargeAttackRudius = 5.0;
    //ヒットストップ時間
    float hitInvincibleTime = 0;
    //ヒットストップ中
    bool  attackHitflag = false;
    //ヒットストップパワー
    float  attackHitPow = 0.0;
    //エネミーと接触しているか
    bool                enemyhitflag = false;

    //飛ぶ斬撃大きさ
    DirectX::XMFLOAT3 slashScale = { 0.6,0.6,0.6 };
    //振るSEするか
    bool swingSe = false;
    //振るSEタイム
    int swingTimer = false;
    //振るSEスピード
    int swingSpeed = 3;
    //アタックムーブパワー
    float attackMove = 0;
    //アタックムーブパワー倍率
    //float attackMoveRate = 0;
    //アタックエネミーID
    int attackMoveID = 0;

    float animeTimer = 0;
    //ノックバックパワー
    DirectX::XMFLOAT2 knockBackpow;

    //}
   //特殊なアクション関係
   //{
    float mp = 0;
    //上限
    float mpMax = 20;
    //テレポート関係
    DirectX::XMFLOAT3 teleportPos;
    //テレポート距離
    float teleportLength;
    //テレポート前方向
    DirectX::XMFLOAT3 telportDir;
    //テレポート前方向
    float telportSpeed = 40;
    //描画フラグ
    bool renderflag = true;
    //スキル関係
    float skillCoolTime[SkillMax] = { 0,0,0,0 };	// スキルごとのクールタイム
    float skillCoolTimeDefault[SkillMax] = { 1,1,1,1 };// スキルごとのクールタイム(固定)
    float skillTime[SkillMax];	// スキル継続時間

    float guardFlag = false;//ガードフラグ
    float pGuardFlag = false;//ガードフラグ
    float counterFlag = false;
    bool  shiftAttackflag = false;//シフトアタックヒット
    //ガード前方向
    DirectX::XMFLOAT2 guardDir = { 0,0 };

     //ロックオン対象との距離
    float enemyLength = 0;

    //ロックオンしてるか
    bool lockOn = false;

    //エフェクト関係
    //{
    bool effectflag = false; //エフェクトフラグ
    DirectX::XMFLOAT3 efDir; //エフェクト方向
    DirectX::XMFLOAT3 efPos; //エフェクト位置

    //ディゾルブタイマー
    float disslveTimer = 3;

    //チャージカウンター
    int chargeCount = 0;
    //チャージ倍率
    int chargeDamage = 2;
    //チャージ上限
    int chargeMax = 3;

    //武器ごとのアニメーション
    int WeponCombo[4] = { Anim1Combo1,Anim1Combo2,Anim1Combo3,Anim1Combo4 };
    //武器のアクションステータス
    DirectX::XMFLOAT4 WeponComboStatus[WeponType::WeponMax][4] = {
        {{0.5, 0, 0.1f, 0.4f},{0.5, 0, 0.1f, 0.4f},{1, 0, 0.2f, 0.8f},{0.1, 0, 0.2f, 0.8f},},
        {{0.6, 0, 0.5f,0.8f},{0.8, 0, 0.9f, 1.32f},{0.8, 0, 1.1f, 1.5f},{ 0,  0,0.0f, 0.0f},},
        {{1, 0, 0.1f, 1.3f},{1, 0, 0.2f, 1.2f},{0.5, 0, 0.5f, 2.5f},{ 0,  0,0.0f, 0.0f},},
        {{1, 0, 0.4f, 0.75f},{1, 0, 0.75f, 1.05f},{1, 0, 1.08f, 1.38f},{ 0,  0,0.0f, 0.0f},},
        {{ 0, 0, 0.0f, 0.0f},{ 0, 0, 0.0f, 0.0f},{10, 0, 0.0f, 0.0f},{ 0,  0,0.0f, 0.0f},},
    };
    //武器ごとアクションパワー
    DirectX::XMFLOAT4 WeponComboPow[WeponType::WeponMax][4] = {
        {{10, 0,2,0.3},{10, 0,2,0.5},{10, 0,1,0.2},{20, 0,5,0.5},},
        {{40, 0,4,0.5},{50, 0,5,0.5},{70, 0,6,0.5},{20, 0,1,0.5},},
        {{10, 0,2,0.3},{ 0, 0,1,0.2},{10, 0,2,0.3},{20, 0,1,0.5},},
        {{20,30,2,0.5},{10,30,2,0.5},{30, 0,2,0.5},{20, 0,1,0.5},},
        {{10, 0,0,0.5},{10, 0,1,0.5},{10, 0,1,0.5},{20, 0,1,0.5},},

    };
   
    //武器ごとのコンボ最大数
    int WeponComboMax[5] = { 3,2,2,2,0 };

    DirectX::XMFLOAT4 ShifPow = { 0, 0,1,0.3 };

    DirectX::XMFLOAT4 FallAttackStatus = { -30, 0,0.3,0.9 };
    DirectX::XMFLOAT4 FallAttackPow = { 10, 0.1,1,0.9 };

    //攻撃アニメーションノード格納
    const char* attackNode[4] = { "te_R_1" ,"te_L_1","ashikubi_R","te_R_1" };
    const char* attackENode[4] = { "hiji_R" ,"hiji_L","hiza_R","hiji_R" };

    //弾関係
    ObjectManager objectManager;

    std::vector<SkinnedMeshResouurce::constants> renderdata;

    float cameraRange = 5.0;

    const float cameraRangeMax = 5.0;

    float		correctionSpeed = 20;

    const float correctionSpeedMax = 20;
private:
    //モデル
    std::unique_ptr<Model> player;
    std::shared_ptr<Model> sword;
    std::shared_ptr<Model> slash;
    std::shared_ptr<Model> beem;
    //下半身ノード
    std::vector <const char*> lowerBody;
    const char* lower[13] = {
        {"root_x"},
        {"thigh_stretch_l"},
        {"leg_stretch_l"},
        {"foot_l"},
        {"toes_01_l"},
        {"leg_twist_l"},
        {"thigh_twist_l"},
        {"thigh_stretch_r"},
        {"leg_stretch_r"},
        {"foot_r"},
        {"toes_01_r"},
        {"leg_twist_r"},
        {"thigh_twist_r"}
    };
    //UI
    GaugeUI* uiHp;
    GaugeUI* uiMp;
    //UI関係
    //HPタイマー
    float hpTimer = 0;
    //ヘルス率
    float healthrate = 1;
    //UI更新速度
    float uiSpeed = 0.5;
    //MP率
    float mpthrate = 1;
    
    //武器関係
    std::shared_ptr<SwordTrail> swordTrail;
    //std::unique_ptr<MainWepon> wepon;
    
    DirectX::XMFLOAT4X4		world = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    DirectX::XMFLOAT4X4		weponTransform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    DirectX::XMFLOAT4X4		nodeTransform = {
       1,0,0,0,
       0,1,0,0,
       0,0,1,0,
       0,0,0,1
    };
    //盾持ち手
    DirectX::XMFLOAT3 shieldHead;
    //剣持ち手
    DirectX::XMFLOAT3 head;
    //剣先
    DirectX::XMFLOAT3 tail;
    //剣の柄
    DirectX::XMFLOAT3 pattern;
    //剣の向き
    DirectX::XMFLOAT3 sworddir;

    //剣のリーチ
    float swordLength = 55;
    
    //武器チェンジタイプ
    int weponChangeType = 0;
    //次の武器タイプ
    int nextWepon = 0;
    //盾チェンジタイプ
    float GuardChangeTimer = 0;
    //武器チェンジタイマー
    int weponChangeTimer = 5;
    //武器チェンジフラグ
    bool weponChange = false;  
    //{
    //動き関係
    //}
    State          state = State::Idle;
    float moveSpeed = 8.0f;
  
    //ノックバックパワー定数
    float kbpow = 15;

    //カード時ノックバック減衰
    float  guardKnockBack = 0.5;
    //上方向入力感度
    float upSens = DirectX::XMConvertToRadians(15);
    //斜め方向入力感度
    float disUpSens = DirectX::XMConvertToRadians(75);
    //斜め下方向入力感度
    float disDownSens = DirectX::XMConvertToRadians(105);
    //下方向入力感度
    float downSens = DirectX::XMConvertToRadians(165);
    //復帰パワー
    float returnPow = 0.5;
    //モーションスピード
    float modelSpeed = 1.5f;
    //注視移動値
    float battelMove = 0.5;

    //}
    //カメラ関係
    //{
    std::shared_ptr<CameraController> cameraController = nullptr;
 
    //ロックオン対象
    int enemyLock = 0;
    //ロックオン対象のID
    int enemyId = 0;
    //ロックオン対象切り替え
    bool lockflag = false;
    //ロックオン入力
    bool locklength = false;
    //始めてのロックオン入力
    bool fistlock = false;
    //ロックオンUI回転
    float lockonAngle = 0.0f;
    //ロックオンエネミー位置保存
    DirectX::XMFLOAT3 epos;
    //カメラシェイクパワー
    float shakePow = 2;
   
    //フレームタイマー
    int frameTimer = 0;
    //弾消耗MP
    float swordMp = 4;
    
    float  efLength = 5;
    //何フレーム前の剣座標
    int  swordFlame = 0;
    //}
    //コンボステータス
    //{
  
    DirectX::XMFLOAT4 JumpComboStatus[4] = {
        {0, 0, 0.1f, 0.3f},
        {0, 0, 0.2f, 0.6f},
        {0, 0, 0.1f, 0.8f},
        {-20, 0,0.9f, 2.0f}
    };
    
    DirectX::XMFLOAT4 JumpComboPow[4] = {
        {0, 0,1,0.5},
        {0, 0,1,0.5},
        {0, 0,1,0.5},
        {0, 0,1,0.5}
    };
    //エネミーと接触しているか
    bool                enemyflyflag = false;

    //回避距離
    float cameraLength = 0;
    //ステートマシーン生成
    std::shared_ptr<StateMachine<Player>> stateMachine = nullptr;

    
    std::vector<SkinnedMeshResouurce::constants> swordrenderdata;

    float adjustMetalness = 0.0f;    // ����x
    float adjustSmoothness = 0.0f;    // �e��
    float timer = 0.0f;                // �X�V����


    DirectX::XMFLOAT3 hologramColor = { 0.0f, 0.0f, 1.0f }; // �z���O�����F
    float glitchSpeed = 50.0f;        // �X�N���[�����x
    float glitchIntensity = 0.0f;    // ���x
    float lerpGlitchIntensity = 0.0f;    // ���x
    float glitchScale = 50.0f;        // �U�ꕝ    
    float deathTimer = 0.0f;        // ���S����
    int titleState = TitleState::TitleDefault;
    float nextStateTimer = 0.0f;
    float animationTimer = 0.0f;
};


