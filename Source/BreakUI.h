#pragma once
#include"UI.h"
class BreakUI : public UI
{
public:
	//画像場所、画像の位置横、画像の位置縦、画像の横大きさ、画像の縦大きさ、点滅するか？
	BreakUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight);

	// 更新処理
	void Update(float elapsedTime) {};

	// 描画処理
	void Render(RenderContext& rc, SpriteShader* shader);
	// 破棄
	void Destroy();
private:
	int weponMax;
};