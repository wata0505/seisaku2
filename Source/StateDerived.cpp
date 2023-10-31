#include "StateDerived.h"
#include "player.h"
#include "Mathf.h"
#include "Calculation.h"
#include "ParticleSprite.h"
#include "EffectTexAll.h"
#include "Base.h"

// TODO 02_03 Stateを基底クラスとして各種Stateクラスを用意する。
// Wanderはサンプルとしてすでに記述済み
// 各種Enter関数の内容は各Transition○○State関数を参考に
// 各種Execute関数の内容は各Update○○State関数を参考に

// 徘徊ステートに入った時のメソッド
void BagWanderState::Enter()
{
	owner->SetRandomTargetPosition();
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	owner->PlaySe((int)EnemyBag::EnemyBagSE::Walk,true);
}

// 徘徊ステートで実行するメソッド
void BagWanderState::Execute(float elapsedTime)
{
	// 目的地点までのXZ平面での距離判定
	DirectX::XMFLOAT3 position = owner->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	DirectX::XMFLOAT3 targetPosition = Base::Instance().GetPos();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// 待機ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Search::Idle));
	}
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);
	// プレイヤー索敵
	if (owner->SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Roar));
	}
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));

}

// 徘徊ステートから出ていくときのメソッド
void BagWanderState::Exit()
{
	owner->StopSe((int)EnemyBag::EnemyBagSE::Walk);
}
//待機ステートに入った時のメソッド
void BagIdleState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
}

// 待機ステートで実行するメソッド
void BagIdleState::Execute(float elapsedTime)
{
	// タイマー処理
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	if (owner->GetTutorialflag())return;
	if (owner->GetStateTimer() < 0.0f) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Search::Wander));
	}
	if (owner->SearchPlayer()) {
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
	}
}

// 待機ステートから出ていくときのメソッド
void BagIdleState::Exit()
{

}
//咆哮ステートに入った時のメソッド
void BagRoarState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), false);
	owner->PlaySe((int)EnemyBag::EnemyBagSE::Roar, false);
}

// 咆哮ステートで実行するメソッド
void BagRoarState::Execute(float elapsedTime)
{
	// タイマー処理
	if (!owner->GetModel()->IsPlayAnimation()) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
}

// 咆哮ステートから出ていくときのメソッド
void BagRoarState::Exit()
{

}
// 追尾ステートで実行するメソッド
void BagPursuitState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	DirectX::XMFLOAT2 dir = owner->ForwardToBase();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = Base::Instance().GetPos();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx  + vz * vz);
	vx /= dist;
	vz /= dist;

	float dot = (dir.x * vx) + (dir.y * vz);
	//目的地が自分より前方向なら
	if (1.0 - dot < 0.2) {
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
		owner->SetMoveRate(1.5);
		owner->PlaySe((int)EnemyBag::EnemyBagSE::Run, true);
	}
	else
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
		owner->SetMoveRate(0.5);
		owner->PlaySe((int)EnemyBag::EnemyBagSE::Walk, true);
	}
	Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgCallHelp);
}

// 追尾ステートで実行するメソッド
void BagPursuitState::Execute(float elapsedTime)
{
	// タイマー処理
	// 目標地点をプレイヤー位置に設定
	//owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	if (owner->GetAttackFlg())owner->SetBattleRange(NULL);
	DirectX::XMFLOAT2 dir = owner->ForwardToBase();
	DirectX::XMFLOAT3 traget = Base::Instance().GetPos();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, owner->GetMoveRate());

	// タイマー処理
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);

	// TODO 03 
	// 追跡時間が経過したとき待機ステートへ遷移しなさい
	if (owner->GetStateTimer() < 0.0f && owner->GetAttackFlg()) {
		//攻撃フラグをfalseに設定
		owner->SetAttackFlg(false);
		// エネミーからメタAIへ MsgChangeAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
		owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	}
	DirectX::XMFLOAT3 vec = Vector3::Subset(owner->GetTargetPosition(), owner->GetPosition());
	float dist = Vector3::Lenght(vec);
	if (Vector3::FrameInput(owner->GetEnemyTimer()) && owner->GetAttackFlg()) {
		owner->SetAttackFlg(false);
		// エネミーからメタAIへ MsgChangeAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
	}
	if (dist < 50 && owner->GetAttackFlg() && owner->GetAttackFireBallFlg()) {
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::FireBall));
	}
	// 攻撃範囲に入ったとき攻撃ステートへ遷移しなさい
	if (dist < owner->GetAttackRange() && owner->GetAttackFlg()) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Attack));
	}
	else if (dist < owner->GetAttackRange()) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Standby));
	}

}

