#include"LockOnUI.h"
#include"UIManager.h"
#include"EnemyManager.h"
#include"Player.h"
#include"Graphics.h"

LockOnUI::LockOnUI(const wchar_t* filename)
{
	uiSprite= std::make_unique<Sprite>(filename);
	// �������Ƀ}�l�[�W���[�ɓo�^����
	UIManager::Instance().Register(this);

}
void LockOnUI::Update(float elapsedTime) {
    lockonAngle ++;
}
void LockOnUI::Render(RenderContext& rc, SpriteShader* shader) {
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    rc.deviceContext->RSGetViewports(&numViewports, &viewport);

    // �ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // �S�Ă̓G�̓����HP�Q�[�W��\��
    EnemyManager& enemyManager = EnemyManager::Instance();
    Player& player = Player::Instance();
    if (enemyManager.GetActiveEnemyCount() == 0)return;
    if (player.GetLockOnflag())
    {
        Enemy* enemy = enemyManager.GetEnemyFromId(player.GetLockEnemyId());
        if (!enemy)return;
        // ���[���h���W����X�N���[�����W�֕ϊ����A�G�̓����HP�Q�[�W�X�v���C�g��\������
        if (!enemy->GetRenderflag())return;//�G�l�~�[���`�悳��ĂȂ��Ȃ�`�悵�Ȃ�
        DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&player.GetLockEnemyPos());
        // ���[���h���W����X�N���[�����W��
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
        //�X�N���[�����W
        DirectX::XMFLOAT2 screenPosition;
        DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);
        //�G�Ƃ̋����Ń��b�N�I���̑傫����������
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