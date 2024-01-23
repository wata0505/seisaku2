#include "Turret.h"
#include "TrapManager.h"
#include "Graphics.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "ProjectileStraite.h"
#include "AudioAll.h"
#include "Calculation.h"

// コンストラクタ
Turret::Turret()
{
	beem = std::make_unique<Model>(".\\resources\\Cube.fbx", true);
	beem->ModelSerialize(".\\resources\\Cube.fbx");
	beem->ModelRegister(".\\resources\\Cube.fbx");
	beem->UpdateBufferData(transform);

	gunModel = std::make_unique<Model>(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx", true);
	gunModel->AppendAnimations(".\\resources\\Sentorygun\\GUN_motionBullet.fbx", 0);
	gunModel->ModelSerialize(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx");
	gunModel->ModelRegister(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx", "Taret_BODY.fbm\\TARET_GUN_BaseColor.png");

	bodyModel = std::make_unique<Model>(".\\resources\\Trap\\Sentorygun\\Taret.fbx", true);
	bodyModel->ModelSerialize(".\\resources\\Trap\\Sentorygun\\Taret.fbx");
	bodyModel->ModelRegister(".\\resources\\Trap\\Sentorygun\\Taret.fbx", "Taret.fbm\\TARET_BODY_BaseColor.png");

	scale.x = scale.y = scale.z = 150.0f;			// サイズ設定
	radius = 2.3f;									// 半径設定
	height = 10.0f;									// 高さ設定
	territoryRange = 40.0f;							// テリトリー範囲設定
	notAttackRange = 15.0f;							// 攻撃不可範囲設定
	maxHealth = 100;								// 最大体力設定
	health = maxHealth;								// 体力設定
	attack = 1;										// ダメージ量設定
	type = Trap::TrapType::TrapTurret;				// トラップ種類設定
	
	// 待機ステートへ遷移
	TransitionIdleState();

	// ホログラムシェーダー情報初期化
	HologramShaderDataInitialize(-2.5f, 2.0f);

	UpdateTransform(0, 0);							// 銃身行列更新処理
	UpdateTransformBody(0, 0);						// 土台行列更新処理
	gunModel->UpdateBufferData(transform);			// 銃身描画情報更新処理
	bodyModel->UpdateBufferData(bodyTransform);		// 土台描画情報更新処理
}

// 更新処理
void Turret::Update(float elapsedTime)
{
	// 経過時間加算
	progressTimer += elapsedTime;
	// シェーダー情報調整
	gunModel->ShaderAdjustment(glitchScale, progressTimer, maxHeight);
	bodyModel->ShaderAdjustment(glitchScale, progressTimer, maxHeight);

	// ホログラムシェーダー更新処理
	UpdateHologramShader(elapsedTime, isActivate);

	// 設置後
	if (isActivate)
	{
		hologramColor = 2.0f;										// ホログラム色(緑)設定

		// ステート毎の更新処理
		switch (state)
		{
		case State::Idle:		// 待機
			UpdateIdleState(elapsedTime);
			break;
		case State::Attack:		// 攻撃
			UpdateAttackState(elapsedTime);
			break;
		case State::Dead:		// 死亡
			UpdateDeadState(elapsedTime);
			break;
		}
	}
	// 設置前
	else
	{
		hologramColor = TrapManager::Instance().GetHologramColor();	// 設置可不可色設定
	}

	UpdateTransform(0, 0);		// 銃身行列更新処理
	UpdateTransformBody(0, 0);	// 土台行列更新処理

	// アニメーション更新
	gunModel->UpdateAnimation(elapsedTime, "root_x");
	gunModel->UpdateSubAnimation(elapsedTime, "root_x");

	gunModel->UpdateBufferData(transform);		// 銃身描画情報更新処理
	bodyModel->UpdateBufferData(bodyTransform);	// 土台描画情報更新処理

	// オブジェクト更新処理
	objectManager.Update(elapsedTime * 1.5f);

	// 弾と敵の衝突処理
	CollisionProjectilesVsEnemies();

#if 0
	hpRenderFlag = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);
#endif
}

// 描画処理
void Turret::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	shader->Draw(deviceContext, gunModel.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 1.0f });
	shader->Draw(deviceContext, bodyModel.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 1.0f });
}
// 残像エフェクト描画処理
void Turret::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader)
{
	objectManager.Render(deviceContext.Get(), shader);
}
// デバッグプリミティブ描画処理
void Turret::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	//Trap::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバック円柱を描画
	debugRenderer->DrawCylinder({ position.x,6.0f,position.z }, radius, height, DirectX::XMFLOAT4(1, 0, 1, 1));
	
	//衝突判定用のデバック円柱を描画
	//debugRenderer->DrawCylinder({ position.x,-2.5f,position.z }, notAttackRange, height, DirectX::XMFLOAT4(1, 0, 0, 1));
	
	// 縄張り範囲をデバッグ円柱描画
	//debugRenderer->DrawCylinder({ territoryOrigin.x,-2.5f,territoryOrigin.z }, territoryRange, height, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}
