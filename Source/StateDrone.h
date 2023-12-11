#pragma once
#include "EnemyDrone.h"
#include "MetaAI.h"


// 徘徊ステートオブジェクト
class DroneWanderState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneWanderState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// デストラクタ
	~DroneWanderState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 待機ステートオブジェクト
class DroneIdleState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneIdleState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// デストラクタ
	~DroneIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 追跡ステートオブジェクト
class DronePursuitState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DronePursuitState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// デストラクタ
	~DronePursuitState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 攻撃ステートオブジェクト
class DroneAttackState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneAttackState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// デストラクタ
	~DroneAttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

//サーチステート
class DroneSearchState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneSearchState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneSearchState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};
class DroneBattleState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneBattleState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneBattleState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};

class DroneRecieveState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneRecieveState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneRecieveState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};

//ダメージ入力ステート
class DroneReDamageState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneReDamageState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneReDamageState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};
//ダメージステート
class DroneDamageState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneDamageState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneDamageState() {};
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//死亡ステート
class DroneDieState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneDieState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {}
	~DroneDieState() {};
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	/// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//他のエネミーから呼ばれたときのステートを追加
class DroneCalledState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneCalledState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// デストラクタ
	~DroneCalledState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// 戦闘待機ステートオブジェクト
class DroneStandbyState : public HierarchicalState<EnemyDrone>
{
public:
	// コンストラクタ
	DroneStandbyState(EnemyDrone* enemy) :HierarchicalState<EnemyDrone>(enemy) {};
	// デストラクタ
	~DroneStandbyState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

	bool rightflag = false;
};
