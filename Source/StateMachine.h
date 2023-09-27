#pragma once
#include <vector>
#include "StateBase.h"

template <class T>class StateMachine
{
public:
	// コンストラクタ
	StateMachine() {}
	// デストラクタ
	//~StateMachine();
	// 更新処理
	void Update(float elapsedTime) {
		state->Execute(elapsedTime);
	};
	// 
	void SetState(int newState) {
		state = statePool.at(newState);
		state->Enter();
	}
	// 
	HierarchicalState<T>* GetState() { return state; }
	// 
	void ChangeState(int newState) {
		state->Exit();
		state = statePool.at(newState);
		state->Enter();
	};
	// 
	void ChangeSubState(int newState) {
		// 
		state->GetSubState()->Exit();
		state->SetSubState(newState);
		//state->GetSubState()->Enter();

	};
	// TODO 03_04 新規で第２階層のステート関連を登録
// ２層目ステート変更
	void RegisterState(HierarchicalState<T>* state) {
		statePool.emplace_back(state);
	};
	// ２層目ステート登録
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
private:
	// 現在のステート
	std::vector<HierarchicalState<T>*> statePool;
	// 各ステートを保持する配列
	HierarchicalState<T>* state = nullptr;
};