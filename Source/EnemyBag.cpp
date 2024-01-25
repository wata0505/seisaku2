#include "EnemyBag.h"
#include "Graphics.h"
#include "Mathf.h"
#include "player.h"
#include "StateDerived.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ParticleSprite.h"
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
#include"EnemyManager.h"

// コンストラクタ
EnemyBag::EnemyBag(bool tutorial, int stag,int enemyType)
{
	model = std::make_unique<Model>(".\\resources\\enemy\\enemy.fbx", true);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_taikiandmigration.fbx",0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_attack1.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_attack2.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_attack3.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_damage.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_Die.fbx", 0);
	model->ModelSerialize(".\\resources\\enemy\\enemy.fbx");
	//model->ModelCreate(".\\resources\\Slime\\Slime.fbx");
	model->ModelRegister(".\\resources\\enemy\\enemy.fbx", "Texture\\all_low_lambert1.tif", true);
	adjustSmoothness = 1.0f;
	switch (enemyType)
	{
	   case 0:
		   adjustMetalness = 0.0f;
	   	   //model->ModelRegister(".\\resources\\enemy\\enemy.fbx", "Texture\\all_low_lambert1.tif", false);
	   	   break;
	   case 1:
		   adjustMetalness = 1.0f;
		   //model->ModelRegister(".\\resources\\enemy\\enemy.fbx", "TextureRed\\all_low_lambert1.tif",false);
		   break;
	   case 2:
		   adjustMetalness = -0.5f;
		   //model->ModelRegister(".\\resources\\enemy\\enemy.fbx", "TextureGreen\\all_low_lambert1.tif",false);
		   break;	
	}
	colorType = enemyType;
	// モデルが大きいのでスケーリング
	maxHealth = 5;
	health = maxHealth;
	radius = 0.5f;
	height = 2.0f;
	activeflag = true;
	isActiveStart = false;
	
	//StateMachineを生成し、階層型ステートマシンに対応するように登録ステートを変更していく。
	stateMachine = std::make_unique<StateMachine<EnemyBag>>();
	// ステートマシンに1層目のステートを登録(BattleStateも同様の方法で各自追加してください)
	stateMachine->RegisterState(new BagSearchState(this));
	stateMachine->RegisterState(new BagBattleState(this));
	stateMachine->RegisterState(new BagRecieveState(this));
	stateMachine->RegisterState(new BagReDamageState(this));
	// 各親ステートにサブステートを登録(WanderState以外の2層目のステートも同様の方法で各自追加してください。)
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Search), new BagWanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Search), new BagIdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagPursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagAttackState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagFireBallState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagStandbyState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagRoarState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagBackStepState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Recieve), new  BagCalledState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::ReDamage), new BagDamageState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::ReDamage), new BagDieState(this));
	// ステートをセット
	stateMachine->SetState(static_cast<int>(BagState::Search));
	mass = 2.0;
	UpdateTransform(0, 0);

	model->UpdateBufferData(transform);
	//renderdata = model->GetBufferData();

	se[(int)EnemyBagSE::Walk] = Audio::Instance().LoadAudioSource("resources\\Audio\\wolk2.wav");
	se[(int)EnemyBagSE::Run] = Audio::Instance().LoadAudioSource("resources\\Audio\\run3.wav");
	//se[(int)EnemyBagSE::hit] = Audio::Instance().LoadAudioSource("resources\\Audio\\pannti.wav");
	searchRange = 6.0f;

	attackRange = 2.5f;
	scale.x = scale.y = scale.z = 6.0f;
	territoryRange = 10.0f;
	angle.y = DirectX::XMConvertToRadians(180);
	tutorialflag = tutorial;
	eria = 100;
	targetNo = 0;
	stagNo = stag;
	float len = 20;
	if (stagNo == 2 ){
		if (rand() % 2 == 0) {
			stagNo = 1;
		}
	    len = 10;
	}
	for (int i = 0; i < MAX_ROOT_POINT; i++) {
		SetTerritory(rootPoint[stagNo][i], len);
		SetRandomTargetPosition();
		rootPoint[stagNo][i] = targetPosition;
	}
	enemyType = EnemyType::Bag;

	// ホログラムシェーダー情報初期化
	HologramShaderDataInitialize(-1600.0f, 1400.0f);
}

