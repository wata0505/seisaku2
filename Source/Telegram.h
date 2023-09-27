#pragma once

// ���M���郁�b�Z�[�W�̎��
enum class MESSAGE_TYPE : int
{
	MsgCallHelp,				// ���Ԃ��Ă�(Enemy�A���^AI�̗����Ŏg�p)
	MsgChangeAttackRight,		// �U�����̈ڏ�(Enemy���烁�^AI�ւ̃��b�Z�[�W�p)
	MsgGiveAttackRight,			// �U������^����i���^AI����Enemy�ւ̃��b�Z�[�W�p�j
	MsgDontGiveAttackRight,		// �U������^���Ȃ��i���^AI����Enemy�ւ̃��b�Z�[�W�p�j
	MsgAskAttackRight,			// �U������v��(Enemy���烁�^AI�ւ̃��b�Z�[�W�p)
};

class Telegram
{
public:
	int sender;			// ������ID
	int receiver;		// �󂯂Ă�ID
	MESSAGE_TYPE msg;	// ���b�Z�[�WNO

	//�R���X�g���N�^
	Telegram(int sender, int receiver, MESSAGE_TYPE msg) :sender(sender), receiver(receiver), msg(msg)
	{}
};
