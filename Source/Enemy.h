#pragma once

#include "Shader.h"
#include "Character.h"
#include "Telegram.h"
#include "StateMachine.h"

// エネミー
class Enemy : public Character
{
public:

	Enemy() {}
	~Enemy() override {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;


	// 破棄
	void Destroy();

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	// デバッグエネミー情報表示
	virtual void DrawDebugGUI();

	// 攻撃フラグ取得
	virtual bool	GetAttackFlg() { return attackFlg; }
	// 攻撃フラグセット
	virtual void	SetAttackFlg(bool flg) { attackFlg = flg; };
	virtual void	SetId(int id) { this->id = id; }
	virtual int		GetId() { return id; }
	// 攻撃範囲取得
	float GetAttackRange() { return attackRange; }

	float GetSearchRange() { return searchRange; }

	// メッセージ受信したときの処理
	virtual bool OnMessage(const Telegram& msg);

	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	void UpdateVerticalMove(float elapsedTime);

	void UpdateHorizontalMove(float elapsedTime);

	void UpdateRnderflag();

	void SetTargetPosition(DirectX::XMFLOAT3 t) { targetPosition = t; }

	void ReMove() {};

	bool GetActiveflag() { return activeflag; }
	bool GetRenderflag() { return renderflag; }
	void SetActiveflag(bool flag) { activeflag = flag; }
	void SetReMoveflag(bool flag) { reMoveflag = flag; }

	void SetReMoveTimer(float timer) { reMoveTimer = timer; }

	void SetLowAltitude(float low) { lowAltitude = low; }

	void SetHealth(float heal) { health = static_cast<int>(heal); }

	Animation::Keyframe GetEnemyKeyframe() { return enemyKeyframe; };

	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius, DirectX::XMFLOAT2 pow, float damage,float invincibleTime = 1.0);
	void CollisionNodeVsBase(const char* nodeName, float nodeRadius, DirectX::XMFLOAT2 pow, float damage, float invincibleTime = 1.0);

	virtual DirectX::XMFLOAT3 SearchNodePos(const char* nodeName) = 0;

	int GetEnemyTimer() { return enemyTimer; }

	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	float GetNodeRudius() { return nodeRudius; }

	//バリア状態取得
	bool GetBarrierFlag() { return barrierFlg; }
	//バリア状態取得
	void SetBarrierFlag(bool barrierFlg) { this->barrierFlg = barrierFlg; }

	// デバッグエネミー情報表示
	//void DrawDebugGUI();
	// ターゲット位置をランダム設定
	void SetRandomTargetPosition();

	void HitInpnt(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 pow, float damage, float invincibleTime);

protected:
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	Animation::Keyframe enemyKeyframe = {};
	DirectX::XMFLOAT3   nodePosition = {};
	int	state = 0;
	//攻撃権
	bool attackFlg = false;
	//バリア状態
	bool barrierFlg = false;
	int id = 0;
	//索敵範囲
	float searchRange = 0.0f;
	//バトルステート範囲
	float attackRange = 0.0f;
	//アクティブフラグ
	bool  activeflag = true;
	//再起動
	bool  reMoveflag = false;
	//再起動までの時間
	float  reMoveTimer = 0;
	//下降限界
	float lowAltitude = 2.1f;
	float efLife = 0.3f;
	float efMax = 1.0f;
	float eria = 50.0f;
	bool renderflag = true;
	//ノード当たり判定
	float nodeRudius = 1.0f;
	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	int enemyTimer = 0;
	
};