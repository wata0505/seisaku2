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
// コンストラクタ
EnemyBag::EnemyBag(bool tutorial)
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
	model->ModelRegister(".\\resources\\enemy\\enemy.fbx");
	// モデルが大きいのでスケーリング
	maxHealth = 30;
	health = maxHealth;
	radius = 0.5f;
	height = 2.0f;
	activeflag = true;
	
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

	model->UpdateBufferDara(transform);
	//renderdata = model->GetBufferData();

	se[(int)EnemyBagSE::Walk] = Audio::Instance().LoadAudioSource("resources\\Audio\\wolk2.wav");
	se[(int)EnemyBagSE::Run] = Audio::Instance().LoadAudioSource("resources\\Audio\\run3.wav");
	se[(int)EnemyBagSE::Roar] = Audio::Instance().LoadAudioSource("resources\\Audio\\bag1.wav");
	//se[(int)EnemyBagSE::hit] = Audio::Instance().LoadAudioSource("resources\\Audio\\pannti.wav");
	searchRange = 20.0f;

	attackRange = 2.5f;
	scale.x = scale.y = scale.z = 4.0f;
	territoryRange = 10.0f;
	angle.y = DirectX::XMConvertToRadians(180);
	tutorialflag = tutorial;
	eria = 100;
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
	//描画判定
	if (reMoveflag)ReMove();
	if (!activeflag)return;
	UpdateRnderflag();
	float ElapsedTime = elapsedTime;
	if (Player::Instance().GetQuickflag()) {
		ElapsedTime *= 0.5;
	}
	if (health <= 0) {
		if (!model->IsPlayAnimation()) {
			//死亡モーションが終わったデリート
			//Destroy();
			activeflag = false;
			renderflag = false;
			//position.y = -2000;
		}
		UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
		model->UpdateAnimation(ElapsedTime,"pelvis");
		// モデル行列更新
		model->UpdateBufferDara(transform);
		//renderdata = model->GetBufferData();
		return;
	}
	
	//ヒットストップ
	if (Player::Instance().GetAttackHitflag()) ElapsedTime *= hitStop;
	//ステートマシン更新
	stateMachine->Update(elapsedTime);



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
		model->UpdateBufferDara(transform);
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
	// 死亡
	stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, DOWNSE);
	
}

void EnemyBag::OnDamaged()
{
	if (!barrierFlg || saveDamage >= 4) {
		stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
	}
	barrierHP -= (--saveDamage) ;
	if (barrierHP < 0 && barrierFlg) {
		barrierFlg = false;
		ParticleSprite* particleSprite = new ParticleSprite(GetEfPos(), { NULL,NULL,NULL }, ParticleSprite::ParticleTriangle, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Flame), 5000, 1.5, 0.2);
	}
	if(tutorialflag)health = maxHealth;
}
void EnemyBag::FireBallShoat()
{
	nodePosition = SearchNodePos("koshi");
    
	// ターゲット方向への進行ベクトルを算出
	//DirectX::XMFLOAT3 dir;
	FireBall* projectile = new FireBall(&objectManager,0.05);
	DirectX::XMFLOAT3 dir{};
	dir.x = sinf(angle.y);
	dir.y = 0;
	dir.z = cosf(angle.y);
	//方向、位置
	projectile->Launch(dir, nodePosition, angle.y);
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
				ParticleSprite* particleSprite = new ParticleSprite(nodePosition, { NULL,NULL,NULL }, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::Impact, efMax, efLife);
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
				AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hit)->Play(false, SE);
			}
		}

	}
}
void EnemyBag::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{

	//if (health <= 0) {
	//	return;
	//}
	if(renderflag)shader->Draw(dc, model.get());
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
	DirectX::XMFLOAT2 dir = ForwardToPlayer();
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
	DirectX::XMFLOAT2 dir = ForwardToBase();
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
	MoveInput(vx, vz, moveSpeed * speedRate);
	if (!SearchPlayer()) {
		Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
	}
	else
	{
		//const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
		const DirectX::XMFLOAT3& playerPosition = Base::Instance().GetPos();
		float vx = playerPosition.x - position.x;
		float vz = playerPosition.z - position.z;
		float dist = sqrtf(vx * vx + vz * vz);
		vx /= dist;
		vz /= dist;
		Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
	}
}
// 徘徊ステートへ遷移
bool EnemyBag::SearchPlayer()
{
	// プレイヤーとの高低差を考慮して3Dで距離判定をする
	//const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	const DirectX::XMFLOAT3& playerPosition = Base::Instance().GetPos();
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
			return true;
		}
	}
	return false;
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
DirectX::XMFLOAT2 EnemyBag::ForwardToBase() {

	const DirectX::XMFLOAT3& basePosition = Base::Instance().GetPos();
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
	health = 30;
	float yaw = DirectX::XMConvertToRadians(rand() % 360);
	DirectX::XMFLOAT2 dir;
	dir.x = sinf(yaw);
	dir.y = cosf(yaw);
	int len = rand() % 100 + 5;
	position = DirectX::XMFLOAT3(dir.x * len, 0.0f, dir.y * len);
	SetTerritory(position, 10.0f);
	reMoveflag = false;
	stateMachine->SetState(static_cast<int>(BagState::Search));
}

