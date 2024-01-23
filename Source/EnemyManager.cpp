#include <imgui.h>
#include "EnemyManager.h"
#include "MetaAI.h"
#include "Collision.h"


// �X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	// ��enemies�͈̔�for���̒���erase()����ƕs����������Ă��܂����߁A
	// �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Enemy* enemy : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// �폜
		delete enemy;
	}
	// �j�����X�g���N���A
	removes.clear();
	// �G���m�̏Փ˔���
	CollisionEnemyVsEnemies();
}

void EnemyManager::Render(ID3D11DeviceContext* context, ModelShader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}
void EnemyManager::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Afterimagerender(immediate_context, shader);
	}
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	// TODO 05_06 �e�G�l�~�[��ID��t�^����
	enemy->SetId(5 + static_cast<int>(enemies.size()));

	enemies.emplace_back(enemy);
}
// �G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
	// �j�����X�g�ɒǉ�
	removes.emplace_back(enemy);
}
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

void EnemyManager::DrawDebugPrimitive()
{
	for (Enemy* enemy : enemies) {
		// �Փ˔���p�f�o�b�O����`��
		enemy->DrawDebugPrimitive();
	}
}
void EnemyManager::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(980, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Enemy", nullptr, ImGuiWindowFlags_None))
	{
		for (Enemy* enemy : enemies) {
			// �G�l�~�[���\��
			enemy->DrawDebugGUI();
		}
	}
	ImGui::End();
}
void EnemyManager::CollisionEnemyVsEnemies()
{
	// �S�Ă̓G�Ƒ�������ŏՓˏ���
	for (int i = 0; i < enemies.size(); ++i)
	{
		Enemy* enemyA = enemies.at(i);
		for (int j = i + 1; j < enemies.size(); ++j)
		{
			Enemy* enemyB = enemies.at(j);
			// �Փˏ���
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsSphere(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				outPosition)
				)
			{
				// �����o����̈ʒu�ݒ�
				enemyB->SetPosition(outPosition);
			}
		}
	}
}
int EnemyManager::GetActiveEnemyCount() {
	int activeCount = 0;
	for (Enemy* enemy : enemies)
	{
		if (enemy->GetActiveflag()) {
			activeCount++;
		}
	}
	return activeCount;
}

// ID����G�l�~�[���擾����
Enemy* EnemyManager::GetEnemyFromId(int id)
{
	for (Enemy* enemy : enemies)
	{
		if (enemy->GetId() == id)
			return enemy;
	}
	return nullptr;
}