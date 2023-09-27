#include"BaseUI.h"
#include"UIManager.h"

BaseUI::BaseUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight,bool flashing)
{
    uiSprite = std::make_unique<Sprite>(filename);
    this->uiWidth = uiWidth;
    this->uiHeight = uiHeight;
    this->gaugeWidth = gaugeWidth;
    this->gaugeHeight = gaugeHeight;
    this->flashing = flashing;
    // 生成時にマネージャーに登録する
    UIManager::Instance().Register(this);

}
void BaseUI::Render(RenderContext& rc, SpriteShader* shader) {

    uiSprite->Render(rc.deviceContext,
        uiWidth,
        uiHeight,
        gaugeWidth,
        gaugeHeight,
        0.0f, 0.0f,
        static_cast<float>(uiSprite->GetWidth()),
        static_cast<float>(uiSprite->GetHeight()),
        0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );
    if (timer / 20 % 2 || !flashing) {
        shader->Draw(rc, uiSprite.get());
    }

    timer++;
}