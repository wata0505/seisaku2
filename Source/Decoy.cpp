#include"Decoy.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics.h"
#include "TrapManager.h"

Decoy::Decoy()
{

    model = std::make_unique<Model>(".\\resources\\Sentorygun\\gunmotiongun1.fbx", true);
    model->ModelSerialize(".\\resources\\Sentorygun\\gunmotiongun1.fbx");
    model->ModelRegister(".\\resources\\Sentorygun\\gunmotiongun1.fbx");


    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    renderdata = model->GetBufferData();

    radius = 1;
}
Decoy::~Decoy()
{

}

void Decoy::Update(float elapsedTime)
{
    CollisionVsEnemies();
    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    //���f���`����󂯓n��
    renderdata = model->GetBufferData();
}

void Decoy::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
    shader->Draw(dc, model.get(), renderdata);
}

void Decoy::DrawDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�N�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));


    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

//�G�Ƃ̏Փˏ���
void Decoy::CollisionVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //�S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;

        if (Collision::IntersectCylinderVsCylinder(
            enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
            position, territoryRange, height,
            outPosition))
        {
            //�G�l�~�[�̃^�[�Q�b�g���f�R�C�ɐݒ肷��

        }
    }
}