#include "StateMachine.h"

//template <class T>StateMachine<T>::~StateMachine()
//{
//	for (HierarchicalState<T>* state : statePool)
//	{
//		delete state;
//	}
//	statePool.clear();
//}
//// �X�V����
//template <class T>void StateMachine<T>::Update(float elapsedTime)
//{
//	currentState->Execute(elapsedTime);
//}
//// �X�e�[�g�Z�b�g
//template <class T>void StateMachine<T>::SetState(int setState)
//{
//	currentState = statePool.at(setState);
//	currentState->Enter();
//}
//// �X�e�[�g�ύX
//template <class T>void StateMachine<T>::ChangeState(int newState)
//{
//	// TODO 02_02 �X�e�[�g�̐؂�ւ��B
//	// ���݂̃X�e�[�g��Exit�֐������s�A�V�����X�e�[�g���Z�b�g�A�V�����X�e�[�g��Enter�֐����Ăяo���B
//	if (currentState->GetSubState() != nullptr) {
//		currentState->GetSubState()->Exit();
//	}
//	if (currentState != nullptr) {
//		currentState->Exit();
//	}
//	SetState(newState);
//	//currentState->Enter();
//}
//template <class T>void StateMachine<T>::ChangeSubState(int newState)
//{
//	// TODO 03_04 HierarchicalState�N���X��ChangeSubState���Ăяo��
//	currentState->ChangeSubState(newState);
//}
//// �T�u�X�e�[�g�o�^
//template <class T>void StateMachine<T>::RegisterSubState(int index, State<T>* subState)
//{
//	statePool.at(index)->RegisterSubState(subState);
//}
//// �X�e�[�g�o�^
//template <class T>void StateMachine<T>::RegisterState(HierarchicalState<T>* state)
//{
//	// �e�X�e�[�g�o�^
//	statePool.emplace_back(state);
//}
//// �X�e�[�g�ԍ��擾
//template <class T>int StateMachine<T>::GetStateIndex()
//{
//	int i = 0;
//	for (HierarchicalState<T>* state : statePool)
//	{
//		if (state == currentState)
//		{
//			// i�ԍ��ڂ̃X�e�[�g�����^�[��
//			return i;
//		}
//		++i;
//	}
//	// �X�e�[�g��������Ȃ�������
//	return -1;
//}