#pragma once
#include"UI.h"
class BreakUI : public UI
{
public:
	//�摜�ꏊ�A�摜�̈ʒu���A�摜�̈ʒu�c�A�摜�̉��傫���A�摜�̏c�傫���A�_�ł��邩�H
	BreakUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight);

	// �X�V����
	void Update(float elapsedTime) {};

	// �`�揈��
	void Render(RenderContext& rc, SpriteShader* shader);
	// �j��
	void Destroy();
private:
	int weponMax;
};