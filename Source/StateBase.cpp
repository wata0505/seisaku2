#include "StateBase.h"

// TODO 03_02 HierarchicalState�N���X�̊֐���`���s��
// 2�w�ڃX�e�[�g�Z�b�g
//template <class T>void HierarchicalState<T>::SetSubState(int newState)
//{
//	// TODO 03_02 �O���SetState�֐����Q�l�ɋL�q���Ȃ���
//	subState = subStatePool.at(newState);
//	subState->Enter();
//}
//
//// 2�w�ڂ̃X�e�[�g�؂�ւ�
//template <class T>void HierarchicalState<T>::ChangeSubState(int newState)
//{
//	// TODO 03_02 �O���ChangeState�֐����Q�l�ɋL�q���Ȃ���
//	if (subState != nullptr) {
//		subState->Exit();
//	}
//	SetSubState(newState);
//}
//// �T�u�X�e�[�g�o�^
//template <class T>void HierarchicalState<T>::RegisterSubState(State<T>* state)
//{
//	subStatePool.emplace_back(state);
//}
//
//// �T�u�X�e�[�g�̔ԍ��擾
//template <class T>int HierarchicalState<T>::GetSubStateIndex()
//{
//	int i = 0;
//	for (State* state : subStatePool)
//	{
//		if (subState == state)return i;
//		++i;
//	}
//	return i;
//}
//