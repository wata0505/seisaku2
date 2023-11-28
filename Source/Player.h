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
        Idle,              //�ҋ@
        Move,              //�ړ�
        JumpStandby,       //�W�����v����
        Jump,              //�W�����v
        Land,              //���n
        Roll,              //���
        Stinger,           //��������
        Shift,             //�V�t�g
        ShiftAttack,       //�V�t�g�U��
        Lance,             //�n���o�[�g����
        Charge,            //�A�b�N�X����`���[�W
        ChargeAttack,      //�`���[�W�A�^�b�N
        Attack,            //���킲�Ƃ̍U��
        FallAttack,        //�����U��
        Guard,             //�K�[�h
        CounterStandby,    //�J�E���^�[�X�^���o�C
        Counter,           //�J�E���^�[
        Teleport,          //�e���|�[�g
        Damage,            //�_���[�W
        Down,              //�_�E��
        Dead               //���S
    };
    enum Animations
    {
        AnimIdle,
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
public:
    Player();
    ~Player() override;

    static Player& Instance();

    // �}�e���A���J���[
    DirectX::XMFLOAT4 materialColor{ 1 ,1, 1, 1 };



    void update(float elapsed_time);
    void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader);
    //�c���G�t�F�N�g�`��
    void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader);
    //�\�[�h�c���G�t�F�N�g�`��
    void SwordTrailrender(RenderContext& rc, SwordTrailShader* shader);

    //�W�����v���͏���
    bool InputJump();

    // �U�����͏���
    bool InputAttack();
   
    //�e���ˏ���
    void InputProjectile();

    void DrawDebugPrimitive();

    Model* GetModel() { return player.get(); }

    std::shared_ptr<Model> GetSword() { return sword; }

    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();
    //���b�N�I��
    void Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);

    bool GetQuickflag() { return quickFlag; };

    bool GetGuardflag() { return guardFlag; };

    bool GetCounterflag() { return counterFlag; };

    State GetState() { return state; };
    
    void SetState(State index) { state = index; };

    void SetCounterflag(bool Counter) { counterFlag = Counter; };
    //���b�N�I���t���O�擾
    bool GetLockOnflag() { return lockOn; };

    void PlayerKnockBack(DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY,float damage);
    //���b�N�I���G�l�~�[�ʒu�擾
    DirectX::XMFLOAT3 GetLockEnemyPos() { return epos; };
    //���̎茳�擾
    DirectX::XMFLOAT3 GetHeadPos() { return head; };
    
    //���̎茳�擾
    DirectX::XMFLOAT3 GetShieldHeadPos() { return  shieldHead; };
    //����擾
    DirectX::XMFLOAT3 GetTailPos() { return tail; };
    //�����擾
    DirectX::XMFLOAT3 GetPatternPos() { return pattern; };
    //���̌����擾
    DirectX::XMFLOAT3 GetSworddir() { return sworddir; };

    DirectX::XMFLOAT3 GetDir() { return dir; };
    //�U������
    DirectX::XMFLOAT3 GetAttackDir() { return attackDir; };
    //����̃g�����X�t�H�[��
    DirectX::XMFLOAT4X4 GetWeponTransform() { return weponTransform; }

    bool GetAttackHitflag() { return attackHitflag; }
    //����؂�ւ���Ԏ擾
    bool GetWeponChange() { return weponChange; }
    //�g���Ă镐�킪���Ă��邩
    //bool GetUseWeponBreak() { return wepon->GetBreakFlag(weponType); }
    //�`�悵�Ă��邩
    bool GetRenderflag() { return renderflag; }
    //�V�t�g�U�����Ă��邩
    bool GetShiftAttackflag() { return shiftAttackflag; }
    //�J�����V�F�C�N����
    void SetShakeInput(DirectX::XMFLOAT3 dir, float Damage);
    //���S�^�C�}�[
    float GetDidTimer() { return disslveTimer; }
    //�v���C���[�p�̓����蔻��ʒu
    DirectX::XMFLOAT3 GetHitPos() { return { position.x,(position.y - (height / 2)),position.z }; }
    //���`��t���O
    bool GetShildFlag() { return shildFlag; };
    //���b�N�I���G�l�~�[I��
    int GetLockEnemyId() { return enemyId; }
    //�G�Ƃ̋���
    float GetEnemyLength() { return enemyLength; }
    //����擾
   // MainWepon* GetWepon() { return wepon.get(); };

    //�ړ����͏���
    bool InputMove(float elapsedTime);

    StateMachine<Player>* GetStateMachine() { return stateMachine.get(); }

    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(float nodeRadius, DirectX::XMFLOAT2 pow, float Damage, float InvincibleTime);

    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;

    //�U��������
    void SetAttackStatus(DirectX::XMFLOAT4 Status, DirectX::XMFLOAT4 Pow);
    //���̋O��
    void SwordEffect();
    //�ł��߂��G�̏��
    Enemy* CloseEnemy(float lenght);
    //�^�[�Q�b�g�̋����ƕ����擾
    void DirToLength(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3& dir, float& length);
    //�U�����
    void SwingInput();
    //��Ԏa������
    void SlashInput();
    //�X�e�b�N�����Ɍ���
    bool InputStick(float elapsedTime);

    //�퓬�s�����̓A�j���[�V�����X�V
    void BattleMoveUpdate();
    //���A���[�V�����A�j���[�V�����X�V
    void DownUpdate();
    //�������Ă��鎞����
    bool InputBattleMove(float elapsedTime);
    //����؂�ւ�
    void ChangeWepon();