// デバッグエネミー情報表示
//void EnemyBag::DrawDebugGUI()
//{
//
//	//std::string str = "";
//
//	// TODO 05 デバッグ表示用に各ステート名をstrに追加しなさい
//	//switch (state) {
//	//case State::Wander:
//	//	str = "Wander";
//	//	break;
//	//
//	//}
//	//switch (static_cast<State>(stateMachine->GetStateIndex())) {
//	//case State::Wander:
//	//	str = "Wander";
//	//	break;
//	//case State::Idle:
//	//	str = "Idle";
//	//	break;
//	//case State::Pursuit:
//	//	str = "Pursuit";
//	//	break;
//	//case State::Attack:
//	//	str = "Attack";
//	//	break;
//	//
//	//
//	//}
//	// TODO 03_07 デバッグ文字列表示の変更
//	std::string str = "";
//	std::string subStr = "";
//	// 現在のステート番号に合わせてデバッグ文字列をstrに格納
//	switch (static_cast<State>(stateMachine->GetStateIndex())) {
//	case State::Search:
//		str = "Search";
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Search::Wander))
//		{
//			subStr = "Wander";
//		}
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Search::Idle))
//		{
//			subStr = "Idle";
//		}
//		break;
//	case State::Battle:
//		str = "Battle";
//		// Battleステートの表示はSearchを参考に考えてください。
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Battle::Pursuit))
//		{
//			subStr = "Pursuit";
//		}
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Battle::Attack))
//		{
//			subStr = "Attack";
//		}
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Battle::Standby))
//		{
//			subStr = "Stanby";
//		}
//		break;
//	case State::Recieve:
//		str = "Recieve";
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Recieve::Called))
//		{
//			subStr = "Called";
//		}
//		break;
//
//	}
//
//
//
//	//トランスフォーム
//	if (ImGui::CollapsingHeader("EnemyBag", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		// 位置
//		ImGui::InputFloat3("Position", &position.x);
//		// 回転
//		DirectX::XMFLOAT3 a;
//		a.x = DirectX::XMConvertToDegrees(angle.x);
//		a.y = DirectX::XMConvertToDegrees(angle.y);
//		a.z = DirectX::XMConvertToDegrees(angle.z);
//		ImGui::InputFloat3("Angle", &a.x);
//		angle.x = DirectX::XMConvertToRadians(a.x);
//		angle.y = DirectX::XMConvertToRadians(a.y);
//		angle.z = DirectX::XMConvertToRadians(a.z);
//		// スケール
//		ImGui::InputFloat3("Scale", &scale.x);
//
//		ImGui::Text(u8"State　%s", str.c_str());
//		ImGui::Text(u8"SubState　%s", subStr.c_str());
//	}
//}
//void EnemyBag::UpdateVerticalMove(float elapsedTime) {
//	position.y = 0;
//}

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
		attackFlg = true;
		return true;
	}
	return false;
}