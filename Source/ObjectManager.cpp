#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	Clear();
}


// �X�V����
void ObjectManager::Update(float elapsedTime)
{
	//�X�V����
	for (Object* Object : Objects)
	{
		Object->Update(elapsedTime);
	}
	// �j������
	// ��Objects�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//	 �X�V�I�������ɔp�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Object* object : removes)
	{
		/*
				std::vector�ŊǗ�����Ă���v�f��
				 �폜����ɂ�erase()�֐����g�p����B
			�@�p�����X�g�ɂ̃|�C���^����C�e���[�^�[��
					�������Aerase()�֐��ɓn���B
														*/
														// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Object*>::iterator it = std::find(Objects.begin(), Objects.end(), object);

		if (it != Objects.end())
		{
			Objects.erase(it);
		}

		//�e�ۂ̔p������
		delete object;
	}
	//�p�����X�g���N���A
	removes.clear();
}


// �`�揈��
void ObjectManager::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	for (Object* Object : Objects)
	{
		Object->Render(dc, shader);
	}
}

void ObjectManager::TrailRender(RenderContext& rc, SwordTrailShader* shader)
{
	for (Object* Object : Objects)
	{
		Object->TrailRender(rc, shader);
	}

}

// �f�o�b�O�v���~�e�B�u�`��
void ObjectManager::DrawDebugPrimitive()
{
	for (Object* Object : Objects)
	{
		Object->DrawDebugPrimitive();
	}
}


// �e�ۓo�^
void ObjectManager::Register(Object* Object)
{
	Objects.emplace_back(Object);
}


// �e�ۑS�폜
void ObjectManager::Clear()
{
	for (Object* Object : Objects)
	{
		delete Object;
	}
	Objects.clear();
}

void ObjectManager::Remove(Object* Object)
{
	/*���ځAObjects�̗v�f���폜���Ă��܂���
		�͈�for���ŕs����N�������߁A
			�p�����X�g�ɒǉ�����
	*/
	//�p�����X�g�ɒǉ�
	removes.insert(Object);
}
