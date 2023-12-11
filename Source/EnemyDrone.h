#pragma once
#include "Model.h"
#include "Enemy.h"
#include "StateMachine.h"
#include "AudioSource.h"
#include "ObjectManager.h"
#define BagSE 0.01



class EnemyDrone :public Enemy
{
public:
	enum  EnemyDroneSE
	{
		Walk,
		Run,
		Roar,
		Max
	};
	// エネミーアニメーション
	enum  EnemyDroneAnimation
	{
		Idle,
	};
	//エネミーターゲット
	enum  EnemyTarget
	{
		BaseTarget,
		TrapTarget,
	};
public:
	EnemyDrone(bool tutorial = false);
	~EnemyDrone()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)override;
	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;


	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);
	//プレイヤーを中心に回転
	void BattleMove(bool leftflag, float elapsedTime, float speedRate);
	//プレイヤーへの方向に向く
	void FacePlayer(float elapsedTime, float speedRate);

	//トラップ索敵
	bool SearchTrap();

	//ファイルボールショット
	void BeemShoat();

	void TargetUpdate();

	// プレイヤーへの方向取得
	DirectX::XMFLOAT2 ForwardToPlayer();

	DirectX::XMFLOAT2 ForwardToTarget();

	// TODO 05_02 メッセージ受信関数を追加
	bool OnMessage(const Telegram& msg);

	void DrawDebugGUI() override;


	//// ターゲットポジション設定
	//void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }
	//
	//// ターゲットポジション取得
	//DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	// ポジション取得
	DirectX::XMFLOAT3 GetPosition() { return position; }



	// ステートタイマー設定
	void SetStateTimer(float timer) {
		stateTimer = timer;
	}
	// ステートタイマー取得
	float GetStateTimer() { return stateTimer; }
	//バトル待機半径取得
	float GetBattleRange() { return battleRange; }
	//バトル待機半径セット
	void  SetBattleRange(float BattleRange) { battleRange = BattleRange; }
	//プレイヤーとの距離をランダムで決定
	void  RandBattleRange() { battleRange = rand() % battleRangeMax + 1.0f; }

	StateMachine<EnemyDrone>* GetStateMachine() { return stateMachine.get(); }

	Model* GetModel() { return model.get(); }



	float GetMoveRate() { return moveRate; }

	void SetMoveRate(float rate) { moveRate = rate; }

	void SetDamegeRadius(float radius) { damegeRadius = radius; }

	float GetDamegeRadius() { return damegeRadius; }
	//遠距離攻撃権取得
	bool GetAttackFireBallFlg() { return attackFireBallFlg; }
	//遠距離攻撃権設定
	void SetAttackFireBallFlg(bool attackFireBallFlg) { this->attackFireBallFlg = attackFireBallFlg; }

	float GetBackStepRange() { return backStepRange; }

	int GetFlameTimer() { return enemyTimer / 60; }


	void RootMove();

	float  MovePow();


	void PlaySe(int Index, bool loop) { se[Index]->Play(loop, static_cast<float>(BagSE)); }
	void StopSe(int Index) { se[Index]->Stop(); }
	//チュートリアルフラグ取得
	bool GetTutorialflag() { return tutorialflag; }

	DirectX::XMFLOAT3 SearchNodePos(const char* nodeName);

	void ReMove();

	int GetRootNo() { return rootNo; }
	int GetMaxRootNo() { return MAX_ROOT_POINT; }

	void SetRootNo(int no) { rootNo = no; }
private:
	void SeUpdate(float elapsedTime);

	void CollisionBeemVSTrap();
	void CollisionBeemVSBase();

protected:
	void OnDead();

	void OnDamaged();

	//void UpdateVerticalMove(float elapsedTime);
	//private:
