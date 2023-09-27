#include"BreakUI.h"
#include"UIManager.h"
#include"MainWepon.h"

BreakUI::BreakUI(const wchar_t* filename, float uiWidth, float uiHeight, float gaugeWidth, float gaugeHeight)
{
    uiSprite = std::make_unique<Sprite>(filename);
    this->uiWidth = uiWidth;
    this->uiHeight = uiHeight;
    this->gaugeWidth = gaugeWidth;
    this->gaugeHeight = gaugeHeight;
    // 生成時にマネージャーに登録する
    UIManager::Instance().Register(this);

}
void BreakUI::Render(RenderContext& rc, SpriteShader* shader) {
    MainWepon& mainWepon = MainWepon::Instance();
    for (int i = 0; i < Shield; i++) {
        uiSprite->Render(rc.deviceContext,
            uiWidth + 100 * mainWepon.GetUIWidth(i),
            uiHeight + mainWepon.GetUIHeight(i),
            gaugeWidth,
            gaugeHeight,
            0.0f, 0.0f,
            static_cast<float>(uiSprite->GetWidth()),
            static_cast<float>(uiSprite->GetHeight()),
            0.0f,
            1.0f, 0.0f, 0.0f, 1.0f
        );
        if (mainWepon.GetBreakFlag(i)) {
            shader->Draw(rc, uiSprite.get());
        }
    }
    
}