private:
    //�v���C���[�ƃG�l�~�[�Ƃ̏Փ˔��菈��
    void CollisionPlayerVsEnemies();

    //�e�ƃG�l�~�[�̏Փˏ���
    void CollisionProjectilesVsEnemies();

    void CollisionBoomVsEnemies();

    //���b�N�I���J����
    void  ComeTerget(float elapsedTime);

    // �X�L������
    void UpdateSkill(float elapsedTime);
    //�����X�L��
    void SkillQuick(float elapsedTime);

    void UpdateQuick(float elapsedTime);
    //����`�F���W�X�L��
    void SkillWeponChange(float elapsedTime);
    //����`�F���W�X�V
    void UpdateWeponChange(float elapsedTime);
    //���͕���`�F���W
    void ButtonWeponChange(int type);
   
    //���b�N�I���`��
    void DrawLockOn(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);
    //UI�`��
    void DrawUI(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);
    //UI�X�V
    void UpdateUI(float elapsedTime);
    //���������
    void SetWepon();
    //�m�[�h����
    void SearchNode(DirectX::XMFLOAT4X4& Transform, const char* nodeName);
    //�m�[�h�̈ʒu
    DirectX::XMFLOAT3 SearchNodePos(const char* nodeName);
    
    //�q�b�g�t���O�X�V
    void UpdateHitflag(float elapsedTime);
    //SE�X�V
    void AudioUpdate();
    //�q�b�g�a�������擾
    //DirectX::XMFLOAT3 GetSlashDir();
    
    //�q�b�g������
    void HitInput(float damge,float invincibleTime);
    //����`����
    void weponRenderUpdate();
   
public:
    //����ȃA�N�V�������͍X�V
    void SpecialUpdate();

protected:
    //���n�������ɌĂ΂��
    void OnLanding() override;

    // ���S�������ɌĂ΂��
    void OnDead() override;

    // �_���[�W���󂯂����ɌĂ΂��
    void OnDamaged()override;
