#include "StateDrone.h"
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
void DroneWanderState::Enter()
{
	owner->SetRandomTargetPosition();
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
	//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
}

// 徘徊ステートで実行するメソッド
void DroneWanderState::Execute(float elapsedTime)
{
	// 目的地点までのXZ平面での距離判定
	DirectX::XMFLOAT3 position = owner->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// 待機ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Search::Idle));
	}
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);
	// プレイヤー索敵
	if (owner->SearchTrap())
	{
		// 見つかったら追跡ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Roar));
	}
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));

}

// 徘徊ステートから出ていくときのメソッド
void DroneWanderState::Exit()
{
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Walk);
}
//待機ステートに入った時のメソッド
void DroneIdleState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
}

// 待機ステートで実行するメソッド
void DroneIdleState::Execute(float elapsedTime)
{
	// タイマー処理
	owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	if (owner->GetTutorialflag())return;
	if (owner->GetStateTimer() < 0.0f) {
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Search::Wander));
	}
	if (owner->SearchTrap()) {
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
	}
}

// 待機ステートから出ていくときのメソッド
void DroneIdleState::Exit()
{

}

// 追尾ステートで実行するメソッド
void DronePursuitState::Enter()
{
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	DirectX::XMFLOAT2 dir = owner->ForwardToTarget();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = owner->GetTargetPosition();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	float dot = (dir.x * vx) + (dir.y * vz);
	//目的地が自分より前方向なら
	if (1.0 - dot < 0.2) {
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
		owner->SetMoveRate(2.5);
		//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
	}
	else
	{
		owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
		owner->SetMoveRate(2.5);
		//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
	}
	if (owner->SearchTrap() || owner->GetRootNo() >= owner->GetMaxRootNo()) {
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
}

// 追尾ステートで実行するメソッド
void DronePursuitState::Execute(float elapsedTime)
{
	// タイマー処理
	// 目標地点をプレイヤー位置に設定
	//owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));
	if (owner->GetAttackFlg())owner->SetBattleRange(NULL);
	DirectX::XMFLOAT2 dir = owner->ForwardToTarget();
	DirectX::XMFLOAT3 traget = owner->GetTargetPosition();
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
	DirectX::XMFLOAT2 vec = Vector2::Subset({ owner->GetTargetPosition().x, owner->GetTargetPosition().z }, { owner->GetPosition().x,owner->GetPosition().z });
	float dist = sqrtf(vec.x * vec.x + vec.y * vec.y);;
	if (Vector3::FrameInput(owner->GetEnemyTimer()) && owner->GetAttackFlg()) {
		owner->SetAttackFlg(false);
		// エネミーからメタAIへ MsgChangeAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgChangeAttackRight);
	}
	if (dist < 50 && owner->GetAttackFlg() && owner->GetAttackFireBallFlg()) {
		//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::FireBall));
	}

	// 攻撃範囲に入ったとき攻撃ステートへ遷移しなさい
	if (dist < owner->GetAttackRange() && owner->GetAttackFlg()) {
		if (owner->SearchTrap() || owner->GetRootNo() >= owner->GetMaxRootNo())
		{
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Attack));
		}
		else
		{
			owner->SetRootNo(owner->GetRootNo() + 1);
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
		}
	}
	else if (dist < owner->GetAttackRange()) {
		if (owner->SearchTrap() || owner->GetRootNo() >= owner->GetMaxRootNo()) {
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Standby));
		}
		else
		{
			owner->SetRootNo(owner->GetRootNo() + 1);
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
		}
	}

}

// 追尾ステートから出ていくときのメソッド
void DronePursuitState::Exit()
{
	owner->SetMoveRate(1.0);
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Walk);
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Run);
}
void DroneAttackState::Enter()
{
	// 攻撃権があればモーション再生開始
	if (owner->GetAttackFlg())
	{

	}
	owner->SetDamegeRadius(0.2f);

}


//アタックステートで実行するメソッド
void DroneAttackState::Execute(float elapsedTime)
{

	owner->FacePlayer(elapsedTime, 1.0);

	// 攻撃権があるとき
	if (owner->GetAttackFlg())
	{


		owner->BeemShoat();

		// 攻撃モーションが終わっていれば待機へ遷移
		//if (animationTime >= 0.8)
		{
			owner->RandBattleRange();
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
		}
	}
	else
	{

		// 攻撃権がないときステート変更
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));

	}



}

// アタックステートから出ていくときのメソッド
void DroneAttackState::Exit()
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


// TODO 03_03 HierarchicalStateを継承した親ステートの
// BattleStateクラスのメンバ関数の定義を行いなさい。
// 書き方の例としてSearchStateクラスは記述しておきます。
// サーチステートデストラクタ
DroneSearchState::~DroneSearchState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// サーチステートに入った時のメソッド
void DroneSearchState::Enter()
{
	SetSubState(static_cast<int>(EnemyDrone::Search::Idle));
}
// サーチステートで実行するメソッド
void DroneSearchState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// サーチステートから出ていくときのメソッド
void DroneSearchState::Exit()
{
}


// バトルステートデストラクタ
DroneBattleState::~DroneBattleState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// バトルステートに入った時のメソッド
void DroneBattleState::Enter()
{
	SetSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
}
// バトルステートで実行するメソッド
void DroneBattleState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);
}
// バトルステートから出ていくときのメソッド
void DroneBattleState::Exit()
{
}
// TODO 05_03 メタAIからメッセージを受信したときに呼ばれるステートを追加
// デストラクタ
DroneRecieveState::~DroneRecieveState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// データ受信したときのステート
void DroneRecieveState::Enter()
{
	// 初期ステートを設定
	SetSubState(static_cast<int>(EnemyDrone::Recieve::Called));
}

