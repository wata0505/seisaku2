#pragma once
#include <vector>
#include"Calculation.h"
class EnemyDragon;

class State
{
public:

	virtual ~State() {}
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;
};
template <class T>class HierarchicalState : public State
{
public:
	// �R���X�g���N�^
	HierarchicalState(T* enemy){ owner = enemy; }
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
