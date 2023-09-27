#include "StateBase.h"

// TODO 03_02 HierarchicalStateクラスの関数定義を行う
// 2層目ステートセット
//template <class T>void HierarchicalState<T>::SetSubState(int newState)
//{
//	// TODO 03_02 前回のSetState関数を参考に記述しなさい
//	subState = subStatePool.at(newState);
//	subState->Enter();
//}
//
//// 2層目のステート切り替え
//template <class T>void HierarchicalState<T>::ChangeSubState(int newState)
//{
//	// TODO 03_02 前回のChangeState関数を参考に記述しなさい
//	if (subState != nullptr) {
//		subState->Exit();
//	}
//	SetSubState(newState);
//}
//// サブステート登録
//template <class T>void HierarchicalState<T>::RegisterSubState(State<T>* state)
//{
//	subStatePool.emplace_back(state);
//}
//
//// サブステートの番号取得
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