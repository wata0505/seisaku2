#include"GaugeUI.h"
#include"UIManager.h"

GaugeUI::GaugeUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight, int gauge, int gaugeMax)
{
    uiSprite = std::make_unique<Sprite>(filename);
    this->uiWidth = uiWidth;
    this->uiHeight = uiHeight;
    this->gaugeWidth = gaugeWidth;
    this->gaugeHeight = gaugeHeight;
    this->gauge = gauge;
    this->gaugeMax = gaugeMax;
    // 生成時にマネージャーに登録する
    UIManager::Instance().Register(this);

}
void GaugeUI::Update(float elapsedTime) {
    float gaugeThrate = this->gauge / static_cast<float>(this->gaugeMax);
    if (thrate >= gaugeThrate) {
        thrate -= elapsedTime * uiSpeed;
    }
    if (thrate <= gaugeThrate) {
        thrate += elapsedTime * uiSpeed;
    }
}
void GaugeUI::Render(RenderContext& rc, SpriteShader* shader) {
    uiSprite->Render(rc.deviceContext,
        uiWidth,
        uiHeight,
        gaugeWidth * thrate,
        gaugeHeight,
        0.0f, 0.0f,
        static_cast<float>(uiSprite->GetWidth() * thrate),
        static_cast<float>(uiSprite->GetHeight()),
        0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );
    shader->Draw(rc, uiSprite.get());

}