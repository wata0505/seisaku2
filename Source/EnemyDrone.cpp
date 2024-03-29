#include "EnemyDrone.h"
#include "Graphics.h"
#include "Mathf.h"
#include "player.h"
#include "StateDrone.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ParticleSprite.h"
#include "ProjectileStraite.h"
#include "Audio.h"
#include "AudioAll.h"
#include "Calculation.h"
#include "FireBall.h"
#include "ParticleSystem.h"
#include "ParticleSprite.h"
#include"Base.h"
#include "Mathf.h"

#include"TrapManager.h"
#include "Camera.h"
// コンストラクタ
EnemyDrone::EnemyDrone(bool tutorial)
{
	beem = std::make_unique<Model>(".\\resources\\Cube.fbx", true);
	beem->ModelSerialize(".\\resources\\Cube.fbx");
	beem->ModelRegister(".\\resources\\Cube.fbx");
	beem->UpdateBufferData(transform);

	model = std::make_unique<Model>(".\\resources\\Drone\\Drone.fbx", true);
	model->ModelSerialize(".\\resources\\Drone\\Drone.fbx");
	model->ModelRegister(".\\resources\\Drone\\Drone.fbx", "Texture\\Albedo.png");
	// モデルが大きいのでスケーリング
	maxHealth = 2;
	health = maxHealth;
	radius = 0.7f;
	height = 2.0f;
	activeflag = true;
	isActiveStart = false;
	
	//StateMachineを生成し、階層型ステートマシンに対応するように登録ステートを変更していく。
	stateMachine = std::make_unique<StateMachine<EnemyDrone>>();
	// ステートマシンに1層目のステートを登録(BattleStateも同様の方法で各自追加してください)
	stateMachine->RegisterState(new DroneSearchState(this));
	stateMachine->RegisterState(new DroneBattleState(this));
	stateMachine->RegisterState(new DroneRecieveState(this));
	stateMachine->RegisterState(new DroneReDamageState(this));
	// 各親ステートにサブステートを登録(WanderState以外の2層目のステートも同様の方法で各自追加してください。)
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::Search), new DroneWanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::Search), new DroneIdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::Battle), new DronePursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::Battle), new DroneAttackState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::Battle), new DroneStandbyState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::Recieve), new  DroneCalledState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::ReDamage), new DroneDamageState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyDrone::DroneState::ReDamage), new DroneDieState(this));
	// ステートをセット
	stateMachine->SetState(static_cast<int>(DroneState::Search));
	mass = 2.0;
	UpdateTransform(0, 0);

	model->UpdateBufferData(transform);
	//renderdata = model->GetBufferData();

	//se[(int)EnemyDroneSE::Walk] = Audio::Instance().LoadAudioSource("resources\\Audio\\wolk2.wav");
	//se[(int)EnemyDroneSE::Run] = Audio::Instance().LoadAudioSource("resources\\Audio\\run3.wav");
	//se[(int)EnemyDroneSE::hit] = Audio::Instance().LoadAudioSource("resources\\Audio\\pannti.wav");
	searchRange = 25.0f;
	attackRange = 25.0f;

	scale.x = scale.y = scale.z = 1.0f;
	territoryRange = 10.0f;
	angle.y = DirectX::XMConvertToRadians(180);
	tutorialflag = tutorial;
	eria = 100;
	targetNo = 0;
	enemyType = EnemyType::Drone;
	for (int i = 0; i < MAX_ROOT_POINT; i++) {
		SetTerritory(rootPoint[i], 50);
		SetRandomTargetPosition();
		rootPoint[i] = targetPosition;
	}
	maxEria = static_cast<float>(10 + (rand() % 10));
	// ホログラムシェーダー情報初期化
	HologramShaderDataInitialize(0.0f, 16.0f);
	adjustMetalness = 0.0f;
	adjustSmoothness = 1.0f;
}

