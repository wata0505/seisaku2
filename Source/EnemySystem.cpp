#include "EnemySystem.h"
#include "EnemyBag.h"
#include "EnemyDrone.h"
EnemySystem::EnemySystem() {
	
	

}

void EnemySystem::Start(int Stag) {
	wave= 0;
	waveTimer = 0;
	stag = Stag;
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < maxEnemyDroneCount[Stag][j]; i++) {
			EnemyDrone* bag = new EnemyDrone();
			//float yaw = DirectX::XMConvertToRadians(rand() % 360);
			//DirectX::XMFLOAT2 dir;
			//dir.x = sinf(yaw);
			//dir.y = cosf(yaw);
			DirectX::XMFLOAT2 pos = { 155 - float(rand() % 95), 40 };
			bag->SetPosition(DirectX::XMFLOAT3(pos.x, 0.0f, pos.y));
			bag->SetTerritory(bag->GetPosition(), 10.0f);
			bag->SetActiveflag(true);
			if (enemyManager.GetEnemyCount() >= waveMaxEnemyCount[stag][wave])	bag->SetActiveflag(false);
			//bag->SetId(0);
			enemyManager.Register(bag);

		}
		for (int i = 0; i < maxEnemyBagCount[Stag][j]; i++) {
			EnemyBag* bag = new EnemyBag(false,stag,j);
			//float yaw = DirectX::XMConvertToRadians(rand() % 360);
			//DirectX::XMFLOAT2 dir;
			//dir.x = sinf(yaw);
			//dir.y = cosf(yaw);
			DirectX::XMFLOAT2 pos = { 105 - float(rand() % 50), 40 };
			bag->SetPosition(DirectX::XMFLOAT3(pos.x, 0.0f, pos.y));
			bag->SetTerritory(bag->GetPosition(), 10.0f);
			bag->SetActiveflag(true);
			if (enemyManager.GetEnemyCount() >= waveMaxEnemyCount[stag][wave])	bag->SetActiveflag(false);
			//bag->SetId(0);
			enemyManager.Register(bag);

		}
	}
	enemyManager.SetEnemyMaxCount(enemyManager.GetEnemyCount());

}

void EnemySystem::Update(float elapsedTime) {
	EnemyManager& enemyManager = EnemyManager::Instance();
	if (weve < 3) {
		for (int i = 0; i < waveMaxEnemyCount[stag][wave]; i++) {

			if (!EnemyManager::Instance().GetEnemy(i)->GetActiveflag() || waveTimer > waveTimerMax[wave]) {
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
	}
	if (waveTimer > waveTimerMax[wave]) {
		wave++;
		waveTimer = 0;
		
	}
	waveTimer += elapsedTime;

}