// デストラクタ
EnemyBag::~EnemyBag()
{
	// delete model;
}
//ルートモーション
void EnemyBag::RootMove() {

	DirectX::XMFLOAT2 dir = ForwardToPlayer();
	velocity.x =  dir.x * (model->GetRootPow() * rootSpeed);
	velocity.z =  dir.y * (model->GetRootPow() * rootSpeed);
}
float EnemyBag::MovePow() {
	
	return (velocity.x * velocity.x) + (velocity.z * velocity.z);
}
void EnemyBag::Update(float elapsedTime)
{
	
	timer += elapsedTime;
	if (stateMachine->GetStateNum() == static_cast<int>(EnemyBag::BagState::ReDamage))
	{
		lerpGlitchIntensity = 1.0f;
	}
	else
	{
		lerpGlitchIntensity = 0.0f;
	}
	glitchIntensity = Mathf::Lerp(glitchIntensity, lerpGlitchIntensity, elapsedTime * 20.0f);
	model->ShaderAdjustment(glitchScale, timer, maxHeight);

	//描画判定
	reMoveTimer -= elapsedTime;
	if (reMoveTimer < 0) {
		if (reMoveflag)ReMove();
	}
	if (!activeflag)return;
	UpdateRnderflag();
	if (colorType == 1) {
		elapsedTime *= 2;
	}
	if (colorType == 2) {
		if (health > 0 && health <= 5) {
			//health += 1;
			if (enemyTimer % 120 == 0) {
				EnemyManager& enemyManager = EnemyManager::Instance();
				//全ての敵と総当たりで衝突処理
				int enemyCount = enemyManager.GetEnemyCount();
				for (int i = 0; i < enemyCount; i++) {
					Enemy* enemy = enemyManager.GetEnemy(i);
					//衝突処理
					DirectX::XMFLOAT3 outPosition;
					if (Collision::IntersectCylinderVsCylinder(
						enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
						position, 3, height,
						outPosition
					)) {
						enemy->RecoverHealth(1);
					}
				}
			}
		}
		ParticleSystem::Instance().CorruptionEffect(position,id);
	}
	float ElapsedTime = elapsedTime;
	
	if (Player::Instance().GetQuickflag()) 
	{
		ElapsedTime *= 0.5f;
	}
	if (health <= 0) {
		if (!model->IsPlayAnimation()) {
			//死亡モーションが終わったデリート
			//Destroy();
			activeflag = false;
			renderflag = false;
			//position.y = -2000;
			TrapManager::Instance().SetTrapPoint(TrapManager::Instance().GetTrapPoint() + 1);
		}
		ElapsedTime *= 0.3f;
		UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
		model->UpdateAnimation(ElapsedTime,"pelvis");
		// モデル行列更新
		model->UpdateBufferData(transform);
		//renderdata = model->GetBufferData();

		hologramTimer = 0.0f;
		UpdateHologramShader(elapsedTime);
		return;
	}
	
	// ホログラムシェーダー実行中フラグが付いていれば
	if (!isActiveStart)
	{
		// ホログラムシェーダー更新処理
		isActiveStart = UpdateHologramShader(elapsedTime);
		ElapsedTime = 0.0f;
	}
	
	//ヒットストップ
	if (Player::Instance().GetAttackHitflag()) ElapsedTime *= hitStop;
	TargetUpdate();
	//stateMachine->SetState(static_cast<int>(BagState::Search));
	//ステートマシン更新
	stateMachine->Update(ElapsedTime);

	// 速力処理更新
	UpdateVelocity(ElapsedTime);

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列更新
	UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);

	CollisionFireBallVSPlayer();
	model->UpdateAnimation(ElapsedTime,"pelvis",renderflag);
	objectManager.Update(elapsedTime);
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
void EnemyBag::SeUpdate(float elapsedTime) {
	//if (model->GetCurrentAnimationIndex() == (int)EnemyBagAnimation::RunFWD) {
	//
	//}

}
// 死亡した時に呼ばれる
void EnemyBag::OnDead()
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
	stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, volume);
	
}

