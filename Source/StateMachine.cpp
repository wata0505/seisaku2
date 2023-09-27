#include "StateMachine.h"

//template <class T>StateMachine<T>::~StateMachine()
//{
//	for (HierarchicalState<T>* state : statePool)
//	{
//		delete state;
//	}
//	statePool.clear();
//}
//// 更新処理
//template <class T>void StateMachine<T>::Update(float elapsedTime)
//{
//	currentState->Execute(elapsedTime);
//}
//// ステートセット
//template <class T>void StateMachine<T>::SetState(int setState)
//{
//	currentState = statePool.at(setState);
//	currentState->Enter();
//}
//// ステート変更
//template <class T>void StateMachine<T>::ChangeState(int newState)
//{
//	// TODO 02_02 ステートの切り替え。
//	// 現在のステートのExit関数を実行、新しいステートをセット、新しいステートのEnter関数を呼び出す。
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
//	// TODO 03_04 HierarchicalStateクラスのChangeSubStateを呼び出す
//	currentState->ChangeSubState(newState);
//}
//// サブステート登録
//template <class T>void StateMachine<T>::RegisterSubState(int index, State<T>* subState)
//{
//	statePool.at(index)->RegisterSubState(subState);
//}
//// ステート登録
//template <class T>void StateMachine<T>::RegisterState(HierarchicalState<T>* state)
//{
//	// 親ステート登録
//	statePool.emplace_back(state);
//}
//// ステート番号取得
//template <class T>int StateMachine<T>::GetStateIndex()
//{
//	int i = 0;
//	for (HierarchicalState<T>* state : statePool)
//	{
//		if (state == currentState)
//		{
//			// i番号目のステートをリターン
//			return i;
//		}
//		++i;
//	}
//	// ステートが見つからなかった時
//	return -1;
//}