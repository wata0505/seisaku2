#pragma once

#include "Trap.h"
#include "ObjectManager.h"

class Turret : public Trap
{
public:
	Turret();
	~Turret() {}

public:
	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;
	// 残像エフェクト描画処理
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader) override;
	// デバッグプリミティブ描画処理
	void DrawDebugPrimitive() override;
	// デバッグ情報表示
	void DrawDebugGUI() override;

private:
	// 土台行列更新処理
	void UpdateTransformBody(int axisType, int lengthType);

	// 弾とエネミーの衝突処理
	void CollisionProjectilesVsEnemies();

	// 待機ステートへ遷移と更新処理
	void TransitionIdleState();
	void UpdateIdleState(float elapsedTime);

	// 攻撃ステートへ遷移と更新処理
	void TransitionAttackState();
	void UpdateAttackState(float elapsedTime);

	// 死亡ステートと更新処理
	void TransitionDeadState();
	void UpdateDeadState(float elapsedTime);

private:
	// タレットアニメーション
	enum  TurretAnimation
	{
		Attack,
	};

	// ステート
	enum class State
	{
		Idle,
		Attack,
		Dead,
	};

private:
	std::unique_ptr<Model>	gunModel = nullptr;										// 銃身モデル
	std::unique_ptr<Model>	bodyModel = nullptr;									// 土台モデル
	std::shared_ptr<Model>	beem = nullptr;											// 弾モデル
	ObjectManager			objectManager;											// 弾
	State					state = State::Idle;									// 状態
	DirectX::XMFLOAT4X4		bodyTransform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };	// 土台行列
	int						coolTime = 0;											// クールタイム
};