public://�ύX���擾�������ϐ�
    //���n�t���O
    bool  landflag = true;
    //�_�E���t���O
    bool  downflag = false;
    //��Ԏa���R���{
    int slashCombo = -1;
    //�R���{��
    int combo = 0;
    //�������
    bool rollflag = false;
    //�V�[���h�`�攻��
    bool shildFlag = true;
    //�O����
    DirectX::XMFLOAT3 dir;
    //���񑬓x
    float turnSpeed = DirectX::XMConvertToRadians(720);
    //����^�C�v
    int weponType = 0;
    //�W�����v�p���[
    float jumpSpeed = 30.0f;
    //�W�����v��
    int   jumpCount = 0;
    //�W�����v���E��
    int   jumpLimit = 2;

    //�������
    float rollPow = 25;
    //��𖳓G����
    float rollInvincibleTimer = 0.3;

    Effekseer::Handle stingerEffect;

    float quickFlag = false;

    float effectSpeed = 5.0f;//�G�t�F�N�g�X�s�[�h

    //�U���X�e�[�^�X
    //{
    DirectX::XMFLOAT3 attackDir = { 0,0,0 };
    //�U�����a
    float weponRadius = 0.5;

    //�R���{��s����
    bool comboflag = false;
    //�A�^�b�N����
    bool attackCollisionFlag = false;
    //�U������n�܂�
    float  attackStart = 0;
    //�U������I���
    float  attackEnd = 0;
    //�U���^�C�}�[
    float attackTime = 0;
    //�V�t�g�^�C�}�[
    float shiftTime = 0.8;
    //�U���_���[�W
    float attackDamage = 0;
    //�U�����G����
    float attackInvincibleTime = 0;
    //�n���o�[�g����^�C�}�[
    float lanceTimer = 2.0;
    //�n���o�[�g���ꓖ���蔻��
    float lanceRudius = 10.0;
    //�`���[�W�A�^�b�N�^�C�}�[
    float chargeAttackTimer = 0.55;
    //�`���[�W�A�^�b�N�����蔻��
    float chargeAttackRudius = 5.0;
    //�q�b�g�X�g�b�v����
    float hitInvincibleTime = 0;
    //�q�b�g�X�g�b�v��
    bool  attackHitflag = false;
    //�q�b�g�X�g�b�v�p���[
    float  attackHitPow = 0.0;
    //�G�l�~�[�ƐڐG���Ă��邩
    bool                enemyhitflag = false;

    //��Ԏa���傫��
    DirectX::XMFLOAT3 slashScale = { 0.6,0.6,0.6 };
    //�U��SE���邩
    bool swingSe = false;
    //�U��SE�^�C��
    int swingTimer = false;
    //�U��SE�X�s�[�h
    int swingSpeed = 3;
    //�A�^�b�N���[�u�p���[
    float attackMove = 0;
    //�A�^�b�N���[�u�p���[�{��
    //float attackMoveRate = 0;
    //�A�^�b�N�G�l�~�[ID
    int attackMoveID = 0;

    float animeTimer = 0;
    //�m�b�N�o�b�N�p���[
    DirectX::XMFLOAT2 knockBackpow;

    //}
   //����ȃA�N�V�����֌W
   //{
    float mp = 0;
    //���
    float mpMax = 20;
    //�e���|�[�g�֌W
    DirectX::XMFLOAT3 teleportPos;
    //�e���|�[�g����
    float teleportLength;
    //�e���|�[�g�O����
    DirectX::XMFLOAT3 telportDir;
    //�e���|�[�g�O����
    float telportSpeed = 40;
    //�`��t���O
    bool renderflag = true;
    //�X�L���֌W
    float skillCoolTime[SkillMax] = { 0,0,0,0 };	// �X�L�����Ƃ̃N�[���^�C��
    float skillCoolTimeDefault[SkillMax] = { 1,1,1,1 };// �X�L�����Ƃ̃N�[���^�C��(�Œ�)
    float skillTime[SkillMax];	// �X�L���p������

    float guardFlag = false;//�K�[�h�t���O
    float pGuardFlag = false;//�K�[�h�t���O
    float counterFlag = false;
    bool  shiftAttackflag = false;//�V�t�g�A�^�b�N�q�b�g
    //�K�[�h�O����
    DirectX::XMFLOAT2 guardDir = { 0,0 };

     //���b�N�I���ΏۂƂ̋���
    float enemyLength = 0;

    //���b�N�I�����Ă邩
    bool lockOn = false;

    //�G�t�F�N�g�֌W
    //{
    bool effectflag = false; //�G�t�F�N�g�t���O
    DirectX::XMFLOAT3 efDir; //�G�t�F�N�g����
    DirectX::XMFLOAT3 efPos; //�G�t�F�N�g�ʒu

    //�f�B�]���u�^�C�}�[
    float disslveTimer = 3;

    //�`���[�W�J�E���^�[
    int chargeCount = 0;
    //�`���[�W�{��
    int chargeDamage = 2;
    //�`���[�W���
    int chargeMax = 3;

    //���킲�Ƃ̃A�j���[�V����
    int WeponCombo[4] = { Anim1Combo1,Anim1Combo2,Anim1Combo3,Anim1Combo4 };
    //����̃A�N�V�����X�e�[�^�X
    DirectX::XMFLOAT4 WeponComboStatus[WeponType::WeponMax][4] = {
        {{3, 0, 0.1f, 0.4f},{3, 0, 0.1f, 0.4f},{1, 0, 0.2f, 0.8f},{0.2, 0, 0.1f, 0.8f},},
        {{0.6, 0, 0.5f,0.8f},{0.8, 0, 0.9f, 1.32f},{0.8, 0, 1.1f, 1.5f},{ 0,  0,0.0f, 0.0f},},
        {{1, 0, 0.1f, 1.3f},{1, 0, 0.2f, 1.2f},{0.5, 0, 0.5f, 2.5f},{ 0,  0,0.0f, 0.0f},},
        {{1, 0, 0.4f, 0.75f},{1, 0, 0.75f, 1.05f},{1, 0, 1.08f, 1.38f},{ 0,  0,0.0f, 0.0f},},
        {{ 0, 0, 0.0f, 0.0f},{ 0, 0, 0.0f, 0.0f},{10, 0, 0.0f, 0.0f},{ 0,  0,0.0f, 0.0f},},
    };
    //���킲�ƃA�N�V�����p���[
    DirectX::XMFLOAT4 WeponComboPow[WeponType::WeponMax][4] = {
        {{10, 0,1,0.3},{10, 0,1,0.5},{10, 0,1,0.2},{20, 0,1,0.1},},
        {{40, 0,4,0.5},{50, 0,5,0.5},{70, 0,6,0.5},{20, 0,1,0.5},},
        {{10, 0,2,0.3},{ 0, 0,1,0.2},{10, 0,2,0.3},{20, 0,1,0.5},},
        {{20,30,2,0.5},{10,30,2,0.5},{30, 0,2,0.5},{20, 0,1,0.5},},
        {{10, 0,0,0.5},{10, 0,1,0.5},{10, 0,1,0.5},{20, 0,1,0.5},},

    };
   
    //���킲�Ƃ̃R���{�ő吔
    int WeponComboMax[5] = { 3,2,2,2,0 };

    DirectX::XMFLOAT4 ShifPow = { 0, 0,1,0.3 };

    DirectX::XMFLOAT4 FallAttackStatus = { -20, 0,0.3,0.9 };
    DirectX::XMFLOAT4 FallAttackPow = { 10, 0.1,1,0.9 };

    //�U���A�j���[�V�����m�[�h�i�[
    const char* attackNode[4] = { "te_R_1" ,"te_L_1","ashikubi_R","te_R_1" };

    //�e�֌W
    ObjectManager objectManager;

    std::vector<SkinnedMeshResouurce::constants> renderdata;
