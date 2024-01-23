#pragma once

#include<DirectXMath.h>

//キャラクター
class Character
{
public:
	enum  AxisType
	{
		RHSYUP, //右手系
		LHSYUP, //左手系
		RHSZUP, //右手系
		LHSZUP //左手系
	};
	enum   LengthType
	{
		Cm, //cm系
		M  //m系
		
	};
public:
	Character() {}
	virtual ~Character() {}

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	const DirectX::XMFLOAT3 GetEfPos() { return { position.x,position.y + height / 2,position.z }; }

	//回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//スケール取得
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//行列更新処理
	//i = 0:RHS Y-UP　右手系
	//i = 1:LHS Y-UP　左手系
	//i = 2:RHS Z-UP　右手系
	//i = 3:LHS Z-UP　左手系
	//j = 0 cm系
	//j = 1 m系
	void UpdateTransform(int axisType,int lengthType);

	//半径取得
	float GetRadius()const { return radius; }

	//地面に接地しているか
	bool IsGround()const { return isGround; }

	//重力値設定
	void SetGravity(float g){ gravity = g; }

	//高さ取得
	float GetHeight() const { return height; }

	//質量取得
	float GetMass() const { return mass; }

	// ダメージを与える
	bool ApplyDamage(int damage, float invincibleTime, bool DamageFlag = true);

	void SetInvincibleTime(float invincibleTime);

	// 衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);


	// 健康状態を取得
	int GetHealth() const { return health; }

	// 最大健康状態を取得
	int GetMaxHealth() const { return maxHealth; }

	// TODO:02/20 0
	// ノックバック処理
	void KnockBack(Character& taisyou, DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY);

	// 行列の取得
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	//回転設定
	void SetAngle(const DirectX::XMFLOAT4X4& angle) { this->transform = angle; }

	//最大速度設定
	void SetMaxMoveSpeed(float max) { this->maxMoveSpeed = max; }

	int GetSaveDamage() { return saveDamage; }

	void  SetFriction(float fn) { this->friction = fn; }

	bool GetDamageFlag() { return damageFlag; }

	float GetInvincibleTimer() { return invincibleTimer; }

	//旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);
	//ジャンプ処理
	void JumpInput(float speed);

	DirectX::XMFLOAT3 GetVelocity() { return velocity; };

    void SetVelocity(DirectX::XMFLOAT3 v) { velocity = v; };
	void SetVelocityY(float y) { velocity.y = y; };

	bool GetFallDeath() { return fallDeath; }

	float GetMaxMoveSpeed() { return maxMoveSpeed; }

	// ホログラムシェーダー情報初期化
	void HologramShaderDataInitialize(float minHeight, float maxHeight);
	// ホログラムシェーダー更新処理
	bool UpdateHologramShader(float elapsedTime);

protected:

	//移動処理
	//void Move(float elapsedTime, float vx, float vz, float speed);
	void MoveInput(float vx, float vz, float speed);

	


	//垂直旋回処理
	void VerticalTurn(float elapsedTime, float vx, float vz, float vy, float speed);


	//速度処理更新
	void UpdateVelocity(float elapsedTime);

	virtual void OnLanding() {}

	// ダメージを受けた時に呼ばれる
	virtual void OnDamaged() {}

	// 死亡した時に呼ばれる
	virtual void OnDead() {}

	// 無敵時間更新
	void UpdateInvincibleTimer(float elapsedTime);

	

private:
	// 垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedFrame);

	// 垂直移動更新処理
	virtual void UpdateVerticalMove(float elapsedTime);

	// 水平速力更新処理
	void UpdateHorizontalVelocity(float elapsedFrame);

	// 水平移動更新処理
	virtual void UpdateHorizontalMove(float elapsedTime);

	// レイキャスト関数の中身省略(使いまわし)
	bool OmissionRaycastContents(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMVECTOR Velocity);

protected:
	DirectX::XMFLOAT3		position = { 0,0,0 };
	DirectX::XMFLOAT3		angle = { 0,0,0 };
	DirectX::XMFLOAT3		scale = { 1,1,1 };
	DirectX::XMFLOAT4X4		transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	float radius = 0.7f;
	float gravity = -1.0f;
	DirectX::XMFLOAT3   velocity = { 0,0,0 };
	//地面と接地しているか
	bool isGround = false;
	float height = 2.0f;
	int health = 5;
	//無敵時間
	float invincibleTimer = 1.0f;
	//摩擦
	float friction = 0.9f;
	//加速力
	float	acceleration = 1.5f;
	bool jumpflag = false;
	//質量
	float   mass = 1.0f;
	float	maxMoveSpeed = 5.0f;
	float	moveVecX = 0.0f;
	float	moveVecZ = 0.0f;
	float   stepOffset = 1.0f;
	float   slopeRate = 1.0f;
	//死亡高さ
	float   deathHight = -5;
	//落下死
	bool    fallDeath = false;
	int     maxHealth = 5;
	//上昇の限界値
	float   altitude = 15;
	//ダメージ保存
	int     saveDamage = 0;
    //無敵時間フラグ
	bool damageFlag = false;

	DirectX::XMFLOAT2 lastPoint = {};
	float scanTimer = 0.0f;			// スキャンラインの時間
	float scanBorder = 0.0f;		// スキャンラインの描画範囲
	float glowTimer = 0.0f;			// グロウラインの時間
	float glowBorder = 0.0f;		// グロウラインの描画範囲
	float hologramTimer = 0.0f;		// ホログラムラインの時間
	float hologramBorder = 0.0f;	// ホログラムラインの描画範囲
	float minHeight = 0.0f;			// 最低点
	float maxHeight = 0.0f;			// 最高点
	bool isActiveStart = false;		// ホログラムシェーダー実行中フラグ
};