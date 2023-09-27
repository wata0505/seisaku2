#pragma once

#include <vector>
#include "Enemy.h"

// �G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

public:

	// �B��̃C���X�^���X�擾
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	// �G�l�~�[�o�^
	void Register(Enemy* enemy);

	// ������
	void Clear();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// // �f�o�b�O���\��
	void DrawDebugGUI();

	// �G�l�~�[���擾
	int GetEnemyCount()const { return static_cast<int>(enemies.size()); }

	// �A�N�e�B�u�G�l�~�[���擾
	int GetActiveEnemyCount();

	void SetEnemyMaxCount(int max){ enemyMaxCount = max; }

	int GetEnemyMaxCount()const { return enemyMaxCount; }

	// �G�l�~�[�擾
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	

	// �G�l�~�[�폜
	void Remove(Enemy* enemy);

	// �t�^����ID����G�l�~�[���擾
	Enemy* GetEnemyFromId(int id);

private:
	std::vector<Enemy*> enemies;
	std::vector<Enemy*> removes;

	// �G�l�~�[���m�̏Փˏ���
	void CollisionEnemyVsEnemies();

public:
	int identity = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)
	int enemyMaxCount = 0;
};