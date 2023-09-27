#pragma once
#include <vector>
#include "StateBase.h"

template <class T>class StateMachine
{
public:
	// �R���X�g���N�^
	StateMachine() {}
	// �f�X�g���N�^
	//~StateMachine();
	// �X�V����
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
	// TODO 03_04 �V�K�ő�Q�K�w�̃X�e�[�g�֘A��o�^
// �Q�w�ڃX�e�[�g�ύX
	void RegisterState(HierarchicalState<T>* state) {
		statePool.emplace_back(state);
	};
	// �Q�w�ڃX�e�[�g�o�^
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
	// ���݂̃X�e�[�g
	std::vector<HierarchicalState<T>*> statePool;
	// �e�X�e�[�g��ێ�����z��
	HierarchicalState<T>* state = nullptr;
};