// 追尾ステートから出ていくときのメソッド
void BagPursuitState::Exit()
{
	owner->SetMoveRate(1.0);
	owner->StopSe((int)EnemyBag::EnemyBagSE::Walk);
	owner->StopSe((int)EnemyBag::EnemyBagSE::Run);
}
void BagAttackState::Enter()
{
	// 攻撃権があればモーション再生開始
	if (owner->GetAttackFlg())
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(owner->GetAttackAnim(rand() % owner->GetAttackAnimMax())), false);
	}
	owner->SetDamegeRadius(0.2);
	ParticleSprite* particleSprite = new ParticleSprite(owner->SearchNodePos("atama"), { NULL,NULL,NULL }, ParticleSprite::ParticleBurst, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::BlackThunder, 1, 0.3);
}


//アタックステートで実行するメソッド
void BagAttackState::Execute(float elapsedTime)
{

	// 攻撃権があるとき
	if (owner->GetAttackFlg())
	{
		owner->CollisionNodeVsPlayer("R_arm",owner->GetDamegeRadius(), { 10,0 },1);
		owner->CollisionNodeVsBase("R_arm", owner->GetDamegeRadius(), { 10,0 }, 1);
		// 攻撃モーションが終わっていれば待機へ遷移
		if (!owner->GetModel()->IsPlayAnimation())
		{

			owner->RandBattleRange();
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
			//if (Vector3::Probability(2)) {//確率でバックステップ
			//	owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::BackStep));
			//}
		}
	}
	else
	{
		
		// 攻撃権がないときステート変更
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	
	}


}

// アタックステートから出ていくときのメソッド
void BagAttackState::Exit()
{
	if (owner->GetAttackFlg())
	{
		// TODO 05_07 攻撃が終わったとき攻撃権の放棄
		//攻撃フラグをfalseに設定
		owner->SetAttackFlg(false);
		// エネミーからメタAIへ MsgChangeAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
	}

}

void BagFireBallState::Enter()
{
	// 攻撃権があればモーション再生開始
	if (owner->GetAttackFlg())
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), false);
	}
	ParticleSprite* particleSprite = new ParticleSprite(owner->SearchNodePos("atama"), { NULL,NULL,NULL }, ParticleSprite::ParticleBurst, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::BlackThunder, 1, 0.3);
}


//アタックステートで実行するメソッド
void BagFireBallState::Execute(float elapsedTime)
{
	owner->FacePlayer(elapsedTime,1.0);

	// 攻撃権があるとき
	if (owner->GetAttackFlg())
	{
		float animationTime = owner->GetModel()->GetCurrentAnimationSeconds();
		bool attackCollisionFlag = animationTime >= 0.68 && animationTime <= 0.69;
		if (attackCollisionFlag)
		{
			owner->FireBallShoat();
		}
		// 攻撃モーションが終わっていれば待機へ遷移
		if (animationTime >= 0.8)
		{
			owner->RandBattleRange();
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
		}
	}
	else
	{

		// 攻撃権がないときステート変更
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));

	}


}

// アタックステートから出ていくときのメソッド
void BagFireBallState::Exit()
{
	owner->SetAttackFireBallFlg(false);
	if (owner->GetAttackFlg())
	{
		// TODO 05_07 攻撃が終わったとき攻撃権の放棄
		//攻撃フラグをfalseに設定
		owner->SetAttackFlg(false);
		// エネミーからメタAIへ MsgChangeAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
	}

}
// TODO 03_03 HierarchicalStateを継承した親ステートの
// BattleStateクラスのメンバ関数の定義を行いなさい。
// 書き方の例としてSearchStateクラスは記述しておきます。
// サーチステートデストラクタ
BagSearchState::~BagSearchState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// サーチステートに入った時のメソッド
void BagSearchState::Enter()
{
	SetSubState(static_cast<int>(EnemyBag::Search::Idle));
}
// サーチステートで実行するメソッド
void BagSearchState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// サーチステートから出ていくときのメソッド
void BagSearchState::Exit()
{
}


// バトルステートデストラクタ
BagBattleState::~BagBattleState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// バトルステートに入った時のメソッド
void BagBattleState::Enter()
{
		SetSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
}
// バトルステートで実行するメソッド
void BagBattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// バトルステートから出ていくときのメソッド
void BagBattleState::Exit()
{
}
// TODO 05_03 メタAIからメッセージを受信したときに呼ばれるステートを追加
// デストラクタ
BagRecieveState::~BagRecieveState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// データ受信したときのステート
void BagRecieveState::Enter()
{
	// 初期ステートを設定
	SetSubState(static_cast<int>(EnemyBag::Recieve::Called));
}

// サーチステートで実行するメソッド
void BagRecieveState::Execute(float elapsedTime)
{
	// 子ステート実行
	subState->Execute(elapsedTime);
	// 
	if (owner->SearchPlayer())
	{
		// Battleステートへ遷移
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
}
// サーチステートから出ていくときのメソッド
void BagRecieveState::Exit()
{
}
// TODO 05_03 他のエネミーから呼ばれたときのステートを追加
void BagCalledState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	owner->SetStateTimer(5.0f);
}
// コールドステートで実行するメソッド
void BagCalledState::Execute(float elapsedTime)
{
	// タイマー処理
	float timer = owner->GetStateTimer();
	timer -= elapsedTime;
	owner->SetStateTimer(timer);

	if (timer < 0.0f)
	{
		// 徘徊ステートへ遷移
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
	

	}
	// 対象をプレイヤー地点に設定
	//owner->SetTargetPosition(Player::Instance().GetPosition());
	owner->SetTargetPosition(Base::Instance().GetPos());
	owner->MoveToTarget(elapsedTime, 1.0f);
}
// コールドステートから出ていくときのメソッド
void BagCalledState::Exit()
{
}

