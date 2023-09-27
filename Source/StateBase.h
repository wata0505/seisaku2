#pragma once
#include <vector>
#include"Calculation.h"
class EnemyDragon;

class State
{
public:

	virtual ~State() {}
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;
};
template <class T>class HierarchicalState : public State
{
public:
	// コンストラクタ
	HierarchicalState(T* enemy){ owner = enemy; }
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;

	T* owner = nullptr;

	void ChangeSubState(int newState) {
		// 
		subState->Exit();
		SetSubState(newState);
		subState->Enter();

	};
	void SetSubState(int newState) {
		// 
		subState = subStatePool.at(newState);
		subState->Enter();
	};
	void RegisterState(HierarchicalState<T>* state) {
		statePool.emplace_back(state);
	};
	State* GetSubState() {
		// 
		return subState;

	};
	void RegisterSubState(int index, State* subState) {
		statePool.at(index)->RegisterSubStatePool(subState);
	};
	int GetStateNum() {
		int i = 0;
		for (State* state : statePool)
		{
			if (this->state == state)return i;
			++i;
		}

		return i;

	};

	void RegisterSubStatePool(State* state)
	{
		subStatePool.emplace_back(state);
	}

	int GetSubStateNum()
	{
		int i = 0;
		for (State* state : subStatePool)
		{
			if (subState == state)return i;
			++i;
		}
		return i;
	}
protected:
	std::vector<State*> subStatePool;
	State* subState = nullptr;
};
