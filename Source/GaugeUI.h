#pragma once
#include "UI.h"

//ゲージUIクラス
class GaugeUI : public UI
{
public:
	GaugeUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight,int gauge,int gaugeMax);
	~GaugeUI() {};

	// 更新処理
	void Update(float elapsedTime);

	void SetGauge(int gauge) { this->gauge = gauge; };

	// 描画処理
	void Render(RenderContext& rc, SpriteShader* shader);

	// 破棄
	void Destroy();

private:
	float thrate = 1;
	int gauge;
	int gaugeMax;
	//UI更新速度
	float uiSpeed = 0.5;
	//UIManager* manager = nullptr;
};