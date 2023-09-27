#include "ParticleSpriteManager.h"


ParticleSpriteManager::ParticleSpriteManager()
{
	
}

ParticleSpriteManager::~ParticleSpriteManager()
{
	Clear();
}


// 更新処理
void ParticleSpriteManager::Update(float elapsedTime)
{
	//更新処理
	ParticleSpriteNo = 0;
	for (ParticleSprite* particleSprite : particleSprites)
	{
		particleSprite->Update(elapsedTime);
	}
	// 破棄処理
	// ※ParticleSpritesの範囲for文中でerase()すると不具合が発生してしまうため、
	//	 更新終わった後に廃棄リストに積まれたオブジェクトを削除する。
	for (ParticleSprite* particleSprite : removes)
	{
		/*
				std::vectorで管理されている要素を
				 削除するにはerase()関数を使用する。
			　廃棄リストにのポインタからイテレーターを
					検索し、erase()関数に渡す。
														*/
														// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<ParticleSprite*>::iterator it = std::find(particleSprites.begin(), particleSprites.end(), particleSprite);

		if (it != particleSprites.end())
		{
			particleSprites.erase(it);
		}

		//弾丸の廃棄処理
		delete particleSprite;
	}
	//廃棄リストをクリア
	removes.clear();
}


// 描画処理
//void ParticleSpriteManager::Render(ID3D11DeviceContext* dc, ParticleSpriteShader* shader)
//{
//
//	//shader->Draw(dc, model.get(), data, ParticleSpriteNo);
//	//for (ParticleSprite* ParticleSprite : ParticleSprites)
//	//{
//	//	ParticleSprite->Render(dc, shader);
//	//}
//
//}





// パーティク登録
void ParticleSpriteManager::Register(ParticleSprite* ParticleSprite)
{
	particleSprites.emplace_back(ParticleSprite);
}


// パーティクル全削除
void ParticleSpriteManager::Clear()
{
	for (ParticleSprite* ParticleSprite : particleSprites)
	{
		delete ParticleSprite;
	}
	particleSprites.clear();
}

void ParticleSpriteManager::Remove(ParticleSprite* ParticleSprite)
{
	/*直接、ParticleSpritesの要素を削除してしまうと
		範囲for文で不具合を起こすため、
			廃棄リストに追加する
	*/
	//廃棄リストに追加
	removes.insert(ParticleSprite);
}