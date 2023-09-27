#pragma once
#pragma once
#include<vector>
#include<set>
#include"InstancingSprite.h"

//�e�ۃ}�l�[�W���[
class InstancingSpriteManager
{
public:

public:
	InstancingSpriteManager();
	~InstancingSpriteManager();
	static InstancingSpriteManager& Instance()
	{
		static InstancingSpriteManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, InstancingSpriteShader* shader);

	// �I�u�W�F�N�g�o�^
	void Register(InstancingSprite* InstancingSprite);

	//�@�I�u�W�F�N�g�S�폜
	void Clear();

	// �I�u�W�F�N�g���擾
	int GetInstancingSpriteCount() const { return static_cast<int>(InstancingSprites.size()); }

	// �I�u�W�F�N�g�擾
	InstancingSprite* GetInstancingSprite(int index) { return InstancingSprites.at(index); }

	InstancingSpriteConstants GetSpriteConstants(int index) { return data[index]; }

	Sprite* GetMoveSprite(int i) { return moveSprite[i].get(); }
	Sprite* GetTexSprite(int i) { return texSprite[i].get(); }

	// �I�u�W�F�N�g�폜
	void Remove(InstancingSprite* InstancingSprite);

	int GetInstancingSpriteNo(int i) { return instancingSpriteNo[i]; }


private:
	std::shared_ptr<Sprite> moveSprite[InstancingSprite::Max] = { nullptr };
	std::shared_ptr<Sprite> texSprite[InstancingSprite::Max] = { nullptr };
	std::vector<InstancingSprite*> InstancingSprites;
	InstancingSpriteConstants data[InstancingSprite::Max];
	std::set<InstancingSprite*> removes;

	int  instancingSpriteNo[InstancingSprite::Max] = { 0 };
};