// デストラクタ
EnemyDrone::~EnemyDrone()
{
	// delete model;
}
//ルートモーション
void EnemyDrone::RootMove() {

	DirectX::XMFLOAT2 dir = ForwardToPlayer();
	velocity.x = dir.x * (model->GetRootPow() * rootSpeed);
	velocity.z = dir.y * (model->GetRootPow() * rootSpeed);
	//objectManager.Clear();
}
float EnemyDrone::MovePow() {

	return (velocity.x * velocity.x) + (velocity.z * velocity.z);
}
void EnemyDrone::Update(float elapsedTime)
{
	coolTime -= elapsedTime;
	

	timer += elapsedTime;
	if (stateMachine->GetStateNum() == static_cast<int>(EnemyDrone::DroneState::ReDamage))
	{
		lerpGlitchIntensity = 1.0f;
	}
	else
	{
		lerpGlitchIntensity = 0.0f;
	}
	glitchIntensity = Mathf::Lerp(glitchIntensity, lerpGlitchIntensity, elapsedTime * 20.0f);
	model->ShaderAdjustment(glitchScale, timer, maxHeight);
	objectManager.Update(elapsedTime);
	//描画判定
	if (reMoveflag)ReMove();
	if (!activeflag)return;
	UpdateRnderflag();
	float ElapsedTime = elapsedTime;

	if (Player::Instance().GetQuickflag())
	{
		ElapsedTime *= 0.5f;
	}
	if (health <= 0) {
		if (!model->IsPlayAnimation() && position.y < 1)
		{
			//死亡モーションが終わったデリート
			//Destroy();
			activeflag = false;
			renderflag = false;
			//position.y = -2000;
			TrapManager::Instance().SetTrapPoint(TrapManager::Instance().GetTrapPoint() + 1);
		}
		// 速力処理更新
		//velocity.y = -1.5;
		position.y += gravity * 0.5f;
		//UpdateVelocity(elapsedTime);
		UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
		model->UpdateAnimation(elapsedTime, "pelvis");
		
		
		// モデル行列更新
		model->UpdateBufferData(transform);
		//renderdata = model->GetBufferData();

		hologramTimer = 0.0f;
		UpdateHologramShader(elapsedTime);
		return;
	}
	if (position.y <= maxEria && health > 0)
	{
		position.y = static_cast<float>(maxEria);
	}
	// ホログラムシェーダー実行中フラグが付いていれば
	if (!isActiveStart)
	{
		// ホログラムシェーダー更新処理
		isActiveStart = UpdateHologramShader(elapsedTime);
		ElapsedTime = 0.0f;
	}
	//ElapsedTime = 0.0f;
	//ヒットストップ
	if (Player::Instance().GetAttackHitflag()) ElapsedTime *= hitStop;
	TargetUpdate();
	//stateMachine->SetState(static_cast<int>(DroneState::Search));
	//ステートマシン更新
	stateMachine->Update(ElapsedTime);

	// 速力処理更新
	UpdateVelocity(ElapsedTime);

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列更新
	UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);

	CollisionBeemVSTrap();
	CollisionBeemVSBase();

	model->UpdateAnimation(ElapsedTime * 4.0f, "pelvis", renderflag);
	
	// モデル行列更新
	if (renderflag) {
		model->UpdateBufferData(transform);
	}
	//モデル描画情報受け渡し
	//renderdata = model->GetBufferData();
	//SEアプデート
	SeUpdate(elapsedTime);
	enemyTimer++;
}
void EnemyDrone::SeUpdate(float elapsedTime) {
	//if (model->GetCurrentAnimationIndex() == (int)EnemyDroneAnimation::RunFWD) {
	//
	//}

}
// 死亡した時に呼ばれる
void EnemyDrone::OnDead()
{
	float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMLoadFloat3(&position))));
	float volume = 0.5f;
	if (length > 20.0f)
	{
		volume = (20.0f / length) * 0.5f;
	}
	else
	{
		volume = 0.5f;
	}
	// 死亡
	stateMachine->ChangeState(static_cast<int>(EnemyDrone::DroneState::ReDamage));
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, volume);

}

void EnemyDrone::OnDamaged()
{
	if (!barrierFlg || saveDamage >= 4) {
		stateMachine->ChangeState(static_cast<int>(EnemyDrone::DroneState::ReDamage));
	}
	barrierHP -= (--saveDamage);
	if (barrierHP < 0 && barrierFlg) {
		barrierFlg = false;
		ParticleSprite* particleSprite = new ParticleSprite(GetEfPos(), { NULL,NULL,NULL }, ParticleSprite::ParticleTriangle, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Flame), 5000, 1.5f, 0.2f);
	}
	if (tutorialflag)health = maxHealth;
}
void EnemyDrone::BeemShoat()
{
	if (coolTime <= 0.0f)
	{
		// 前方向
		float h = 0;
		// 発射
		
		h = 0.4f;
		ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
		projectile->TurretLaunch(beem, h, 2.5 - 0.5, position, targetPosition, angle.y, Type::Beem, (int)EffectTexAll::EfTexAll::Distortion, 1, 1, 0.5f);
		
		coolTime = 3.0f;
	}
}

