#pragma once
#include<vector>
#include<set>
#include"Particle.h"

//�e�ۃ}�l�[�W���[
class ParticleManager
{
public:

public:
	ParticleManager();
	~ParticleManager();
	static ParticleManager& Instance()
	{
		static ParticleManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, ParticleShader* shader);

	// �I�u�W�F�N�g�o�^
	void Register(Particle* Particle);

	//�@�I�u�W�F�N�g�S�폜
	void Clear();

	// �I�u�W�F�N�g���擾
	int GetParticleCount() const { return static_cast<int>(particles.size()); }

	// �I�u�W�F�N�g�擾
	Particle* GetParticle(int index) { return particles.at(index); }


	std::shared_ptr<Model> GetModel(int type) { return model[type]; }

	// �I�u�W�F�N�g�폜
	void Remove(Particle* Particle);



private:
	std::shared_ptr<Model> model[Particle::Max] = { nullptr };
	std::vector<Particle*> particles;
	ParticleConstants data[Particle::Max];
	std::set<Particle*> removes;

	int  particleNo[Particle::Max] = { 0 };
};