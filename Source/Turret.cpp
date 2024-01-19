#include"Turret.h"
#include "Graphics.h"
#include "ProjectileStraite.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "AudioAll.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "TrapManager.h"
#include "Player.h"
#include "Input.h"
#include "Calculation.h"
Turret::Turret()
{

	beem = std::make_unique<Model>(".\\resources\\Cube.fbx", true);
	beem->ModelSerialize(".\\resources\\Cube.fbx");
	beem->ModelRegister(".\\resources\\Cube.fbx");
	beem->UpdateBufferDara(transform);


	model = std::make_unique<Model>(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx", true);
	model->AppendAnimations(".\\resources\\Sentorygun\\GUN_motionBullet.fbx", 0);

	model->ModelSerialize(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx");
	model->ModelRegister(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx", "Taret_BODY.fbm\\TARET_GUN_BaseColor.png");

	model2 = std::make_unique<Model>(".\\resources\\Trap\\Sentorygun\\Taret.fbx", true);
	model2->ModelSerialize(".\\resources\\Trap\\Sentorygun\\Taret.fbx");
	model2->ModelRegister(".\\resources\\Trap\\Sentorygun\\Taret.fbx", "Taret.fbm\\TARET_BODY_BaseColor.png");

	UpdateTransform(0, 0);
	UpdateTransform2(0, 0);

	model->UpdateBufferDara(transform);
	model2->UpdateBufferDara(transform2);

	renderdata = model->GetBufferData();
	renderdata2 = model2->GetBufferData();


	radius = 2.3f;
	height = 10;
	notAttackRange = 15.0f;
	territoryRange = 40.0f;
	maxHealth = 100;
	health = maxHealth;
	
	scale.x = scale.y = scale.z = 150.0f;
	//scale.x = scale.y = scale.z = 300.0f;
	TransitionIdleState();
	attack = 1;
	type = Trap::TrapType::TrapTurret;

	// ホログラムシェーダー情報初期化
	HologramShaderDataInitialize(-2.5f, 2.0f);
	
}
Turret::~Turret()
{

}

void Turret::Update(float elapsedTime)
{
	timer += elapsedTime;
	model->ShaderAdjustment(adjustMetalness, adjustSmoothness, glitchScale, timer, maxHeight);
	model2->ShaderAdjustment(adjustMetalness, adjustSmoothness, glitchScale, timer, maxHeight);

	// ホログラムシェーダー更新処理
	UpdateHologramShader(elapsedTime, activateFlag);
	if (activateFlag)
	{
		hologramColor = { 0.0f, 1.0f, 0.0f };

		// ステート毎の更新処理
		switch (state)
		{
		case State::Idle:
			UpdateIdleState(elapsedTime);
			break;
		case State::Attack:
			UpdateAttackState(elapsedTime);
			break;
		case State::Dead:
			UpdateDeadState(elapsedTime);
			break;
		}
	}
	else
	{
		hologramColor = TrapManager::Instance().GetHologramColor();
	}

	UpdateTransform(0, 0);
	UpdateTransform2(0, 0);

	//アニメーション更新
	model->UpdateAnimation(elapsedTime, "root_x");
	model->UpdateSubAnimation(elapsedTime, "root_x");

	model->UpdateBufferDara(transform);
	model2->UpdateBufferDara(transform2);

	//モデル描画情報受け渡し
	renderdata = model->GetBufferData();
	renderdata2 = model2->GetBufferData();

	//弾入力
	//InputProjectile();
	//弾更新
	objectManager.Update(elapsedTime * 1.5f);
	//弾当たり判定
	CollisionProjectilesVsEnemies();
	hpRenderFlag = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);
}

void Turret::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	shader->Draw(dc, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor.x, hologramColor.y, hologramColor.z, 1.0f });
	shader->Draw(dc, model2.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor.x, hologramColor.y, hologramColor.z, 1.0f });
}
void Turret::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
	objectManager.Render(immediate_context.Get(), shader);
}
void Turret::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	//Trap::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバック円柱を描画
	debugRenderer->DrawCylinder({ position.x,6.0f,position.z }, radius, height, DirectX::XMFLOAT4(1, 0, 1, 1));
	//
	////衝突判定用のデバック円柱を描画
	//debugRenderer->DrawCylinder({ position.x,-2.5f,position.z }, notAttackRange, height, DirectX::XMFLOAT4(1, 0, 0, 1));
	//
	//
	//// 縄張り範囲をデバッグ円柱描画
	//debugRenderer->DrawCylinder({ territoryOrigin.x,-2.5f,territoryOrigin.z }, territoryRange, height, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