void EnemyDrone::TargetUpdate() {
	if (SearchTrap() || rootNo >= MAX_ROOT_POINT) {
		switch (targetNo)
		{
		case BaseTarget:
			targetPosition = Base::Instance().GetPosition();
			break;
		case TrapTarget:
			break;
		default:
			break;
		}

	}
	else
	{

		targetPosition = rootPoint[rootNo];

	}

}


void EnemyDrone::CollisionBeemVSTrap()
{
	DirectX::XMFLOAT3 outPosition;
	int projectileCount = objectManager.GetObjectCount();

	for (int i = 0; i < projectileCount; ++i)
	{
		Object* object = objectManager.GetObjectW(i);
		int count = TrapManager::Instance().GetTrapCount();
		for (int l = 0; l < count; l++)
		{
			Trap* trap = TrapManager::Instance().GetTrap(l);
			if (!trap->GetIsActive())
			{
				continue;
			}
			//タレットとデコイ以外はスルー
			if (trap->GetType() != Trap::TrapType::TrapTurret)
			{
				continue;
			}
			//衝突処理
			if (Collision::IntersectSphereVsCylinder(
				object->GetPosition(),
				object->GetRadius(),
				trap->GetPosition(),
				trap->GetRadius(),
				trap->GetHeight(),
				outPosition))
			{
				trap->InputDamage(10);

				if (object->GetType() == Type::Beem) {
					ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::BlueThader), 3, 0.5, { NULL,NULL,2,1 });
					//ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 200, 1.5, 0, true, 0.015, 0.06, { 1,0,0,1 });
					continue;
				}
			}
		}
	}
}
void EnemyDrone::CollisionBeemVSBase()
{

	DirectX::XMFLOAT3 outPosition;
	int projectileCount = objectManager.GetObjectCount();

	for (int i = 0; i < projectileCount; ++i)
	{
		Object* object = objectManager.GetObjectW(i);
		int count = TrapManager::Instance().GetTrapCount();

		//衝突処理
		if (Collision::IntersectSphereVsCylinder(
			object->GetPosition(),
			object->GetRadius(),
			Base::Instance().GetPosition(),
			Base::Instance().GetRadius(),
			Base::Instance().GetHeight(),
			outPosition))
		{
			Base::Instance().InputDamage(10);

			if (object->GetType() == Type::Beem) {
				ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::BlueThader), 3, 0.5, { NULL,NULL,2,1 });
				//ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 200, 1.5, 0, true, 0.015, 0.06, { 1,0,0,1 });
				continue;
			}
		}

	}
}
void EnemyDrone::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{

	//if (health <= 0) {
	//	return;
	//}
	if (renderflag)shader->Draw(dc, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { 1.0f, adjustMetalness, adjustSmoothness, 1.0f });
}
void EnemyDrone::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
	objectManager.Render(immediate_context.Get(), shader);
}

void EnemyDrone::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 攻撃範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

	// ターゲット位置をデバッグ球描画
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	if (attackFlg) {
		debugRenderer->DrawSphere(nodePosition, 0.5, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	// 索敵範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

// プレイヤーの周りを回る
void EnemyDrone::BattleMove(bool leftflag, float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	DirectX::XMFLOAT2 dir = ForwardToTarget();
	// 移動処理
	if (leftflag) {
		MoveInput(-dir.y, dir.x, moveSpeed * speedRate);
	}
	else
	{
		MoveInput(dir.y, -dir.x, moveSpeed * speedRate);
	}

	Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);

}

void EnemyDrone::FacePlayer(float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	DirectX::XMFLOAT2 dir = ForwardToTarget();
	//プレイヤーに向く
	Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);

}

void EnemyDrone::MoveToTarget(float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	// 移動処理
	DirectX::XMFLOAT2 dir;
	MoveInput(vx, vz, moveSpeed * speedRate);


	{
		//const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
		dir = ForwardToTarget();
		Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);
	}
}

