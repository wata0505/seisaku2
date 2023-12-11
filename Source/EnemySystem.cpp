#include "EnemySystem.h"
#include "EnemyBag.h"
#include "EnemyDrone.h"
EnemySystem::EnemySystem() {

	

}

void EnemySystem::Start() {
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < maxEnemyCount; i++) {
		EnemyBag* bag = new EnemyBag();
		//float yaw = DirectX::XMConvertToRadians(rand() % 360);
		//DirectX::XMFLOAT2 dir;
		//dir.x = sinf(yaw);
		//dir.y = cosf(yaw);
		DirectX::XMFLOAT2 pos = { 105 - float(rand() % 50), 40 };
		bag->SetPosition(DirectX::XMFLOAT3(pos.x, 0.0f, pos.y));
		bag->SetTerritory(bag->GetPosition(), 10.0f);
		bag->SetActiveflag(true);
		//bag->SetId(0);
		enemyManager.Register(bag);

	}
	for (int i = 0; i < maxEnemyCount; i++) {
		EnemyDrone* drone = new EnemyDrone();
		DirectX::XMFLOAT2 pos = { 105 - float(rand() % 50), 40 };
		drone->SetPosition(DirectX::XMFLOAT3(pos.x,2.0f, pos.y));
		drone->SetTerritory(drone->GetPosition(), 10.0f);
		drone->SetActiveflag(true);
		enemyManager.Register(drone);
	}
	enemyManager.SetEnemyMaxCount(enemyManager.GetEnemyCount());


}

void EnemySystem::Update(float elapsedTime) {
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++) {
		if (!EnemyManager::Instance().GetEnemy(i)->GetActiveflag() || waveTimer > waveTimerMax) {
			EnemyManager::Instance().GetEnemy(i)->SetReMoveflag(true);
			//EnemyManager::Instance().GetEnemy(i)->SetReMoveTimer(10);
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
	if (waveTimer > waveTimerMax) {
		wave++;
		waveTimer = 0;
	}
	waveTimer += elapsedTime;

}