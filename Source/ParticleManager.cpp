#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	model[Particle::Ball] = std::make_unique<Model>(".\\resources\\Particle\\Sphere.fbx", true, false);
	model[Particle::Ball]->ModelSerialize(".\\resources\\Particle\\Sphere.fbx");
	model[Particle::Ball]->ModelRegister(".\\resources\\Particle\\Sphere.fbx");
	model[Particle::Ball]->UpdateBufferDara(data[Particle::Ball].world[0]);
	//model = std::make_unique<Model>(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx", false,false);
	//model->ModelSerialize(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//model->ModelRegister(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//model[Particle::Lance] = std::make_unique<Model>(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx", true);
	//model[Particle::Lance]->ModelSerialize(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx");
	//model[Particle::Lance]->ModelRegister(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx");
	//model[Particle::Lance]->UpdateBufferDara(data[Particle::Lance].world[0]);
	model[Particle::Rock1] = std::make_unique<Model>(".\\resources\\ExampleStage\\rock_11.fbx", true);
	model[Particle::Rock1]->ModelSerialize(".\\resources\\ExampleStage\\rock_11.fbx");
	model[Particle::Rock1]->ModelRegister(".\\resources\\ExampleStage\\rock_11.fbx","tex.png");
	model[Particle::Rock1]->UpdateBufferDara(data[Particle::Rock1].world[0]);
	model[Particle::Rock2] = std::make_unique<Model>(".\\resources\\ExampleStage\\rock_10.fbx", true);
	model[Particle::Rock2]->ModelSerialize(".\\resources\\ExampleStage\\rock_10.fbx");
	model[Particle::Rock2]->ModelRegister(".\\resources\\ExampleStage\\rock_10.fbx","tex.png");
	model[Particle::Rock2]->UpdateBufferDara(data[Particle::Rock2].world[0]);
	model[Particle::Rock3] = std::make_unique<Model>(".\\resources\\ExampleStage\\rock_9.fbx", true);
	model[Particle::Rock3]->ModelSerialize(".\\resources\\ExampleStage\\rock_9.fbx");
	model[Particle::Rock3]->ModelRegister(".\\resources\\ExampleStage\\rock_9.fbx","tex.png");
	model[Particle::Rock3]->UpdateBufferDara(data[Particle::Rock3].world[0]);
}

ParticleManager::~ParticleManager()
{
	Clear();
}


// 更新処理
void ParticleManager::Update(float elapsedTime)
{
	//更新処理
	for (int i = 0; i < Particle::Max; i++)
	{
		particleNo[i] = 0;
	}
	for (Particle* Particle : particles)
	{
		Particle->Update(elapsedTime);
	}
	// 破棄処理
	// ※Particlesの範囲for文中でerase()すると不具合が発生してしまうため、
	//	 更新終わった後に廃棄リストに積まれたオブジェクトを削除する。
	for (Particle* particle : removes)
	{
		/*
				std::vectorで管理されている要素を
				 削除するにはerase()関数を使用する。
			　廃棄リストにのポインタからイテレーターを
					検索し、erase()関数に渡す。
														*/
														// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Particle*>::iterator it = std::find(particles.begin(), particles.end(), particle);

		if (it != particles.end())
		{
			particles.erase(it);
		}

		//弾丸の廃棄処理
		delete particle;
	}
	//廃棄リストをクリア
	removes.clear();

	//model->UpdateBufferDara(data.world[1]);
}


// 描画処理
void ParticleManager::Render(ID3D11DeviceContext* dc, ParticleShader* shader)
{
	
	for (Particle* particle : particles)
	{
		particle->Render(dc, shader);
	}

}






// 弾丸登録
void ParticleManager::Register(Particle* Particle)
{
	particles.emplace_back(Particle);
}


// 弾丸全削除
void ParticleManager::Clear()
{
	for (Particle* Particle : particles)
	{
		delete Particle;
	}
	particles.clear();
}

void ParticleManager::Remove(Particle* Particle)
{
	/*直接、Particlesの要素を削除してしまうと
		範囲for文で不具合を起こすため、
			廃棄リストに追加する
	*/
	//廃棄リストに追加
	removes.insert(Particle);
}