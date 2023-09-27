#include"LockOnUI.h"
#include"UIManager.h"
#include"EnemyManager.h"
#include"Player.h"
#include"Graphics.h"

LockOnUI::LockOnUI(const wchar_t* filename)
{
	uiSprite= std::make_unique<Sprite>(filename);
	// 生成時にマネージャーに登録する
	UIManager::Instance().Register(this);

}
void LockOnUI::Update(float elapsedTime) {
    lockonAngle ++;
}
void LockOnUI::Render(RenderContext& rc, SpriteShader* shader) {
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    rc.deviceContext->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // 全ての敵の頭上にHPゲージを表示
    EnemyManager& enemyManager = EnemyManager::Instance();
    Player& player = Player::Instance();
    if (enemyManager.GetActiveEnemyCount() == 0)return;
    if (player.GetLockOnflag())
    {
        Enemy* enemy = enemyManager.GetEnemyFromId(player.GetLockEnemyId());
        if (!enemy)return;
        // ワールド座標からスクリーン座標へ変換し、敵の頭上にHPゲージスプライトを表示する
        if (!enemy->GetRenderflag())return;//エネミーが描画されてないなら描画しない
        DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&player.GetLockEnemyPos());
        // ワールド座標からスクリーン座標へ
        DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
            WorldPosition,
            viewport.TopLeftX,
            viewport.TopLeftY,
            Graphics::Instance().GetScreenWidth(),
            Graphics::Instance().GetScreenHeight(),
            viewport.MinDepth,
            viewport.MaxDepth,
            Projection,
            View,
            World
        );
        //スクリーン座標
        DirectX::XMFLOAT2 screenPosition;
        DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);
        //敵との距離でロックオンの大きさをかえる
        float length = player.GetEnemyLength();
        if (length > 20) length = 20;
        if (length < 10) length = 10;

        gaugeWidth = 100.0f + length;
        gaugeHeight = 100.0f + length;

        float dissolve = 1.0f - ((float)enemy->GetHealth() / (float)enemy->GetMaxHealth());
        uiSprite->Render(rc.deviceContext,
            screenPosition.x - gaugeWidth * 0.5,
            screenPosition.y - gaugeHeight * 0.5,
            gaugeWidth,
            gaugeHeight,
            0, 0,
            static_cast<float>(uiSprite->GetTextureWidth()),
            static_cast<float>(uiSprite->GetTextureHeight()),
            lockonAngle,
            3.0f, 1.0f, 1.0f, 1.0f, 0, dissolve
        );
        shader->Draw(rc, uiSprite.get());
    }

}