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
	void Start(int Stag);
	// �X�V����
	void Update(float elapsedTime);

	int GetWave() { return wave; }

	float GetWaveTimer() { return waveTimer; }
	int GetWaveMaxTimer(int index) { return waveTimerMax[index]; }

public:
	int identity = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)
	int maxEnemyCount = 0;
	int maxEnemyDroneCount[3][3] = { { 0,0,0 },{ 100,50,50 },{ 50,50,50 } };
	int maxEnemyBagCount[3][3] = { {20,5,5},{0,0,0} ,{10,5,5}};
	float waveTimer = 0;
	const int waveTimerMax[4] = { 60,70,80,90 };
	int waveEnemyCount = 0;
	const int waveMaxEnemyCount[3][3] = { {20,25,30},{100,150,200} ,{60,115,170}};
	int wave = 0;
	int stag = 0;
};