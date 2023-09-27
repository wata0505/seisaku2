#pragma once
#include<vector>
#include<set>
#include"ParticleSprite.h"

//�e�ۃ}�l�[�W���[
class ParticleSpriteManager
{
public:
	ParticleSpriteManager();
	~ParticleSpriteManager();
	static ParticleSpriteManager& Instance()
	{
		static ParticleSpriteManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	//void Render(ID3D11DeviceContext* dc, ParticleSpriteShader* shader);

	// �I�u�W�F�N�g�o�^
	void Register(ParticleSprite* ParticleSprite);

	//�@�I�u�W�F�N�g�S�폜
	void Clear();

	// �I�u�W�F�N�g���擾
	int GetParticleSpriteCount() const { return static_cast<int>(particleSprites.size()); }

	// �I�u�W�F�N�g�擾
	ParticleSprite* GetParticleSprite(int index) { return particleSprites.at(index); }

	// �I�u�W�F�N�g�폜
	void Remove(ParticleSprite* ParticleSprite);

	std::vector<ParticleSprite*> GetParticleSprites() {return particleSprites;}

	int GetParticleCount()const { return static_cast<int>(particleSprites.size()); }

	

private:
	std::shared_ptr<Model> model = { nullptr };
	std::vector<ParticleSprite*> particleSprites;
	std::set<ParticleSprite*> removes;

	int  ParticleSpriteNo = 0;
};