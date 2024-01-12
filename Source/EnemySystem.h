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
	void Start(int Stag);
	// 更新処理
	void Update(float elapsedTime);

	int GetWave() { return wave; }

	float GetWaveTimer() { return waveTimer; }

public:
	int identity = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)
	int maxEnemyCount = 0;
	int maxEnemyDroneCount[3][3] = { { 0,0,0 },{ 100,50,50 },{ 50,50,50 } };
	int maxEnemyBagCount[3][3] = { {10,10,10},{0,0,0} ,{10,5,5}};
	float waveTimer = 0;
	const int waveTimerMax[4] = { 20,20,20,90 };
	int waveEnemyCount = 0;
	const int waveMaxEnemyCount[3][3] = { {10,20,30},{100,150,200} ,{60,115,170}};
	int wave = 0;
	int stag = 0;
};