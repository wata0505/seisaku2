#pragma once
#include<vector>
#include<set>
#include"ParticleSprite.h"

//弾丸マネージャー
class ParticleSpriteManager
{
public:
	ParticleSpriteManager();
	~ParticleSpriteManager();
	static ParticleSpriteManager& Instance()
	{
		static ParticleSpriteManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	//void Render(ID3D11DeviceContext* dc, ParticleSpriteShader* shader);

	// オブジェクト登録
	void Register(ParticleSprite* ParticleSprite);

	//　オブジェクト全削除
	void Clear();

	// オブジェクト数取得
	int GetParticleSpriteCount() const { return static_cast<int>(particleSprites.size()); }

	// オブジェクト取得
	ParticleSprite* GetParticleSprite(int index) { return particleSprites.at(index); }

	// オブジェクト削除
	void Remove(ParticleSprite* ParticleSprite);

	std::vector<ParticleSprite*> GetParticleSprites() {return particleSprites;}

	int GetParticleCount()const { return static_cast<int>(particleSprites.size()); }

	

private:
	std::shared_ptr<Model> model = { nullptr };
	std::vector<ParticleSprite*> particleSprites;
	std::set<ParticleSprite*> removes;

	int  ParticleSpriteNo = 0;
};