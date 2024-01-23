#pragma once

#include "Shader.h"

class Trap
{
public:
	Trap() {}
	~Trap() {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) = 0;
	// 残像エフェクト描画処理
	virtual void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader) = 0;
	// デバッグプリミティブ描画処理
	virtual void DrawDebugPrimitive();
	// デバッグ情報表示
	virtual void DrawDebugGUI() = 0;

	// 行列更新処理
	void UpdateTransform(int axisType, int lengthType);

	// 破棄
	void Destroy();

	// 一番近い敵索敵
	bool SearchEnemy(float territoryRange, float radius);

	// 旋回処理
	void TurnToTarget(float elspsedTime, float speedRate);
	// XZ平面旋回処理
	void TurnHorizontal(float elapsedTime, float vx, float vz, float speed);
	// Y軸旋回処理
	void TurnVertical();

	// ダメージ処理
	void InputDamage(int damage);

	// ホログラムシェーダー情報初期化
	void HologramShaderDataInitialize(float minHeight, float maxHeight);
	// ホログラムシェーダー更新処理
	void UpdateHologramShader(float elapsedTime, bool isActivate);

public:
	// 行列取得
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// 位置設定と取得
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 角度設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	// 半径取得
	float GetRadius()const { return radius; }

	// 高さ取得
	float GetHeight() const { return height; }

	// HP描画フラグ取得
	//bool GetHpRenderFlag() const { return hpRenderFlag; }

	// 設置前後フラグ設定と取得
	void SetIsActive(bool isActivate) { this->isActivate = isActivate; }
	bool GetIsActive() { return isActivate; }

	// インスタンシング取得
	bool GetInstancing() { return instancing; }

	// HP設定と取得
	void SetHealth(const int health) { this->health = health; }
	int GetHealth() const { return health; }
	int GetMaxHealth() const { return maxHealth; }	

	// トラップ種類設定と取得
	void SetType(int type) { this->type = type; }
	int GetType() { return type; }

	
	float GetHologramTimer() { return hologramTimer; }
	float GetScanGlowBorder() { return scanBorder; }
	float GetHologramBorder() { return hologramBorder; }
	float GetHologramColor() { return hologramColor; }
	float GetYUp() { return yUp; }

public:
	// トラップ種類
	enum TrapType
	{
		TrapTurret,	// タレット
		TrapMine,	// 地雷
		TrapTotem,	// トーテム
		TrapDecoy,	// デコイ

		TrapMax,
	};

protected:
	DirectX::XMFLOAT4X4		transform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };		// 行列
	DirectX::XMFLOAT3		position = {};											// 位置
	DirectX::XMFLOAT3		targetPosition = {};									// ターゲット位置
	DirectX::XMFLOAT3		angle = {};												// 角度
	DirectX::XMFLOAT3		scale = { 50.0f, 50.0f, 50.0f };						// サイズ
	DirectX::XMFLOAT3		territoryOrigin = { 0.0f, 0.0f, 0.0f };					// テリトリー位置
	float					radius = 1.0f;											// 半径
	float					height = 1.0f;											// 高さ
	float					turnSpeed = 5.0f;										// 旋回速度
	float					territoryRange = 10.0f;									// テリトリー範囲
	float					notAttackRange = 5.0f;									// 攻撃不可範囲
	float					dist = 0.0f;											// 距離
	float					hologramColor = 1.0f;									// ホログラム色
	float					scanTimer = 0.0f;										// スキャンラインの時間
	float					scanBorder = 0.0f;										// スキャンラインの描画範囲
	float					glowTimer = 0.0f;										// グロウラインの時間
	float					glowBorder = 0.0f;										// グロウラインの描画範囲
	float					hologramTimer = 0.0f;									// ホログラムラインの時間
	float					hologramBorder = 0.0f;									// ホログラムラインの描画範囲
	float					minHeight = 0.0f;										// 最低点
	float					maxHeight = 0.0f;										// 最高点
	float					progressTimer = 0.0f;									// 経過時間	
	float					glitchIntensity = 0.0f;									// 強度
	float					glitchScale = 50.0f;									// 振れ幅
	float					yUp = 1;
	float					brokenTimer = 1.0f;										// 破壊時の破棄までの時間
	//bool					hpRenderFlag = false;									// HP描画フラグ
	bool					isActivate = false;										// 設置前後
	int						health = 10;											// 体力
	int						maxHealth = 10;											// 最大体力
	int						attack = 1;												// ダメージ量
	int						type = 0;												// 敵の種類
	bool					instancing = false;										// インスタンシング
};