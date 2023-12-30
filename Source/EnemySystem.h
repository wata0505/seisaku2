#pragma once
#pragma once

#include <vector>
#include "Enemy.h"
#include "EnemyManager.h"

// エネミーマネージャー
class EnemySystem
{
private:
	EnemySystem();
	~EnemySystem() {}

public:

	// 唯一のインスタンス取得
	static EnemySystem& Instance()
	{
		static EnemySystem instance;
		return instance;
	}
	void Start();
	// 更新処理
	void Update(float elapsedTime);

	int GetWave() { return wave; }

	float GetWaveTimer() { return waveTimer; }

public:
	int identity = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)
	int maxEnemyCount = 100;
	float waveTimer = 0;
	const int waveTimerMax[4] = { 60,70,80,90 };
	int waveEnemyCount = 0;
	const int waveMaxEnemyCount[4] = {100,100,100,100};
	int wave = 0;

};