// デストラクタ
BagReDamageState::~BagReDamageState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// ダメージ入力された時のステート
void BagReDamageState::Enter()
{
	owner->SetGravity(-1.0f);
	// 初期ステートを設定
	SetSubState(static_cast<int>(EnemyBag::ReDamage::Damege));
	if (owner->GetHealth() < 1) {
		SetSubState(static_cast<int>(EnemyBag::ReDamage::Die));
	}
}

// ダメージ入力ステートで実行するメソッド
void BagReDamageState::Execute(float elapsedTime)
{
	// 子ステート実行
	subState->Execute(elapsedTime);

}
// ダメージ入力ステートから出ていくときのメソッド

void BagReDamageState::Exit()
{
}
//ダメージステートに入った時のメソッド
void BagDamageState::Enter()
{
	{
		owner->GetModel()->PlayAnimation(EnemyBag::Damage, false);
	}
}


// ダメージステートで実行するメソッド
void BagDamageState::Execute(float elapsedTime)
{

	// 攻撃権があるとき
	owner->FacePlayer(elapsedTime,2.0f);
		// 攻撃モーションが終わっていれば待機へ遷移
	if (!owner->GetModel()->IsPlayAnimation())
	{
		
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Battle));
		if (owner->GetTutorialflag()) {
			owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyBag::BagState::Search));
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Search::Idle));
		}
	}

}

// ダメージステートから出ていくときのメソッド
void BagDamageState::Exit()
{

}

//死亡ステートに入った時のメソッド
void BagDieState::Enter()
{
	// 
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::Die1), false);
}


// 死亡ステートで実行するメソッド
void BagDieState::Execute(float elapsedTime)
{
	if (!owner->GetModel()->IsPlayAnimation()) {
		//owner->Destroy();
	}
}

// 死亡ステートから出ていくときのメソッド
void BagDieState::Exit()
{

}
// 戦闘待機ステートに入った時のメソッド
void BagStandbyState::Enter()
{
	if (!owner->GetAttackFlg())
	{
		// 攻撃権をメタAIから求める
		// エネミーからメタAIへ MsgAskAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
	owner->SetStateTimer(Mathf::RandomRange(2.0f, 3.0f));
	//if (rand() % 2 == 0) {
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	//	rightflag = true;
	//}
	//else
	//{
	//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), true);
	//	rightflag = false;
	//}
	//owner->PlaySe((int)EnemyBag::EnemyBagSE::Walk, true);
	owner->SetMoveRate(0.5);
	//owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBagAnimation::IdleBattle), false);
}
// 戦闘待機ステートで実行するメソッド
void BagStandbyState::Execute(float elapsedTime)
{
	if (!owner->GetAttackFlg())
	{
		// 攻撃権をメタAIから求める
		// エネミーからメタAIへ MsgAskAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	// 攻撃権があるとき
	if (owner->GetAttackFlg())
	{
		// 攻撃権があるときステート変更
		owner->SetBattleRange(0);
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
	owner->BattleMove(rightflag, elapsedTime, owner->GetMoveRate());
	// 目標地点をプレイヤー位置に設定
	//DirectX::XMFLOAT2 dir = owner->ForwardToPlayer();
	DirectX::XMFLOAT2 dir = owner->ForwardToBase();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = Base::Instance().GetPos();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist > owner->GetAttackRange()&& !owner->GetAttackFlg())
	{
		// 攻撃範囲から出たら追跡ステートへ遷移
		//owner->SetBattleRange(rand() % 6 + 1);
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));

	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		if (Vector3::Probability(2) && dist < owner->GetBackStepRange()) {
			//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::BackStep));
		}
	}
		
}
// 戦闘待機ステートから出ていくときのメソッド
void BagStandbyState::Exit()
{
	owner->StopSe((int)EnemyBag::EnemyBagSE::Walk);
	owner->SetMoveRate(1.0);
}
// 戦闘待機ステートに入った時のメソッド
void BagBackStepState::Enter()
{
	
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyBag::WalkFWD), false);
	
}
// 戦闘待機ステートで実行するメソッド
void BagBackStepState::Execute(float elapsedTime)
{
	owner->RootMove();
	//モーションが終わったら
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyBag::Battle::Pursuit));
	}
	
}
// 戦闘待機ステートから出ていくときのメソッド
void BagBackStepState::Exit()
{
	owner->SetMoveRate(1.0);
}
