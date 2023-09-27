#include "ParticleSpriteManager.h"


ParticleSpriteManager::ParticleSpriteManager()
{
	
}

ParticleSpriteManager::~ParticleSpriteManager()
{
	Clear();
}


// �X�V����
void ParticleSpriteManager::Update(float elapsedTime)
{
	//�X�V����
	ParticleSpriteNo = 0;
	for (ParticleSprite* particleSprite : particleSprites)
	{
		particleSprite->Update(elapsedTime);
	}
	// �j������
	// ��ParticleSprites�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//	 �X�V�I�������ɔp�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (ParticleSprite* particleSprite : removes)
	{
		/*
				std::vector�ŊǗ�����Ă���v�f��
				 �폜����ɂ�erase()�֐����g�p����B
			�@�p�����X�g�ɂ̃|�C���^����C�e���[�^�[��
					�������Aerase()�֐��ɓn���B
														*/
														// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<ParticleSprite*>::iterator it = std::find(particleSprites.begin(), particleSprites.end(), particleSprite);

		if (it != particleSprites.end())
		{
			particleSprites.erase(it);
		}

		//�e�ۂ̔p������
		delete particleSprite;
	}
	//�p�����X�g���N���A
	removes.clear();
}


// �`�揈��
//void ParticleSpriteManager::Render(ID3D11DeviceContext* dc, ParticleSpriteShader* shader)
//{
//
//	//shader->Draw(dc, model.get(), data, ParticleSpriteNo);
//	//for (ParticleSprite* ParticleSprite : ParticleSprites)
//	//{
//	//	ParticleSprite->Render(dc, shader);
//	//}
//
//}





// �p�[�e�B�N�o�^
void ParticleSpriteManager::Register(ParticleSprite* ParticleSprite)
{
	particleSprites.emplace_back(ParticleSprite);
}


// �p�[�e�B�N���S�폜
void ParticleSpriteManager::Clear()
{
	for (ParticleSprite* ParticleSprite : particleSprites)
	{
		delete ParticleSprite;
	}
	particleSprites.clear();
}

void ParticleSpriteManager::Remove(ParticleSprite* ParticleSprite)
{
	/*���ځAParticleSprites�̗v�f���폜���Ă��܂���
		�͈�for���ŕs����N�������߁A
			�p�����X�g�ɒǉ�����
	*/
	//�p�����X�g�ɒǉ�
	removes.insert(ParticleSprite);
}