// デバッグ情報表示
void Turret::DrawDebugGUI()
{
	// トランスフォーム
	if (ImGui::CollapsingHeader("Turret", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::TreeNode("Hologram"))
		{
			ImGui::SliderFloat("scanTimer", &scanTimer, -1.0f, 2.0f);
			ImGui::SliderFloat("glowTimer", &glowTimer, -1.0f, 2.0f);
			ImGui::SliderFloat("hologramTimer", &hologramTimer, -1.0f, 2.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Glitch"))
		{			
			ImGui::SliderFloat("glitchIntensity", &glitchIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("glitchScale", &glitchScale, 1.0f, 50.0f);
			ImGui::TreePop();
		}
	}
}

// 土台行列更新処理
void Turret::UpdateTransformBody(int axisType, int lengthType)
{
	const DirectX::XMFLOAT4X4 coordinateSystemTransforms[] = 
	{
		{ -1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		// 0:RHS Y-UP　右手系
		{  1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		// 1:LHS Y-UP　左手系
		{ -1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1 },		// 2:RHS Z-UP　右手系
		{  1,0,0,0, 0,0, 1,0, 0,1,0,0, 0,0,0,1 },		// 3:LHS Z-UP　左手系
	};

	const float scaleFactor[] = { 0.01f, 1.0f };

	DirectX::XMMATRIX C = { DirectX::XMLoadFloat4x4(&coordinateSystemTransforms[axisType]) * DirectX::XMMatrixScaling(scaleFactor[lengthType], scaleFactor[lengthType], scaleFactor[lengthType]) };
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0, angle.z);
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y - 2.5f, position.z);//scale=150
	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y - 5.0f, position.z);//scale=300
	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = C * S * R * T;
	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&bodyTransform, W);
}

// 弾と敵の衝突処理
void Turret::CollisionProjectilesVsEnemies()
{
	// 全ての弾丸とすべての敵を総当たりで衝突判定
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	int projectileCount = objectManager.GetObjectCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Object* object = objectManager.GetObjectW(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			// 衝突処理
			Enemy* enemy = enemyManager.GetEnemy(j);
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
				if (enemy->ApplyDamage(attack, 0.0f))
				{
					// オブジェクト種類がビームなら
					if (object->GetType() == Type::Beem)
					{
						ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::BlueThader), 3.0f, 0.5f, { NULL, NULL, 2, 1 });
						ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), { NULL, NULL, NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 200, 1.5f, 0.0f, true, 0.015f, 0.06f, { 0.0f, 0.0f, 1.0f, 1.0f });
						continue;
					}
				}
			}
		}
	}
}

// 待機ステートへ遷移と更新処理
void Turret::TransitionIdleState()
{
	state = State::Idle;				// 待機状態
	gunModel->PlayAnimation(-1, false);	// アニメーション指定なし
}
void Turret::UpdateIdleState(float elapsedTime)
{
	// ホログラムが終了したら
	if (GetHologramTimer() >= 1.0f)
	{
		// プレイヤー索敵
		if (SearchEnemy(territoryRange, notAttackRange))
		{
			// 見つかったら追跡ステートへ遷移
			TransitionAttackState();
		}
	}

	// 体力が残っていなければ
	if (health <= 0)
	{
		// 死亡ステートへ遷移
		TransitionDeadState();
	}
}

// 攻撃ステートへ遷移と更新処理
void Turret::TransitionAttackState()
{
	state = State::Attack;														// 攻撃状態
	gunModel->PlayAnimation(static_cast<int>(TurretAnimation::Attack), true);	// 攻撃アニメーション再生
}
void Turret::UpdateAttackState(float elapsedTime)
{
	// 旋回処理
	TurnToTarget(elapsedTime, turnSpeed);

	coolTime--;			// クールタイム減少
	// クールタイムが残っていなければ
	if (coolTime <= 0)
	{	
		ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
		projectile->TurretLaunch(beem, 0.4f, 1.5f, position, targetPosition, angle.y, Type::Beem, (int)EffectTexAll::EfTexAll::BlueThader, 1.0f, 1.0f, 0.0f);
		float volume = 0.3f;
		// カメラからの距離に応じて音量調節
		{
			float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMLoadFloat3(&projectile->GetPosition()))));
			if (length > 20.0f)
			{
				volume = (20.0f / length) * 0.3f;
			}
			else
			{
				volume = 0.3f;
			}
		}
		// 発射音再生
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Shot)->Play(false, volume);
		coolTime = 20;	// クールタイム設定
	}

	// 攻撃アニメーションが終わって範囲内に敵がいなければ
	if (!SearchEnemy(territoryRange, notAttackRange))
	{
		//待機ステートへ遷移
		TransitionIdleState();
	}

	// 体力が残っていなければ
	if (health <= 0)
	{
		// 死亡ステートへ遷移
		TransitionDeadState();
	}
}

// 死亡ステートへ遷移と更新処理
void Turret::TransitionDeadState()
{
	state = State::Dead;				// 死亡状態
	gunModel->PlayAnimation(-1, true);	// アニメーション指定なし
	glitchIntensity = 1.0f;				// グリッチ設定
}
void Turret::UpdateDeadState(float elapsedTime)
{
	brokenTimer -= elapsedTime;	// 破壊時の破棄までの時間減少
	// 破壊時の破棄までの時間が残っていなければ
	if (brokenTimer <= 0.0f)
	{
		Destroy();	// 破棄
	}
}