#pragma once

#include<vector>
#include<set>
#include"UI.h"

//�e�ۃ}�l�[�W���[
class UIManager
{
public:
	UIManager() {};
	~UIManager() {};

	// �B��̃C���X�^���X�擾
	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}
	// �X�V����
	virtual void Update(float elapsedTime);

	// �`�揈��
	virtual void Render(RenderContext& rc, SpriteShader* shader);

	// �I�u�W�F�N�g�o�^
	void Register(UI* ui);

	//�@�I�u�W�F�N�g�S�폜
	void Clear();

	// �I�u�W�F�N�g���擾
	int GetUICount() const { return static_cast<int>(uis.size()); }

	// �I�u�W�F�N�g�擾
	UI* GetUI(int index) { return uis.at(index); }

	// �I�u�W�F�N�g�폜
	void Remove(UI* ui);



private:
	std::vector<UI*> uis;
	std::set<UI*> removes;

};