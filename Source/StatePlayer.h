#pragma once
#include "Player.h"
#include "MetaAI.h"


// 待機ステートオブジェクト
class IdleState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	IdleState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~IdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 移動ステートオブジェクト
class MoveState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	MoveState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~MoveState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// ジャンプ準備ステートオブジェクト
class JumpStandbyState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	JumpStandbyState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~JumpStandbyState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// ジャンプステートオブジェクト
class JumpState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	JumpState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~JumpState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 着地ステートオブジェクト
class LandState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	LandState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~LandState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};


// 回避ステートオブジェクト
class RollState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	RollState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~RollState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// スティンガーステートオブジェクト
class StingerState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	StingerState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~StingerState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// シフトステートオブジェクト
class ShiftState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	ShiftState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~ShiftState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// シフト攻撃ステートオブジェクト
class ShiftAttackState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	ShiftAttackState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~ShiftAttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// ハルバート特殊ステートオブジェクト
class LanceState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	LanceState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~LanceState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// チャージステートオブジェクト
class ChargeState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	ChargeState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~ChargeState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// チャージ攻撃ステートオブジェクト
class ChargeAttackState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	ChargeAttackState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~ChargeAttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// 攻撃ステートオブジェクト
class AttackState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	AttackState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~AttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// 落下攻撃ステートオブジェクト
class FallAttackState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	FallAttackState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~FallAttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// ガードステートオブジェクト
class GuardState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	GuardState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~GuardState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// カウンター準備ステートオブジェクト
class CounterStandbyState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	CounterStandbyState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~CounterStandbyState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// カウンターステートオブジェクト
class CounterState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	CounterState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~CounterState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// テレポートステートオブジェクト
class TeleportState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	TeleportState(Player* player) :HierarchicalState<Player>(player) {};
	// デストラクタ
	~TeleportState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

//自機ステート
class PlayerState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	PlayerState(Player* player) :HierarchicalState<Player>(player) {}
	~PlayerState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};
//ダメージステート
class DamageState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	DamageState(Player* player) :HierarchicalState<Player>(player) {}
	~DamageState() {};
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//ダウンステート
class DownState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	DownState(Player* player) :HierarchicalState<Player>(player) {}
	~DownState() {};
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//死亡ステート
class DieState : public HierarchicalState<Player>
{
public:
	// コンストラクタ
	DieState(Player* player) :HierarchicalState<Player>(player) {}
	~DieState() {};
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	/// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