void EnemyBag::OnDamaged()
{
	if (!barrierFlg || saveDamage >= 4) {
		stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
	}
	barrierHP -= (--saveDamage);
	if (barrierHP < 0 && barrierFlg) {
		barrierFlg = false;
		ParticleSprite* particleSprite = new ParticleSprite(GetEfPos(), { NULL,NULL,NULL }, ParticleSprite::ParticleTriangle, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Flame), 5000, 1.5f, 0.2f);
	}
	if(tutorialflag)health = maxHealth;
}
void EnemyBag::FireBallShoat()
{
	nodePosition = SearchNodePos("koshi");
    
	// ターゲット方向への進行ベクトルを算出
	//DirectX::XMFLOAT3 dir;
	FireBall* projectile = new FireBall(&objectManager, 0.05f);
	DirectX::XMFLOAT3 dir{};
	dir.x = sinf(angle.y);
	dir.y = 0;
	dir.z = cosf(angle.y);
	//方向、位置
	projectile->Launch(dir, nodePosition, angle.y);
}

void EnemyBag::TargetUpdate() {
	if (SearchTrap()||SearchPlayer()|| rootNo >= MAX_ROOT_POINT) {
		switch (targetNo)
		{
		case BaseTarget:
			targetPosition = Base::Instance().GetPosition();
			break;
		case PlayerTarget:
			targetPosition = Player::Instance().GetPosition();
			break;
		case TrapTarget:
			break;
		default:
			break;
		}
	}
	else
	{
		targetPosition = rootPoint[stagNo][rootNo];
	}
	
}


void EnemyBag::CollisionFireBallVSPlayer()
{
	int projectileCount = objectManager.GetObjectCount();

	for (int i = 0; i < projectileCount; ++i)
	{

		Object* object = objectManager.GetObjectW(i);
		Player& player = Player::Instance();
		if (!player.GetRenderflag())return;
		//衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectSphereVsCylinder(
			object->GetPosition(),
			object->GetRadius(),
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{

			if (player.GetGuardflag() && player.GetInvincibleTimer() < 0) {
				//ParticleSprite* particleSprite = new ParticleSprite(player.GetEfPos(), { NULL,NULL,NULL }, ParticleSprite::ParticleSquare, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::BlackThunder, 3, 0.3);
				player.SetCounterflag(true);
			}
			if (player.GetCounterflag())return;
			if (player.GetState() == Player::State::Guard && player.GetInvincibleTimer() < 0) {
				ParticleSprite* particleSprite = new ParticleSprite(nodePosition, { NULL,NULL,NULL }, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::Impact, static_cast<int>(efMax), efLife);
				player.PlayerKnockBack(this->position, player.GetPosition(), 20, 0, 1);
				player.SetInvincibleTime(object->GetInvincibleTime());
				return;
			}
			// ダメージを与える
			if (player.ApplyDamage(object->GetDamage(), object->GetInvincibleTime()))
			{

				player.PlayerKnockBack(this->position, player.GetPosition(), 20, 0, 1);
				//EffectAll::Instance().swordEffect->Play(player.GetPosition(), 0.2);
				//弾丸破棄
				AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hit)->Play(false, static_cast<float>(SE));
			}
		}

	}
}
void EnemyBag::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{

	//if (health <= 0) {
	//	return;
	//}
	if (renderflag)shader->Draw(dc, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { 1.0f, adjustMetalness, adjustSmoothness, 1.0f });
}
void EnemyBag::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
	//objectManager.Render(immediate_context.Get(), shader);
}

