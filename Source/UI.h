#pragma once
#include "Shader.h"
#include "Sprite.h"

//UI��{�N���X
class UI
{
public:
	UI() {};
	virtual ~UI() {};

	// �X�V����
	virtual void Update(float elapsedTime) {};

	// �`�揈��
	virtual void Render(RenderContext& rc, SpriteShader* shader) {};

	void SetUIPos(float uiWidth, float uiHeight) { this->uiWidth = uiWidth; this->uiHeight = uiHeight; };

	// �j��
	void Destroy();
protected:
	std::unique_ptr<Sprite> uiSprite;
	float   uiWidth;
	float   uiHeight;
	float   gaugeWidth;
	float   gaugeHeight;

	//UIManager* manager = nullptr;
};