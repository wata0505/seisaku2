#include "UIManager.h"




// �X�V����
void UIManager::Update(float elapsedTime)
{
	//�X�V����
	for (UI* ui : uis)
	{
		ui->Update(elapsedTime);
	}
	// �j������
	// ��Objects�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//	 �X�V�I�������ɔp�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (UI* ui : removes)
	{
		/*
				std::vector�ŊǗ�����Ă���v�f��
				 �폜����ɂ�erase()�֐����g�p����B
			�@�p�����X�g�ɂ̃|�C���^����C�e���[�^�[��
					�������Aerase()�֐��ɓn���B
														*/
														// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<UI*>::iterator it = std::find(uis.begin(), uis.end(), ui);

		if (it != uis.end())
		{
			uis.erase(it);
		}

		//�e�ۂ̔p������
		delete ui;
	}
	//�p�����X�g���N���A
	removes.clear();
}


// �`�揈��
void UIManager::Render(RenderContext& rc, SpriteShader* shader)
{
	for (UI* ui : uis)
	{
		ui->Render(rc, shader);
	}
}

// �e�ۓo�^
void UIManager::Register(UI* ui)
{
	uis.emplace_back(ui);
}


// �e�ۑS�폜
void UIManager::Clear()
{
	for (UI* ui : uis)
	{
		delete ui;
	}
	uis.clear();
}

void UIManager::Remove(UI* ui)
{
	/*���ځAuis�̗v�f���폜���Ă��܂���
		�͈�for���ŕs����N�������߁A
			�p�����X�g�ɒǉ�����
	*/
	//�p�����X�g�ɒǉ�
	removes.insert(ui);
}