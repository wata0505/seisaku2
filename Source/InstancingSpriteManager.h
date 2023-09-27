#pragma once
#pragma once
#include<vector>
#include<set>
#include"InstancingSprite.h"

//弾丸マネージャー
class InstancingSpriteManager
{
public:

public:
	InstancingSpriteManager();
	~InstancingSpriteManager();
	static InstancingSpriteManager& Instance()
	{
		static InstancingSpriteManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, InstancingSpriteShader* shader);

	// オブジェクト登録
	void Register(InstancingSprite* InstancingSprite);

	//　オブジェクト全削除
	void Clear();

	// オブジェクト数取得
	int GetInstancingSpriteCount() const { return static_cast<int>(InstancingSprites.size()); }

	// オブジェクト取得
	InstancingSprite* GetInstancingSprite(int index) { return InstancingSprites.at(index); }

	InstancingSpriteConstants GetSpriteConstants(int index) { return data[index]; }

	Sprite* GetMoveSprite(int i) { return moveSprite[i].get(); }
	Sprite* GetTexSprite(int i) { return texSprite[i].get(); }

	// オブジェクト削除
	void Remove(InstancingSprite* InstancingSprite);

	int GetInstancingSpriteNo(int i) { return instancingSpriteNo[i]; }


private:
	std::shared_ptr<Sprite> moveSprite[InstancingSprite::Max] = { nullptr };
	std::shared_ptr<Sprite> texSprite[InstancingSprite::Max] = { nullptr };
	std::vector<InstancingSprite*> InstancingSprites;
	InstancingSpriteConstants data[InstancingSprite::Max];
	std::set<InstancingSprite*> removes;

	int  instancingSpriteNo[InstancingSprite::Max] = { 0 };
};