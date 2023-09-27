#include "MetaAI.h"

static Meta* instance = nullptr;
Meta& Meta::Instance()
{
	return *instance;
}
Meta::Meta(Player* pl, EnemyManager* eManager) : player(pl), enemyManager(eManager)
{
	instance = this;
}
void Meta::Update()
{
	// �����Ŏ����I�Ƀ��^AI���Ď����Ă��鏈��������΋L�ڂ���
	// ���ۑ�ł̓��^AI�̃C�x���g�g���K�[�̓��b�Z�[�W����M�����Ƃ��̂��ߋL�q���鏈���͖���
}

// ���b�Z�[�W��M�����Ƃ��̏���
bool Meta::OnMessage(const Telegram& telegram)
{
	std::vector<Enemy*> enmVec;
	enmVec.clear();
	int enemyCount = enemyManager->GetEnemyCount();
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MsgCallHelp://���ꂩ���v���C���[�𔭌������̂ő��̓G���Ă�
		for (int i = 0; i < enemyCount; ++i)//�S�Ă̓G�̃��X�g
		{
			int enemyId = enemyManager->GetEnemy(i)->GetId();
			// ���M�҂������G�l�~�[�Ƀ��b�Z�[�W�𑗂�
			if (telegram.sender != enemyId)
			{
				// TODO 05_05 ���Ԃ��Ăԃ��b�Z�[�W�̑��M
				// �N�����v���C���[�𔭌������̂ő��M�҂������G�Ƀ��b�Z�[�W�𑗂�
				// ���^AI����e�G�ɑ��郁�b�Z�[�W�� MsgCallHelp �ł��B
				SendMessaging(0, enemyId, MESSAGE_TYPE::MsgCallHelp);
			}
		}
		return true;
		break;
	case MESSAGE_TYPE::MsgChangeAttackRight:
		//�G�l�~�[�}�l�[�W������AttackRange�ȓ��̃G�l�~�[�����̔z����쐬��
		for (int i = 0; i < enemyCount; ++i)
		{
			Enemy* enemy = enemyManager->GetEnemy(i);
			DirectX::XMFLOAT3 position = enemy->GetPosition();
			DirectX::XMFLOAT3 targetPosition =Player::Instance().GetPosition();
			float vx = targetPosition.x - position.x;
			float vy = targetPosition.y - position.y;
			float vz = targetPosition.z - position.z;
			float dist = sqrtf(vx * vx + vy * vy + vz * vz);

			if (dist < enemy->GetSearchRange())
			{
				enmVec.push_back(enemy);
			}
		}
		if (enmVec.size() > 0)
		{
			// TODO 05_05 �U�����̕t�^
			// enmVec�̃T�C�Y�ŗ������擾���A�擾�����l���C���f�b�N�X�l�Ƃ���
			// enmVec�ɓo�^���ꂽ�G�̈�l�ɍU������^����
			// �U������^���郁�b�Z�[�W��MsgGiveAttackRight�ł��B
			int random = 0;
			random = rand() % enmVec.size();
			SendMessaging(0, enmVec[random]->GetId(), MESSAGE_TYPE::MsgGiveAttackRight);
		}
		return true;
		break;
	case MESSAGE_TYPE::MsgAskAttackRight: //�G����A�ŏ��̍U������v�����ꂽ
		int count = 0;//�S�Ă̓G�ɂ��āA�U�����������Ă��Ȃ������ׂ�B
		for (int i = 0; i < enemyCount; ++i)
		{
			Enemy* enemy = enemyManager->GetEnemy(i);
			if (enemy->GetAttackFlg()) count++;
		}
		//���ɍU���������Ă���G�����Ȃ��Ȃ�A�v�����Ă����G�ɍU������^���郁�b�Z�[�W�𑗂�B
		//�v�����Ă����G��id��telegram����擾�o���܂��B
		if (count < 2)
		{
			// TODO 05_05 �U�����̕t�^
			// �N���U�����������Ă��Ȃ����MetaAI���瑗�M�҂ɍU�����t�^�̃��b�Z�[�W�𑗂�
			// ���b�Z�[�W��MsgGiveAttackRight
			SendMessaging(0, telegram.sender, MESSAGE_TYPE::MsgGiveAttackRight);

		}
		return true;
		break;
	}
	return false;
}
void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	if (receiver == static_cast<int>(Meta::Identity::Meta))
	{// MetaAI���̎�
		//���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, msg);
		// ���b�Z�[�W���M
		OnMessage(telegram);
	}
	else
	{// �G�l�~�[����M�҂̂Ƃ��A�ǂ̃G�l�~�[�ɑ��M���邩
		// ��M�҂̃|�C���^���擾
		Enemy* receiveEnemy = enemyManager->GetEnemyFromId(receiver);
		//���V�[�o�[���Ȃ��Ƃ��֐����I������
		if (receiveEnemy == nullptr) return;
		//���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, msg);
		//�f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		receiveEnemy->OnMessage(telegram);
	}
}