// サーチステートで実行するメソッド
void DroneRecieveState::Execute(float elapsedTime)
{
	// 子ステート実行
	subState->Execute(elapsedTime);
	// 
	if (owner->SearchTrap())
	{
		// Battleステートへ遷移
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
	}
}
// サーチステートから出ていくときのメソッド
void DroneRecieveState::Exit()
{
}
// TODO 05_03 他のエネミーから呼ばれたときのステートを追加
void DroneCalledState::Enter()
{
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
	owner->SetStateTimer(5.0f);
}
// コールドステートで実行するメソッド
void DroneCalledState::Execute(float elapsedTime)
{
	// タイマー処理
	float timer = owner->GetStateTimer();
	timer -= elapsedTime;
	owner->SetStateTimer(timer);

	if (timer < 0.0f)
	{
		// 徘徊ステートへ遷移
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));


	}
	// 対象をプレイヤー地点に設定
	//owner->SetTargetPosition(Player::Instance().GetPosition());
	//owner->SetTargetPosition(Base::Instance().GetPos());
	owner->MoveToTarget(elapsedTime, 1.0f);
}
// コールドステートから出ていくときのメソッド
void DroneCalledState::Exit()
{
}

// デストラクタ
DroneReDamageState::~DroneReDamageState()
{
	for (State* state : subStatePool)
	{
		delete state;
	}
	subStatePool.clear();
}
// ダメージ入力された時のステート
void DroneReDamageState::Enter()
{
	owner->SetGravity(-1.0f);
	// 初期ステートを設定
	SetSubState(static_cast<int>(EnemyDrone::ReDamage::Damege));
	if (owner->GetHealth() < 1)
	{
		SetSubState(static_cast<int>(EnemyDrone::ReDamage::Die));
	}
}

// ダメージ入力ステートで実行するメソッド
void DroneReDamageState::Execute(float elapsedTime)
{
	// 子ステート実行
	subState->Execute(elapsedTime);

}
// ダメージ入力ステートから出ていくときのメソッド

void DroneReDamageState::Exit()
{
}
//ダメージステートに入った時のメソッド
void DroneDamageState::Enter()
{
	//ダメージアニメーション再生
	owner->GetModel()->PlayAnimation(EnemyDrone::EnemyDroneAnimation::Idle, false);

}


// ダメージステートで実行するメソッド
void DroneDamageState::Execute(float elapsedTime)
{


	if (!owner->GetModel()->IsPlayAnimation())
	{

		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Battle));
		if (owner->GetTutorialflag()) {
			owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyDrone::DroneState::Search));
			owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Search::Idle));
		}
	}

}

// ダメージステートから出ていくときのメソッド
void DroneDamageState::Exit()
{

}

//死亡ステートに入った時のメソッド
void DroneDieState::Enter()
{
	// 
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), false);
}


// 死亡ステートで実行するメソッド
void DroneDieState::Execute(float elapsedTime)
{
	if (!owner->GetModel()->IsPlayAnimation()) {
		//owner->Destroy();
	}
}

// 死亡ステートから出ていくときのメソッド
void DroneDieState::Exit()
{

}
// 戦闘待機ステートに入った時のメソッド
void DroneStandbyState::Enter()
{
	if (!owner->GetAttackFlg())
	{
		// 攻撃権をメタAIから求める
		// エネミーからメタAIへ MsgAskAttackRight を送信する
		Meta::Instance().SendMessaging(owner->GetId(), 0, MESSAGE_TYPE::MsgAskAttackRight);
	}
	owner->SetStateTimer(Mathf::RandomRange(2.0f, 3.0f));
	//if (rand() % 2 == 0) {
	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::EnemyDroneAnimation::Idle), true);
	//	rightflag = true;
	//}
	//else
	//{
	//	owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDrone::WalkFWD), true);
	//	rightflag = false;
	//}
	//owner->PlaySe((int)EnemyDrone::EnemyDroneSE::Walk, true);
	owner->SetMoveRate(0.5);
	//owner->GetModel()->PlayAnimation(static_cast<int>(EnemyDroneAnimation::IdleBattle), false);
}
// 戦闘待機ステートで実行するメソッド
void DroneStandbyState::Execute(float elapsedTime)
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
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
	}
	owner->BattleMove(rightflag, elapsedTime, owner->GetMoveRate());
	// 目標地点をプレイヤー位置に設定
	//DirectX::XMFLOAT2 dir = owner->ForwardToPlayer();
	DirectX::XMFLOAT2 dir = owner->ForwardToTarget();
	//DirectX::XMFLOAT3 traget = Player::Instance().GetPosition();
	DirectX::XMFLOAT3 traget = owner->GetTargetPosition();
	traget.x -= dir.x * owner->GetBattleRange();
	traget.z -= dir.y * owner->GetBattleRange();
	owner->SetTargetPosition(traget);
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vy = owner->GetTargetPosition().y - owner->GetPosition().y;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist > owner->GetAttackRange() && !owner->GetAttackFlg())
	{
		// 攻撃範囲から出たら追跡ステートへ遷移
		owner->SetBattleRange(static_cast<float>(rand() % 6 + 1));
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));
	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		owner->RandBattleRange();
		owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::Pursuit));

	}
	if (owner->GetStateTimer() < 0 && !owner->GetAttackFlg()) {
		if (Vector3::Probability(2) && dist < owner->GetBackStepRange()) {
			//owner->GetStateMachine()->ChangeSubState(static_cast<int>(EnemyDrone::Battle::BackStep));
		}
	}

}
// 戦闘待機ステートから出ていくときのメソッド
void DroneStandbyState::Exit()
{
	//owner->StopSe((int)EnemyDrone::EnemyDroneSE::Walk);
	owner->SetMoveRate(1.0);
}
