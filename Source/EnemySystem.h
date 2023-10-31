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



public:
	int identity = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)
	int maxEnemyCount = 20;
};