#pragma once
#include "UI.h"

//�gUI�N���X
class BaseUI : public UI
{
public:
	//�摜�ꏊ�A�摜�̈ʒu���A�摜�̈ʒu�c�A�摜�̉��傫���A�摜�̏c�傫���A�_�ł��邩�H
	BaseUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight,bool flashing = false);
	~BaseUI() {};

	// �X�V����
	void Update(float elapsedTime) {};

	// �`�揈��
	void Render(RenderContext& rc, SpriteShader* shader);
	// �j��
	void Destroy();
private:
	//�_�Ńt���O
	bool flashing = false;
	int timer = 0;
	//UIManager* manager = nullptr;
};