#pragma once
#pragma once

#include <vector>
#include "Enemy.h"
#include "EnemyManager.h"

// �G�l�~�[�}�l�[�W���[
class EnemySystem
{
private:
	EnemySystem();
	~EnemySystem() {}

public:

	// �B��̃C���X�^���X�擾
	static EnemySystem& Instance()
	{
		static EnemySystem instance;
		return instance;
	}
	void Start();
	// �X�V����
	void Update(float elapsedTime);

	int GetWave() { return wave; }

	float GetWaveTimer() { return waveTimer; }

public:
	int identity = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)
	int maxEnemyCount = 100;
	float waveTimer = 0;
	const int waveTimerMax[4] = { 60,70,80,90 };
	int waveEnemyCount = 0;
	const int waveMaxEnemyCount[4] = {100,100,100,100};
	int wave = 0;

};