void EnemyBag::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 縄張り範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// ターゲット位置をデバッグ球描画
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	if (attackFlg) {
		debugRenderer->DrawSphere(nodePosition, 0.5, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	// 索敵範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

// プレイヤーの周りを回る
void EnemyBag::BattleMove(bool leftflag,float elapsedTime, float speedRate)
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

void EnemyBag::FacePlayer(float elapsedTime, float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	DirectX::XMFLOAT2 dir = ForwardToTarget();
	//プレイヤーに向く
	Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);

}

void EnemyBag::MoveToTarget(float elapsedTime, float speedRate)
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
	if (!SearchPlayer()) {
		dir = ForwardToTarget();
		Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);
	}
	else
	{
		//const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
		dir = ForwardToTarget();
		Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);
	}
}
// 徘徊ステートへ遷移
bool EnemyBag::SearchPlayer()
{
	// プレイヤーとの高低差を考慮して3Dで距離判定をする
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	//const DirectX::XMFLOAT3& playerPosition = Base::Instance().GetPos();
	DirectX::XMFLOAT3 vec = Vector3::Subset(playerPosition, position);
	float dist = Vector3::Lenght(vec);
	//距離で視界に入っているか
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vec.x * vec.x + vec.z * vec.z);
		// 単位ベクトル化
		vec.x /= distXZ;
		vec.z /= distXZ;
		// 方向ベクトル化
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2つのベクトルの内積値で前後判定
		float dot = (frontX * vec.x) + (frontZ * vec.z);
		if (dot > 0.0f)
		{
			targetNo = 1;
			return true;
		}
	}
	targetNo = 0;
	return false;
}
bool EnemyBag::SearchTrap()
{

	int count = TrapManager::Instance().GetTrapCount();
	float dist = searchRange + 10;
	for (int i = 0; i < count; i++)
	{
		Trap* trap = TrapManager::Instance().GetTrap(i);
		if (trap->GetHologramTimer() < 1.0f)
		{
			continue;
		}
		//タレットとデコイ以外はスルー
		if (trap->GetType() != Trap::TrapType::TrapDecoy)
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

				
			}
		}
	}
	if (dist != searchRange + 10)//敵が見つかっていたら
	{
		return true;
	}
	else
	{
		targetNo = 0;
		return false;
	}
}
//プレイヤーへの方向を取得する
DirectX::XMFLOAT2 EnemyBag::ForwardToPlayer() {

	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	return {vx,vz};
}
DirectX::XMFLOAT2 EnemyBag::ForwardToTarget() {
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
DirectX::XMFLOAT3 EnemyBag::SearchNodePos(const char* nodeName) {
	Animation::Keyframe::node* leftHandBone = model->FindNode(nodeName);
	XMStoreFloat4x4(&world, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));
	DirectX::XMFLOAT3 node;
	// ノード位置取得
	node.x = world._41;
	node.y = world._42;
	node.z = world._43;
	return node;
}
void EnemyBag::ReMove()
{
	activeflag = true;
	health = maxHealth;
	DirectX::XMFLOAT2 pos = { 105 - float(rand() % 50), 40 };
	glitchIntensity = 0;
	// ホログラムシェーダー情報初期化
	HologramShaderDataInitialize(minHeight, maxHeight);
	deathTimer = 0;	
	position = DirectX::XMFLOAT3(pos.x, 0.0f, pos.y);
	SetTerritory(position, 10.0f);
	reMoveflag = false;
	rootNo = 0;
	model->PlayAnimation(EnemyBagAnimation::WalkFWD, false, 1.0f);
	model->UpdateAnimation(1.0f, "pelvis");
	stateMachine->SetState(static_cast<int>(BagState::Search));
}

void EnemyBag::DrawDebugGUI()
{
	//トランスフォーム
	if (ImGui::CollapsingHeader("EnemyBag", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button(u8"Start"))
		{
			// ホログラムシェーダー情報初期化
			HologramShaderDataInitialize(minHeight, maxHeight);
		}
		if (ImGui::Button(u8"Death"))
		{
			health = 0;
			stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
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
			//ImGui::SliderFloat("glitchSpeed", &glitchSpeed, 0.0f, 10.0f);
			ImGui::SliderFloat("glitchSpeed", &glitchSpeed, 1.0f, 50.0f);
			ImGui::SliderFloat("glitchIntensity", &glitchIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("glitchScale", &glitchScale, 1.0f, 50.0f);
			ImGui::TreePop();
		}
	}
}

// TODO 05_05 メッセージを受信したときの処理を追加
bool EnemyBag::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
		// 他のエネミーから呼ばれた時
	case MESSAGE_TYPE::MsgCallHelp:
		if (!SearchPlayer()&& health > 0)
		{// TODO 05_05 プレイヤーを見つけていないときに1層目ステートをReceiveに変更する
			stateMachine->ChangeState(static_cast<int>(BagState::Recieve));
		}
		return true;
		// メタAIから攻撃権を与えられたとき
	case MESSAGE_TYPE::MsgGiveAttackRight:
		// TODO 05_05 攻撃フラグをtrueに設定
		if (Vector3::Probability(5)) {
			attackFireBallFlg = true;
		}
		if (SearchPlayer() || SearchTrap() || rootNo >= MAX_ROOT_POINT) {
			attackFlg = true;
		}
		return true;
	}
	return false;
}