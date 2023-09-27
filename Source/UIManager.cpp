#include "UIManager.h"




// 更新処理
void UIManager::Update(float elapsedTime)
{
	//更新処理
	for (UI* ui : uis)
	{
		ui->Update(elapsedTime);
	}
	// 破棄処理
	// ※Objectsの範囲for文中でerase()すると不具合が発生してしまうため、
	//	 更新終わった後に廃棄リストに積まれたオブジェクトを削除する。
	for (UI* ui : removes)
	{
		/*
				std::vectorで管理されている要素を
				 削除するにはerase()関数を使用する。
			　廃棄リストにのポインタからイテレーターを
					検索し、erase()関数に渡す。
														*/
														// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<UI*>::iterator it = std::find(uis.begin(), uis.end(), ui);

		if (it != uis.end())
		{
			uis.erase(it);
		}

		//弾丸の廃棄処理
		delete ui;
	}
	//廃棄リストをクリア
	removes.clear();
}


// 描画処理
void UIManager::Render(RenderContext& rc, SpriteShader* shader)
{
	for (UI* ui : uis)
	{
		ui->Render(rc, shader);
	}
}

// 弾丸登録
void UIManager::Register(UI* ui)
{
	uis.emplace_back(ui);
}


// 弾丸全削除
void UIManager::Clear()
{
	for (UI* ui : uis)
	{
		delete ui;
	}
	uis.clear();
}

void UIManager::Remove(UI* ui)
{
	/*直接、uisの要素を削除してしまうと
		範囲for文で不具合を起こすため、
			廃棄リストに追加する
	*/
	//廃棄リストに追加
	removes.insert(ui);
}