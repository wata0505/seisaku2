#pragma once
#include<vector>
#include<set>
#include"Particle.h"

//弾丸マネージャー
class ParticleManager
{
public:

public:
	ParticleManager();
	~ParticleManager();
	static ParticleManager& Instance()
	{
		static ParticleManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, ParticleShader* shader);

	// オブジェクト登録
	void Register(Particle* Particle);

	//　オブジェクト全削除
	void Clear();

	// オブジェクト数取得
	int GetParticleCount() const { return static_cast<int>(particles.size()); }

	// オブジェクト取得
	Particle* GetParticle(int index) { return particles.at(index); }


	std::shared_ptr<Model> GetModel(int type) { return model[type]; }

	// オブジェクト削除
	void Remove(Particle* Particle);



private:
	std::shared_ptr<Model> model[Particle::Max] = { nullptr };
	std::vector<Particle*> particles;
	ParticleConstants data[Particle::Max];
	std::set<Particle*> removes;

	int  particleNo[Particle::Max] = { 0 };
};