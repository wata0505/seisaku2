#include "InstancingSpriteManager.h"
#include"EffectTexAll.h"

InstancingSpriteManager::InstancingSpriteManager()
{
	//model = std::make_unique<Model>(".\\resources\\InstancingSprite\\Sphere.fbx", true, false);
	//model->ModelSerialize(".\\resources\\InstancingSprite\\Sphere.fbx");
	////model->ModelCreate(".\\resources\\ExampleStage\\ExampleStage.fbx");
	//model->ModelRegister(".\\resources\\InstancingSprite\\Sphere.fbx");
	//model = std::make_unique<Model>(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx", false,false);
	//model->ModelSerialize(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//model->ModelRegister(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//moveSprite[InstancingSprite::InstancingSpriteSoft] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\InstancingSprite_Soft.png");
	//moveSprite[InstancingSprite::InstancingSpriteLine] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Line9.png");
	//moveSprite[InstancingSprite::InstancingSpriteImpact] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Shock_wave1.png");
	//moveSprite[InstancingSprite::InstancingSpriteWind] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\wind04.png");
	moveSprite[InstancingSprite::FlameBreath] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Fire.png");
	moveSprite[InstancingSprite::SeirlConvergence] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Particle_Soft.png");
	moveSprite[InstancingSprite::VortexDiffusion] = moveSprite[InstancingSprite::SeirlConvergence];
	moveSprite[InstancingSprite::Corruption] = std::make_unique<Sprite>(L".\\resources\\Textures\\hart.png");
	moveSprite[InstancingSprite::Corruption2] = std::make_unique<Sprite>(L".\\resources\\Textures\\OIP.png");

	//texSprite[InstancingSprite::TexNull] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\dummy.png");
	texSprite[InstancingSprite::FlameBreath] = EffectTexAll::Instance().GetSprite((int)EffectTexAll::EfTexAll::Flame);
	texSprite[InstancingSprite::SeirlConvergence] = EffectTexAll::Instance().GetSprite((int)EffectTexAll::EfTexAll::Impact);
	texSprite[InstancingSprite::VortexDiffusion] = texSprite[InstancingSprite::SeirlConvergence];
	texSprite[InstancingSprite::Corruption] = moveSprite[InstancingSprite::Corruption];
	texSprite[InstancingSprite::Corruption2] = moveSprite[InstancingSprite::Corruption2];

}

InstancingSpriteManager::~InstancingSpriteManager()
{
	Clear();
}


// 更新処理
void InstancingSpriteManager::Update(float elapsedTime)
{
	//更新処理
	for (int i = 0; i < InstancingSprite::Max; i++)
	{
		instancingSpriteNo[i] = 0;
	}
	for (InstancingSprite* instancingSprite : InstancingSprites)
	{
		instancingSprite->Update(elapsedTime);
		int type = instancingSprite->GetSpriteType();
		//タイプごとの配列に情報を格納
		if (instancingSpriteNo[type] < MAX_INSTANCES) {
			data[type].world[instancingSpriteNo[type]] = instancingSprite->GetPosition();
			data[type].materialColor[instancingSpriteNo[type]] = instancingSprite->GetMaterialColor();
			data[type].spriteSize[instancingSpriteNo[type]] = instancingSprite->GetSpriteSize();
			data[type].timer[instancingSpriteNo[type]] = { instancingSprite->GetTexTimer(),0,0,0};
			data[type].dissolve[instancingSpriteNo[type]] = instancingSprite->GetUvStatus();
			instancingSpriteNo[type]++;
		}
	}
	// 破棄処理
	// ※InstancingSpritesの範囲for文中でerase()すると不具合が発生してしまうため、
	//	 更新終わった後に廃棄リストに積まれたオブジェクトを削除する。
	for (InstancingSprite* instancingSprite : removes)
	{
		/*
				std::vectorで管理されている要素を
				 削除するにはerase()関数を使用する。
			　廃棄リストにのポインタからイテレーターを
					検索し、erase()関数に渡す。
														*/
														// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<InstancingSprite*>::iterator it = std::find(InstancingSprites.begin(), InstancingSprites.end(), instancingSprite);

		if (it != InstancingSprites.end())
		{
			InstancingSprites.erase(it);
		}

		//弾丸の廃棄処理
		delete instancingSprite;
	}
	//廃棄リストをクリア
	removes.clear();

	//model->UpdateBufferDara(data.world[1]);
}


// 描画処理
void InstancingSpriteManager::Render(ID3D11DeviceContext* dc, InstancingSpriteShader* shader)
{
	for (int i = 0; i < InstancingSprite::Max; i++)
	{
		
		
	}

}






// 弾丸登録
void InstancingSpriteManager::Register(InstancingSprite* InstancingSprite)
{
	InstancingSprites.emplace_back(InstancingSprite);
}


// 弾丸全削除
void InstancingSpriteManager::Clear()
{
	for (InstancingSprite* instancingSprite : InstancingSprites)
	{
		delete instancingSprite;
	}
	InstancingSprites.clear();
}

void InstancingSpriteManager::Remove(InstancingSprite* instancingSprite)
{
	/*直接、InstancingSpritesの要素を削除してしまうと
		範囲for文で不具合を起こすため、
			廃棄リストに追加する
	*/
	//廃棄リストに追加
	removes.insert(instancingSprite);
}