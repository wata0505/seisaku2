#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	model[Particle::Ball] = std::make_unique<Model>(".\\resources\\Particle\\Sphere.fbx", true, false);
	model[Particle::Ball]->ModelSerialize(".\\resources\\Particle\\Sphere.fbx");
	model[Particle::Ball]->ModelRegister(".\\resources\\Particle\\Sphere.fbx");
	model[Particle::Ball]->UpdateBufferDara(data[Particle::Ball].world[0]);
	//model = std::make_unique<Model>(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx", false,false);
	//model->ModelSerialize(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//model->ModelRegister(".\\resources\\Weapon\\Halberd\\PREF_halberd_22.fbx");
	//model[Particle::Lance] = std::make_unique<Model>(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx", true);
	//model[Particle::Lance]->ModelSerialize(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx");
	//model[Particle::Lance]->ModelRegister(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx");
	//model[Particle::Lance]->UpdateBufferDara(data[Particle::Lance].world[0]);
	model[Particle::Rock1] = std::make_unique<Model>(".\\resources\\ExampleStage\\rock_11.fbx", true);
	model[Particle::Rock1]->ModelSerialize(".\\resources\\ExampleStage\\rock_11.fbx");
	model[Particle::Rock1]->ModelRegister(".\\resources\\ExampleStage\\rock_11.fbx","tex.png");
	model[Particle::Rock1]->UpdateBufferDara(data[Particle::Rock1].world[0]);
	model[Particle::Rock2] = std::make_unique<Model>(".\\resources\\ExampleStage\\rock_10.fbx", true);
	model[Particle::Rock2]->ModelSerialize(".\\resources\\ExampleStage\\rock_10.fbx");
	model[Particle::Rock2]->ModelRegister(".\\resources\\ExampleStage\\rock_10.fbx","tex.png");
	model[Particle::Rock2]->UpdateBufferDara(data[Particle::Rock2].world[0]);
	model[Particle::Rock3] = std::make_unique<Model>(".\\resources\\ExampleStage\\rock_9.fbx", true);
	model[Particle::Rock3]->ModelSerialize(".\\resources\\ExampleStage\\rock_9.fbx");
	model[Particle::Rock3]->ModelRegister(".\\resources\\ExampleStage\\rock_9.fbx","tex.png");
	model[Particle::Rock3]->UpdateBufferDara(data[Particle::Rock3].world[0]);
}

ParticleManager::~ParticleManager()
{
	Clear();
}


// �X�V����
void ParticleManager::Update(float elapsedTime)
{
	//�X�V����
	for (int i = 0; i < Particle::Max; i++)
	{
		particleNo[i] = 0;
	}
	for (Particle* Particle : particles)
	{
		Particle->Update(elapsedTime);
	}
	// �j������
	// ��Particles�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//	 �X�V�I�������ɔp�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Particle* particle : removes)
	{
		/*
				std::vector�ŊǗ�����Ă���v�f��
				 �폜����ɂ�erase()�֐����g�p����B
			�@�p�����X�g�ɂ̃|�C���^����C�e���[�^�[��
					�������Aerase()�֐��ɓn���B
														*/
														// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Particle*>::iterator it = std::find(particles.begin(), particles.end(), particle);

		if (it != particles.end())
		{
			particles.erase(it);
		}

		//�e�ۂ̔p������
		delete particle;
	}
	//�p�����X�g���N���A
	removes.clear();

	//model->UpdateBufferDara(data.world[1]);
}


// �`�揈��
void ParticleManager::Render(ID3D11DeviceContext* dc, ParticleShader* shader)
{
	
	for (Particle* particle : particles)
	{
		particle->Render(dc, shader);
	}

}






// �e�ۓo�^
void ParticleManager::Register(Particle* Particle)
{
	particles.emplace_back(Particle);
}


// �e�ۑS�폜
void ParticleManager::Clear()
{
	for (Particle* Particle : particles)
	{
		delete Particle;
	}
	particles.clear();
}

void ParticleManager::Remove(Particle* Particle)
{
	/*���ځAParticles�̗v�f���폜���Ă��܂���
		�͈�for���ŕs����N�������߁A
			�p�����X�g�ɒǉ�����
	*/
	//�p�����X�g�ɒǉ�
	removes.insert(Particle);
}