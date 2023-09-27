#pragma once
#include "Shader.h"
#include "Sprite.h"

//UI基本クラス
class UI
{
public:
	UI() {};
	virtual ~UI() {};

	// 更新処理
	virtual void Update(float elapsedTime) {};

	// 描画処理
	virtual void Render(RenderContext& rc, SpriteShader* shader) {};

	void SetUIPos(float uiWidth, float uiHeight) { this->uiWidth = uiWidth; this->uiHeight = uiHeight; };

	// 破棄
	void Destroy();
protected:
	std::unique_ptr<Sprite> uiSprite;
	float   uiWidth;
	float   uiHeight;
	float   gaugeWidth;
	float   gaugeHeight;

	//UIManager* manager = nullptr;
};