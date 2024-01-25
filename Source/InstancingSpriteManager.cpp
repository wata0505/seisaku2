#include "InstancingSpriteManager.h"
#include"EffectTexAll.h"

InstancingSpriteManager::InstancingSpriteManager()
{
	//model = std::make_unique<Model>(".\\resources\\InstancingSprite\\Sphere.fbx", true, false);
	//model->ModelSerialize(".\\resources\\InstancingSprite\\Sphere.fbx");
	////model->ModelCreate(".\\resources\\ExampleStage\\ExampleStage.fbx");
	//model->ModelRegister(".\\resources\\InstancingSprite\\Sphere.fbx");
	//model = std::make_unique<Model>(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx", false,false);
	//model->ModelSerialize(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//model->ModelRegister(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//moveSprite[InstancingSprite::InstancingSpriteSoft] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\InstancingSprite_Soft.png");
	//moveSprite[InstancingSprite::InstancingSpriteLine] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Line9.png");
	//moveSprite[InstancingSprite::InstancingSpriteImpact] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Shock_wave1.png");
	//moveSprite[InstancingSprite::InstancingSpriteWind] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\wind04.png");
	moveSprite[InstancingSprite::FlameBreath] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Fire.png");
	moveSprite[InstancingSprite::SeirlConvergence] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Particle_Soft.png");
	moveSprite[InstancingSprite::VortexDiffusion] = moveSprite[InstancingSprite::SeirlConvergence];
	moveSprite[InstancingSprite::Corruption] = std::make_unique<Sprite>(L".\\resources\\Textures\\hart.png");
	moveSprite[InstancingSprite::Corruption2] = std::make_unique<Sprite>(L".\\resources\\Textures\\OIP.png");

	//texSprite[InstancingSprite::TexNull] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\dummy.png");
	texSprite[InstancingSprite::FlameBreath] = EffectTexAll::Instance().GetSprite((int)EffectTexAll::EfTexAll::Flame);
	texSprite[InstancingSprite::SeirlConvergence] = EffectTexAll::Instance().GetSprite((int)EffectTexAll::EfTexAll::Impact);
	texSprite[InstancingSprite::VortexDiffusion] = texSprite[InstancingSprite::SeirlConvergence];
	texSprite[InstancingSprite::Corruption] = moveSprite[InstancingSprite::Corruption];
	texSprite[InstancingSprite::Corruption2] = moveSprite[InstancingSprite::Corruption2];

}

InstancingSpriteManager::~InstancingSpriteManager()
{
	Clear();
}


// �X�V����
void InstancingSpriteManager::Update(float elapsedTime)
{
	//�X�V����
	for (int i = 0; i < InstancingSprite::Max; i++)
	{
		instancingSpriteNo[i] = 0;
	}
	for (InstancingSprite* instancingSprite : InstancingSprites)
	{
		instancingSprite->Update(elapsedTime);
		int type = instancingSprite->GetSpriteType();
		//�^�C�v���Ƃ̔z��ɏ����i�[
		if (instancingSpriteNo[type] < MAX_INSTANCES) {
			data[type].world[instancingSpriteNo[type]] = instancingSprite->GetPosition();
			data[type].materialColor[instancingSpriteNo[type]] = instancingSprite->GetMaterialColor();
			data[type].spriteSize[instancingSpriteNo[type]] = instancingSprite->GetSpriteSize();
			data[type].timer[instancingSpriteNo[type]] = { instancingSprite->GetTexTimer(),0,0,0};
			data[type].dissolve[instancingSpriteNo[type]] = instancingSprite->GetUvStatus();
			instancingSpriteNo[type]++;
		}
	}
	// �j������
	// ��InstancingSprites�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//	 �X�V�I�������ɔp�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (InstancingSprite* instancingSprite : removes)
	{
		/*
				std::vector�ŊǗ�����Ă���v�f��
				 �폜����ɂ�erase()�֐����g�p����B
			�@�p�����X�g�ɂ̃|�C���^����C�e���[�^�[��
					�������Aerase()�֐��ɓn���B
														*/
														// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<InstancingSprite*>::iterator it = std::find(InstancingSprites.begin(), InstancingSprites.end(), instancingSprite);

		if (it != InstancingSprites.end())
		{
			InstancingSprites.erase(it);
		}

		//�e�ۂ̔p������
		delete instancingSprite;
	}
	//�p�����X�g���N���A
	removes.clear();

	//model->UpdateBufferDara(data.world[1]);
}


// �`�揈��
void InstancingSpriteManager::Render(ID3D11DeviceContext* dc, InstancingSpriteShader* shader)
{
	for (int i = 0; i < InstancingSprite::Max; i++)
	{
		
		
	}

}






// �e�ۓo�^
void InstancingSpriteManager::Register(InstancingSprite* InstancingSprite)
{
	InstancingSprites.emplace_back(InstancingSprite);
}


// �e�ۑS�폜
void InstancingSpriteManager::Clear()
{
	for (InstancingSprite* instancingSprite : InstancingSprites)
	{
		delete instancingSprite;
	}
	InstancingSprites.clear();
}

void InstancingSpriteManager::Remove(InstancingSprite* instancingSprite)
{
	/*���ځAInstancingSprites�̗v�f���폜���Ă��܂���
		�͈�for���ŕs����N�������߁A
			�p�����X�g�ɒǉ�����
	*/
	//�p�����X�g�ɒǉ�
	removes.insert(instancingSprite);
}