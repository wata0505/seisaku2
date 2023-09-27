#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	Clear();
}


// 更新処理
void ObjectManager::Update(float elapsedTime)
{
	//更新処理
	for (Object* Object : Objects)
	{
		Object->Update(elapsedTime);
	}
	// 破棄処理
	// ※Objectsの範囲for文中でerase()すると不具合が発生してしまうため、
	//	 更新終わった後に廃棄リストに積まれたオブジェクトを削除する。
	for (Object* object : removes)
	{
		/*
				std::vectorで管理されている要素を
				 削除するにはerase()関数を使用する。
			　廃棄リストにのポインタからイテレーターを
					検索し、erase()関数に渡す。
														*/
														// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Object*>::iterator it = std::find(Objects.begin(), Objects.end(), object);

		if (it != Objects.end())
		{
			Objects.erase(it);
		}

		//弾丸の廃棄処理
		delete object;
	}
	//廃棄リストをクリア
	removes.clear();
}


// 描画処理
void ObjectManager::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	for (Object* Object : Objects)
	{
		Object->Render(dc, shader);
	}
}

void ObjectManager::TrailRender(RenderContext& rc, SwordTrailShader* shader)
{
	for (Object* Object : Objects)
	{
		Object->TrailRender(rc, shader);
	}

}

// デバッグプリミティブ描画
void ObjectManager::DrawDebugPrimitive()
{
	for (Object* Object : Objects)
	{
		Object->DrawDebugPrimitive();
	}
}


// 弾丸登録
void ObjectManager::Register(Object* Object)
{
	Objects.emplace_back(Object);
}


// 弾丸全削除
void ObjectManager::Clear()
{
	for (Object* Object : Objects)
	{
		delete Object;
	}
	Objects.clear();
}

void ObjectManager::Remove(Object* Object)
{
	/*直接、Objectsの要素を削除してしまうと
		範囲for文で不具合を起こすため、
			廃棄リストに追加する
	*/
	//廃棄リストに追加
	removes.insert(Object);
}
