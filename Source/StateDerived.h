#pragma once
#include "EnemyBag.h"
#include "MetaAI.h"


// 徘徊ステートオブジェクト
class BagWanderState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagWanderState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagWanderState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 待機ステートオブジェクト
class BagIdleState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagIdleState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 追跡ステートオブジェクト
class BagPursuitState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagPursuitState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagPursuitState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 攻撃ステートオブジェクト
class BagAttackState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagAttackState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagAttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//ファイルボールステートオブジェクト
class BagFireBallState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagFireBallState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagFireBallState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//サーチステート
class BagSearchState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagSearchState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagSearchState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};
class BagBattleState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagBattleState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagBattleState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};
//咆哮ステート
class BagRoarState: public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagRoarState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagRoarState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

class BagRecieveState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagRecieveState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagRecieveState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};

//ダメージ入力ステート
class BagReDamageState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagReDamageState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagReDamageState();
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter();
	// ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド
	void Exit();
};
//ダメージステート
class BagDamageState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagDamageState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagDamageState() {};
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//死亡ステート
class BagDieState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagDieState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {}
	~BagDieState() {};
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	/// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//他のエネミーから呼ばれたときのステートを追加
class BagCalledState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagCalledState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagCalledState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
// 戦闘待機ステートオブジェクト
class BagStandbyState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagStandbyState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagStandbyState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

	bool rightflag = false;
};
// バックステップステートオブジェクト
class BagBackStepState : public HierarchicalState<EnemyBag>
{
public:
	// コンストラクタ
	BagBackStepState(EnemyBag* enemy) :HierarchicalState<EnemyBag>(enemy) {};
	// デストラクタ
	~BagBackStepState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

	bool rightflag = false;
};