// デバッグ情報表示
void Turret::DrawDebugGUI()
{
	//トランスフォーム
	if (ImGui::CollapsingHeader("Turret", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::TreeNode("PBR"))
		{
			ImGui::SliderFloat("adjustMetalness", &adjustMetalness, -1.0f, 1.0f);
			ImGui::SliderFloat("adjustSmoothness", &adjustSmoothness, -1.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Hologram"))
		{
			ImGui::SliderFloat("scanBorder", &scanBorder, minHeight, maxHeight);
			ImGui::SliderFloat("glowBorder", &glowBorder, minHeight, maxHeight);
			ImGui::SliderFloat("hologramBorder", &hologramBorder, minHeight, maxHeight);
			ImGui::SliderFloat("scanTimer", &scanTimer, -1.0f, 2.0f);
			ImGui::SliderFloat("glowTimer", &glowTimer, -1.0f, 2.0f);
			ImGui::SliderFloat("hologramTimer", &hologramTimer, -1.0f, 2.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Glitch"))
		{
			//ImGui::SliderFloat("glitchSpeed", &glitchSpeed, 0.0f, 10.0f);
			ImGui::SliderFloat("glitchSpeed", &glitchSpeed, 1.0f, 50.0f);
			ImGui::SliderFloat("glitchIntensity", &glitchIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("glitchScale", &glitchScale, 1.0f, 50.0f);
			ImGui::TreePop();
		}
	}
}

//行列更新処理
void Turret::UpdateTransform2(int axisType, int lengthType)
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		{ -1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//0:RHS Y-UP　右手系
		{  1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//1:LHS Y-UP　左手系
		{ -1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1},		//2:RHS Z-UP　右手系
		{  1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},		//3:LHS Z-UP　左手系
	};

	const float scale_factor[] = { 0.01f,1.0f };

	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[axisType]) * DirectX::XMMatrixScaling(scale_factor[lengthType],scale_factor[lengthType],scale_factor[lengthType]) };
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
	DirectX::XMStoreFloat4x4(&transform2, W);
}


// 待機ステートへ遷移
void Turret::TransitionIdleState()
{
	state = State::Idle;
	model->PlayAnimation(-1, false);
}

// 待機ステート更新処理
void Turret::UpdateIdleState(float elapsedTime)
{
	if (GetHologramTimer() >= 1)
	{
		// プレイヤー索敵
		if (SearchEnemy(territoryRange, notAttackRange))
		{
			// 見つかったら追跡ステートへ遷移
			TransitionAttackState();
		}
	}

	if (health <= 0)
	{
		TransitionDeadState();
	}
}

// 攻撃ステートへ遷移
void Turret::TransitionAttackState()
{
	state = State::Attack;
	model->PlayAnimation(static_cast<int>(TurretAnimation::Attack), true);
}

// 攻撃ステート更新処理
void Turret::UpdateAttackState(float elapsedTime)
{
	TurnToTarget(elapsedTime, turnSpeed);
	coolTime--;
	if (coolTime <= 0)
	{
		//// 前方向   		
		float h = 0;
		// 発射
		
			h = 0.4f;
			ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
			projectile->TurretLaunch(beem, h, 1.5f, position, targetPosition, angle.y, Type::Beem, (int)EffectTexAll::EfTexAll::BlueThader, 1, 1, 0.0f);
		
		coolTime = 20;
	}
	else
	{
		//DirectX::XMFLOAT3 v = Vector3::Subset(targetPosition,position);
		//v = Vector3::Normalize(v);
		//v = Vector3::PosDir(position, v, 5);
		//ParticleSystem::Instance().SeirlConvergenceEffect(v, 90,20);
		//ParticleSystem::Instance().SeirlConvergenceEffect(v, 180,20);
		//ParticleSystem::Instance().SeirlConvergenceEffect(v, 270,20);
	}

	// 攻撃アニメーションが終わって範囲内に敵がいなければ
	if (SearchEnemy(territoryRange, notAttackRange) == false)
	{
		//待機ステートへ
		TransitionIdleState();
	}

	if (health <= 0)
	{
		TransitionDeadState();
	}
}

// 死亡ステートへ遷移
void Turret::TransitionDeadState()
{
	state = State::Dead;
	model->PlayAnimation(-1, true);

}

// 死亡ステートへ遷移
void Turret::UpdateDeadState(float elapsedTime)
{
	Destroy();
}

void Turret::CollisionProjectilesVsEnemies()
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
				if (enemy->ApplyDamage(attack, 0.0f))
				{
					if (object->GetType() == Type::Beem) {
						ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::BlueThader), 3, 0.5f, { NULL,NULL,2,1 });
						ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 200, 1.5f, 0, true, 0.015f, 0.06f, { 0,0,1,1 });
						continue;
					}
				}
			}
		}
	}
}