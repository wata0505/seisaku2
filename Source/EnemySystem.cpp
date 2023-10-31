#include "EnemySystem.h"
#include "EnemyBag.h"
EnemySystem::EnemySystem() {

	

}

void EnemySystem::Start() {
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < maxEnemyCount; i++) {
		EnemyBag* bag = new EnemyBag();
		float yaw = DirectX::XMConvertToRadians(rand() % 360);
		DirectX::XMFLOAT2 dir;
		dir.x = sinf(yaw);
		dir.y = cosf(yaw);
		int len = rand() % 100 + 5;
		bag->SetPosition(DirectX::XMFLOAT3(dir.x * len, 0.0f, dir.y * len));
		bag->SetTerritory(bag->GetPosition(), 10.0f);
		bag->SetActiveflag(true);
		//bag->SetId(0);
		enemyManager.Register(bag);

	}
	enemyManager.SetEnemyMaxCount(enemyManager.GetEnemyCount());
}

void EnemySystem::Update(float elapsedTime) {
	for (int i = 0; i < maxEnemyCount; i++) {
		if (!EnemyManager::Instance().GetEnemy(i)->GetActiveflag()) {
			EnemyManager::Instance().GetEnemy(i)->SetReMoveflag(true);
			//EnemyManager::Instance().GetEnemy(i)->SetHealth(EnemyManager::Instance().GetEnemy(i)->GetMaxHealth());
			//float yaw = DirectX::XMConvertToRadians(rand() % 360);
			//DirectX::XMFLOAT2 dir;
			//dir.x = sinf(yaw);
			//dir.y = cosf(yaw);
			//int len = rand() % 100 + 5;
			//EnemyManager::Instance().GetEnemy(i)->SetPosition(DirectX::XMFLOAT3(dir.x * len, 0.0f, dir.y * len));
			//EnemyManager::Instance().GetEnemy(i)->SetTerritory(EnemyManager::Instance().GetEnemy(i)->GetPosition(), 10.0f);

		}

	}

}