bool EnemyDrone::SearchTrap()
{

	int count = TrapManager::Instance().GetTrapCount();
	float dist = searchRange;
	for (int i = 0; i < count; i++)
	{
		Trap* trap = TrapManager::Instance().GetTrap(i);
		if (trap->GetHologramTimer() < 1.0f)
		{
			continue;
		}
		//タレット以外はスルー
		if (trap->GetType() != Trap::TrapType::TrapTurret)//&& trap->GetType() != Trap::TrapType::TrapDecoy)
		{
			continue;
		}
		//HPが０ならスルー
		if (trap->GetHealth() <= 0)
		{
			continue;
		}
		float vx = trap->GetPosition().x - position.x;
		float vz = trap->GetPosition().z - position.z;
		float d = sqrtf(vx * vx + vz * vz);
		if (d < dist)
		{
			// 単位ベクトル化
			vx /= d;
			vz /= d;
			// 方向ベクトル化
			float frontX = sinf(angle.y);
			float frontZ = cosf(angle.y);
			// 2つのベクトルの内積値で前後判定
			float dot = (frontX * vx) + (frontZ * vz);
			if (dot > 0.0f)
			{
				targetNo = EnemyTarget::TrapTarget;
				dist = d;
				targetPosition = trap->GetPosition();

				return true;
			}
		}
	}
	targetNo = 0;
	return false;
}
//プレイヤーへの方向を取得する
DirectX::XMFLOAT2 EnemyDrone::ForwardToPlayer() {

	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	return { vx,vz };
}
DirectX::XMFLOAT2 EnemyDrone::ForwardToTarget() {
	DirectX::XMFLOAT3 basePosition = targetPosition;
	//switch (targetNo)
	//{
	//case BaseTarget:
	//	basePosition = Base::Instance().GetPos();
	//	break;
	//case PlayerTarget:
	//	basePosition = Player::Instance().GetPosition();
	//	break;
	//}
	float vx = basePosition.x - position.x;
	float vz = basePosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	return { vx,vz };
}
DirectX::XMFLOAT3 EnemyDrone::SearchNodePos(const char* nodeName) {
	Animation::Keyframe::node* leftHandBone = model->FindNode(nodeName);
	XMStoreFloat4x4(&world, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));
	DirectX::XMFLOAT3 node;
	// ノード位置取得
	node.x = world._41;
	node.y = world._42;
	node.z = world._43;
	return node;
}
void EnemyDrone::ReMove()
{
	activeflag = true;
	health = maxHealth;
	DirectX::XMFLOAT2 pos = { 155 - float(rand() % 95), 40 };
	glitchIntensity = 0;
	// ホログラムシェーダー情報初期化
	HologramShaderDataInitialize(minHeight, maxHeight);
	deathTimer = 0;
	position = DirectX::XMFLOAT3(pos.x, 0.0f, pos.y);
	SetTerritory(position, 10.0f);
	reMoveflag = false;
	rootNo = 0;
	stateMachine->SetState(static_cast<int>(DroneState::Search));
}

void EnemyDrone::DrawDebugGUI()
{
	//トランスフォーム
	if (ImGui::CollapsingHeader("EnemyDrone", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button(u8"Start"))
		{
			// ホログラムシェーダー情報初期化
			HologramShaderDataInitialize(minHeight, maxHeight);
		}
		if (ImGui::Button(u8"Death"))
		{
			health = 0;
			//stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
		}
		if (ImGui::TreeNode("PBR"))
		{
			ImGui::SliderFloat("adjustMetalness", &adjustMetalness, -1.0f, 1.0f);
			ImGui::SliderFloat("adjustSmoothness", &adjustSmoothness, -1.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Hologram"))
		{
			ImGui::SliderFloat("scanBorder", &scanBorder, -1600.0f, 1400.0f);
			ImGui::SliderFloat("glowBorder", &glowBorder, -1600.0f, 1400.0f);
			ImGui::SliderFloat("hologramBorder", &hologramBorder, -1600.0f, 1400.0f);
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

// TODO 05_05 メッセージを受信したときの処理を追加
bool EnemyDrone::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
		// 他のエネミーから呼ばれた時
	case MESSAGE_TYPE::MsgCallHelp:
		return true;
		// メタAIから攻撃権を与えられたとき
	case MESSAGE_TYPE::MsgGiveAttackRight:
		// TODO 05_05 攻撃フラグをtrueに設定
		if (Vector3::Probability(5)) {
			attackFireBallFlg = true;
		}
		if (SearchTrap() || rootNo >= MAX_ROOT_POINT) {
			attackFlg = true;
		}
		return true;
	}
	return false;
}

