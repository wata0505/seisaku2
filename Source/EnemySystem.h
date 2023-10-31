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



public:
	int identity = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)
	int maxEnemyCount = 20;
};