private:
    //���f��
    std::unique_ptr<Model> player;
    std::shared_ptr<Model> sword;
    std::shared_ptr<Model> slash;
    std::shared_ptr<Model> beem;
    //�����g�m�[�h
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
    //UI�֌W
    //HP�^�C�}�[
    float hpTimer = 0;
    //�w���X��
    float healthrate = 1;
    //UI�X�V���x
    float uiSpeed = 0.5;
    //MP��
    float mpthrate = 1;
    
    //����֌W
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
    //��������
    DirectX::XMFLOAT3 shieldHead;
    //��������
    DirectX::XMFLOAT3 head;
    //����
    DirectX::XMFLOAT3 tail;
    //���̕�
    DirectX::XMFLOAT3 pattern;
    //���̌���
    DirectX::XMFLOAT3 sworddir;

    //���̃��[�`
    float swordLength = 55;
    
    //����`�F���W�^�C�v
    int weponChangeType = 0;
    //���̕���^�C�v
    int nextWepon = 0;
    //���`�F���W�^�C�v
    float GuardChangeTimer = 0;
    //����`�F���W�^�C�}�[
    int weponChangeTimer = 5;
    //����`�F���W�t���O
    bool weponChange = false;  
    //{
    //�����֌W
    //}
    State          state = State::Idle;
    float moveSpeed = 8.0f;
  
    //�m�b�N�o�b�N�p���[�萔
    float kbpow = 15;

    //�J�[�h���m�b�N�o�b�N����
    float  guardKnockBack = 0.5;
    //��������͊��x
    float upSens = DirectX::XMConvertToRadians(15);
    //�΂ߕ������͊��x
    float disUpSens = DirectX::XMConvertToRadians(75);
    //�΂߉��������͊��x
    float disDownSens = DirectX::XMConvertToRadians(105);
    //���������͊��x
    float downSens = DirectX::XMConvertToRadians(165);
    //���A�p���[
    float returnPow = 0.5;
    //���[�V�����X�s�[�h
    float modelSpeed = 1.5f;
    //�����ړ��l
    float battelMove = 0.5;

    //}
    //�J�����֌W
    //{
    std::shared_ptr<CameraController> cameraController = nullptr;
 
    //���b�N�I���Ώ�
    int enemyLock = 0;
    //���b�N�I���Ώۂ�ID
    int enemyId = 0;
    //���b�N�I���Ώې؂�ւ�
    bool lockflag = false;
    //���b�N�I������
    bool locklength = false;
    //�n�߂Ẵ��b�N�I������
    bool fistlock = false;
    //���b�N�I��UI��]
    float lockonAngle = 0.0f;
    //���b�N�I���G�l�~�[�ʒu�ۑ�
    DirectX::XMFLOAT3 epos;
    //�J�����V�F�C�N�p���[
    float shakePow = 2;
   
    //�t���[���^�C�}�[
    int timer = 0;
    //�e����MP
    float swordMp = 4;
    
    float  efLength = 5;
    //���t���[���O�̌����W
    int  swordFlame = 0;
    //}
    //�R���{�X�e�[�^�X
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
    //�G�l�~�[�ƐڐG���Ă��邩
    bool                enemyflyflag = false;

    //�������
    float cameraLength = 0;
    //�X�e�[�g�}�V�[������
    std::shared_ptr<StateMachine<Player>> stateMachine = nullptr;

    
    std::vector<SkinnedMeshResouurce::constants> swordrenderdata;
};


