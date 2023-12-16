#pragma once
#include "Shader.h"


//トラップ
class Trap
{
public:
	Trap() {}
	~Trap() {}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;
	//残像エフェクト描画
	virtual void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader) = 0;
	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();
	// デバッグ情報表示
	virtual void DrawDebugGUI() = 0;

	//破棄
	void Destroy();


	//行列更新処理
	void UpdateTransform(int axisType, int lengthType);

	//一番近い敵索敵
	bool SearchEnemy(float territoryRange, float radius);
	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);
	void TurnToTarget(float elspsedTime, float speedRate);
	//旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);
	void TurnVertical();

	// ホログラムシェーダー情報初期化
	void HologramShaderDataInitialize(float minHeight, float maxHeight);
	// ホログラムシェーダー更新処理
	void UpdateHologramShader(float elapsedTime, bool isActivate);

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 角度設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	//半径取得
	float GetRadius()const { return radius; }

	//高さ取得
	float GetHeight() const { return height; }

	//HP
	int GetHealth() const { return health; }
	int GetMaxHealth() const { return maxHealth; }
	void SetHealth(const int health) { this->health = health; }

	void InputDamage(int damage);

	bool GetHpRenderFlag() const { return hpRenderFlag; }

	enum TrapType
	{
		TrapTurret,
		TrapMine,
		TrapTotem,
		TrapDecoy,

		TrapMax,
	};
	//敵種類
	void	SetType(int type) { this->type = type; }
	int		GetType() { return type; }
	bool GetActiveFlag() { return this->activateFlag; }
	void SetActiveFlag(bool activateflag) { this->activateFlag = activateflag; }
protected:

	int type = 0;//敵の種類

	DirectX::XMFLOAT3		position = { 0,0,0 };
	DirectX::XMFLOAT3		targetPosition = { 0,0,0 };
	DirectX::XMFLOAT3		angle = { 0,0,0 };
	DirectX::XMFLOAT3		scale = { 50,50,50 };
	DirectX::XMFLOAT4X4		transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	int health = 10;
	int maxHealth = 10;
	int attack = 1;
	float radius = 1.0f;
	float height = 1.0f;
	float turnSpeed = 5.0f;

	float dist = 0.0f;

	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange = 10.0f;
	float notAttackRange = 5.0f;

	bool center = false;

	bool hpRenderFlag = false;

	bool activateFlag = false;
	
	DirectX::XMFLOAT3 hologramColor = { 0.0f, 1.0f, 0.0f };
	float scanTimer = 0.0f;			// スキャンラインの時間
	float scanBorder = 0.0f;		// スキャンラインの描画範囲
	float glowTimer = 0.0f;			// グロウラインの時間
	float glowBorder = 0.0f;		// グロウラインの描画範囲
	float hologramTimer = 0.0f;		// ホログラムラインの時間
	float hologramBorder = 0.0f;	// ホログラムラインの描画範囲
	float minHeight = 0.0f;			// 最低点
	float maxHeight = 0.0f;			// 最高点
	float adjustMetalness = 0.0f;	// 金属度
	float adjustSmoothness = 0.0f;	// 粗さ
	float timer = 0.0f;				// 更新時間	
	float glitchSpeed = 50.0f;		// スクロール速度
	float glitchIntensity = 0.0f;	// 強度
	float lerpGlitchIntensity = 0.0f;	// 強度
	float glitchScale = 50.0f;		// 振れ幅	
};