#pragma once
#include "UI.h"

//�Q�[�WUI�N���X
class GaugeUI : public UI
{
public:
	GaugeUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight,int gauge,int gaugeMax);
	~GaugeUI() {};

	// �X�V����
	void Update(float elapsedTime);

	void SetGauge(int gauge) { this->gauge = gauge; };

	// �`�揈��
	void Render(RenderContext& rc, SpriteShader* shader);

	// �j��
	void Destroy();

private:
	float thrate = 1;
	int gauge;
	int gaugeMax;
	//UI�X�V���x
	float uiSpeed = 0.5;
	//UIManager* manager = nullptr;
};