public:
	// TODO 01 必要なステートをenum classで準備しなさい。
	// （Wanderのみ記述済み）
	enum class DroneState
	{
		Search,   //索敵
		Battle,	  //戦闘
		Recieve,  //呼び寄せ
		ReDamage, //ダメージ受けた時
	};
	enum class Search
	{
		// TODO 03_01 SearchStateに含まれるサブステートを記述しなさい
		Wander,  //徘徊
		Idle,	 //待機
	};
	enum class Battle
	{
		// TODO 03_01 BattleStateに含まれるサブステートを記述しなさい
		Pursuit,   //追尾
		Attack,    //攻撃
		Standby,   //攻撃待機
	};
	// TODO 05_02 MetaAIを経由して他のエネミーから呼ばれた時の処理
	enum class Recieve
	{
		Called,   //呼び寄せ
	};
	enum class ReDamage
	{
		Damege,  //ダメージ受け
		Die,	 //死亡
	};
private:
	std::shared_ptr<Model> beem;
	std::shared_ptr<Model> model = nullptr;

	static const int MAX_ROOT_POINT{ 4 };

	int rootNo = 0;
	DirectX::XMFLOAT3 rootPoint[MAX_ROOT_POINT] = { {129,-2.5,41}, {128,-2.5,-8.5} ,{78,-2.5,-5} ,{80,-2.5,-45} };

	//遠距離攻撃権
	bool attackFireBallFlg = false;
	float				moveSpeed = 4.5f;
	//歩く速度
	float				moveRate = 3.0f;
	//バック速度
	float			    workRate = 0.5f;
	//走る速度
	float			    runRate = 1.5f;
	//チュートリアル用
	bool                tutorialflag = false;
	float				turnSpeed = DirectX::XMConvertToRadians(360);
	float				stateTimer = 0.0f;
	//バトルステート範囲
	float				battleRange = 6.0f;
	//バトルステート範囲MAX
	int				battleRangeMax = 6.0f;
	//バックステップ範囲
	float				backStepRange = 2.0f;
	//当たり判定半径
	float               damegeRadius = 1.0f;
	//回転スピード
	float               rootSpeed = 0.8f;
	//ヒットストップ
	float               hitStop = 0.0;
	//バリアHP
	int                 barrierHP = 5;
	DirectX::XMFLOAT4X4 world = {};
	//ターゲットNo
	int targetNo = 0;
	//ヒットアニメーション最大
	static const int MAX_HIT_ANIM{ 3 };
	//ヒットアニメーション格納
	//int hitAnim[MAX_HIT_ANIM] = { static_cast<int>(EnemyDroneAnimation::Gethit2) ,static_cast<int>(EnemyDroneAnimation::Gethit3) ,static_cast<int>(EnemyDroneAnimation::Gethit4) };

	//ステートマシーン生成
	std::shared_ptr<StateMachine<EnemyDrone>> stateMachine = nullptr;
	//描画情報格納
	std::vector<SkinnedMeshResouurce::constants> renderdata;

	std::unique_ptr<AudioSource> se[(int)EnemyDroneSE::Max];
	//ファイルボールマネージャー
	ObjectManager objectManager;

	float adjustMetalness = 0.0f;	// 金属度
	float adjustSmoothness = 0.0f;	// 粗さ
	float emissiveStrength = 1.0f;	// エミッシブ強度
	float timer = 0.0f;				// 更新時間
	float scanTiling = 20.0f;		// 解像度
	float scanSpeed = -2.0f;		// スクロール速度
	float scanBorder = 10.0f;		// 描画範囲
	float glowTiling = 1.0f;		// 解像度
	float glowSpeed = 2.0f;			// スクロール速度
	float glowBorder = 10.0f;		// 描画範囲
	float hologramBorder = 10.0f;	// 描画範囲
	float rimStrength = 3.0f;		// リムライト強度
	float glitchSpeed = 50.0f;		// スクロール速度
	float glitchIntensity = 0.0f;	// 強度
	float lerpGlitchIntensity = 0.0f;	// 強度
	float glitchScale = 50.0f;		// 振れ幅
	bool isActiveStart = false;
	float activeTimer = 0.0f;		// 行動開始時間
	float deathTimer = 0.0f;		// 死亡時間

	int coolTime = 0;
};
#pragma once

