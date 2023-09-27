#pragma once
#include <set>
#include "Player.h"
#include "Telegram.h"
#include "EnemyManager.h"

class Meta
{
private:
	Player* player = nullptr;
	EnemyManager* enemyManager = nullptr;

public:
	enum class Identity : int
	{
		Meta = 0,
		Player = 1,	// �v���C���[ID��1�`4
		Enemy = 5	// �G�l�~�[ID��5�ȍ~��t�^
	};
	// �R���X�g���N�^
	Meta(Player* pl, EnemyManager* eManager);
	~Meta() {}
	// �C���X�^���X�擾
	static Meta& Instance();
	// MetaAI�̋@�\��ǉ����Ă���
	// �X�V����
	void Update();
	// ���b�Z�[�W��M�����Ƃ��̏���
	bool OnMessage(const Telegram& msg);
	// ���b�Z�[�W���M�֐�
	void SendMessaging(int sender, int receiver, MESSAGE_TYPE msg);

};